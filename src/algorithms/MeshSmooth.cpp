#include <stdafx.h>
//#include <MeshProcess/DeformationImpl/MeshSmooth.h>
//#include <Eigen/Eigen>



namespace deform
{

	namespace
	{
		using Vector3 = Eigen::Vector3f;
		std::pair<Triangle_mesh::Point, Triangle_mesh::Point> FittingPlane(const std::vector<Triangle_mesh::Point> & c)
		{
			size_t num_atoms = c.size();
			Eigen::Matrix< Vector3::Scalar, Eigen::Dynamic, Eigen::Dynamic > coord(3, num_atoms);
			for (size_t i = 0; i < num_atoms; ++i)
			{
				coord(0, i) = c[i][0];
				coord(1, i) = c[i][1];
				coord(2, i) = c[i][2];
			}
			Vector3 centroid(coord.row(0).mean(), coord.row(1).mean(), coord.row(2).mean());
			coord.row(0).array() -= centroid(0); coord.row(1).array() -= centroid(1); coord.row(2).array() -= centroid(2);
			auto svd = coord.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
			Vector3 plane_normal = svd.matrixU().rightCols<1>();
			return std::make_pair(
				Triangle_mesh::Point(centroid(0,0), centroid(1,0), centroid(2,0)),
				Triangle_mesh::Point(plane_normal(0,0), plane_normal(1,0), plane_normal(2,0)));
		}

		float  MeanEdge(const Triangle_mesh& mesh)
		{
			float m = 0;
			int n_edge = (int)mesh.n_edges();
#pragma omp parallel for reduction(+: m)
			for (int e = 0; e < n_edge; ++e)
				m += mesh.calc_edge_length(EdgeHandle(e));

			return m / n_edge;
		}



		bool collapse_flips_normal(Triangle_mesh& mMesh, HalfedgeHandle heh)
		{
			if (!mMesh.is_collapse_ok(heh))
				return true;
			auto fh0 = mMesh.face_handle(heh);
			auto fh1 = mMesh.face_handle(mMesh.opposite_halfedge_handle(heh));
			auto collapsing_vertex = mMesh.from_vertex_handle(heh);
			auto point_before = mMesh.point(collapsing_vertex);
			std::vector<Triangle_mesh::Point> normals_before;
			for (auto fh : mMesh.vf_range(collapsing_vertex))
				normals_before.push_back(mMesh.calc_face_normal(fh));
			mMesh.point(collapsing_vertex) = mMesh.point(mMesh.to_vertex_handle(heh));
			bool collapse_ok = true;
			int i = 0;
			for (auto fh : mMesh.vf_range(collapsing_vertex))
			{
				if (fh != fh0 && fh != fh1)
				{
					auto normal_after = mMesh.calc_face_normal(fh);
					if ((normal_after | normals_before[i]) <= 0)
						collapse_ok = false;
				}
				++i;
			}
			mMesh.point(collapsing_vertex) = point_before;
			return !collapse_ok;
		}

		void RemeshBoudnaryImpl(Triangle_mesh& mesh)
		{
			mesh.request_edge_status();
			mesh.request_face_status();
			mesh.request_vertex_status();
			const float meanlength = MeanEdge(mesh);

			for (int i = 0; i < 100; ++i)
			{
				for (auto e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it)
				{
					if (!mesh.is_valid_handle(*e_it))
					{
						continue;
					}
					if (!mesh.is_boundary(*e_it)) {
						continue;
					}

					auto h = mesh.halfedge_handle(*e_it, 0);
					if (mesh.is_boundary(h))
						h = mesh.opposite_halfedge_handle(h);

					auto v0 = mesh.from_vertex_handle(h);
					auto v1 = mesh.to_vertex_handle(h);
					auto v2 = mesh.to_vertex_handle(mesh.next_halfedge_handle(h));
					auto fh = mesh.face_handle(h);
					if (mesh.is_boundary(v0) && mesh.is_boundary(v1) && mesh.is_boundary(v2))
					{
						mesh.delete_edge(*e_it);
						continue;
					}
					float elen = mesh.calc_edge_length(*e_it);
					if (elen < meanlength)
					{
						if (!collapse_flips_normal(mesh, h))
						{
							mesh.collapse(h);
						}
						continue;
					}
					else if (mesh.valence(v0) ==3) {
						if (!collapse_flips_normal(mesh, h))
						{
							mesh.collapse(h);
						}
						continue;
					}
					else if (elen > 2.0f*meanlength)
					{
						mesh.split(*e_it, (mesh.point(v0) + mesh.point(v1))*0.5f);
						continue;
					}
				}
				mesh.garbage_collection();

			}

			for (int i = 0; i < 100; ++i)
			{
				for (auto v = mesh.vertices_begin(), vend = mesh.vertices_end(); v != vend; ++v)
				{
					if (!mesh.is_valid_handle(*v))
						continue;
					if (mesh.is_isolated(*v))
						continue;
					if (mesh.is_boundary(*v))
						continue;

					if (mesh.valence(*v) == 3)
					{
						for (auto voh = mesh.voh_begin(*v), vohend = mesh.voh_end(*v); voh != vohend; ++voh)
						{
							if (!collapse_flips_normal(mesh, *voh))
							{
								mesh.collapse(*voh);
								break;
							}
						}
						continue;
					}
				}
				mesh.garbage_collection();
			}


			mesh.release_edge_status();
			mesh.release_face_status();
			mesh.release_vertex_status();
		}


		bool InCircumsphere(
			const Point & x,
			const Point & a,
			const Point & b,
			const Point & c)
		{
			Point ab = b - a;
			Point ac = c - a;

			double a00 = -2.0f * (ab | a);
			double a01 = -2.0f * (ab | b);
			double a02 = -2.0f * (ab | c);
			double b0 = a.sqrnorm() - b.sqrnorm();

			double a10 = -2.0f * (ac | a);
			double a11 = -2.0f * (ac | b);
			double a12 = -2.0f * (ac | c);
			double b1 = a.sqrnorm() - c.sqrnorm();

			double alpha = -(-a11 * a02 + a01 * a12 - a12 * b0 + b1 * a02 + a11 * b0 - a01 * b1)
				/ (-a11 * a00 + a11 * a02 - a10 * a02 + a00 * a12 + a01 * a10 - a01 * a12);
			double beta = (a10*b0 - a10 * a02 - a12 * b0 + a00 * a12 + b1 * a02 - a00 * b1)
				/ (-a11 * a00 + a11 * a02 - a10 * a02 + a00 * a12 + a01 * a10 - a01 * a12);
			double gamma = (-a11 * a00 - a10 * b0 + a00 * b1 + a11 * b0 + a01 * a10 - a01 * b1)
				/ (-a11 * a00 + a11 * a02 - a10 * a02 + a00 * a12 + a01 * a10 - a01 * a12);

			Point center = alpha * a + beta * b + gamma * c;

			return (x - center).sqrnorm() < (a - center).sqrnorm();
		}

		bool edge_flip_flips_normal(const Triangle_mesh& mesh, EdgeHandle eh)
		{
			if (mesh.is_boundary(eh))
				return true;
			if (!mesh.is_flip_ok(eh))
				return true;
			auto heh = mesh.halfedge_handle(eh, 0);
			auto p0 = mesh.point(mesh.to_vertex_handle(heh));
			auto p1 = mesh.point(mesh.to_vertex_handle(mesh.next_halfedge_handle(heh)));
			auto p2 = mesh.point(mesh.from_vertex_handle(heh));
			auto p3 = mesh.point(mesh.to_vertex_handle(mesh.next_halfedge_handle(mesh.opposite_halfedge_handle(heh))));
			auto n0_before = (p1 - p0) % (p2 - p0);
			auto n1_before = (p2 - p0) % (p3 - p0);
			auto n0_after = (p1 - p0) % (p3 - p0);
			auto n1_after = (p3 - p2) % (p1 - p2);
			if ((n0_before | n0_after) < 0 || (n1_before | n1_after) < 0 ||
				(n0_before | n1_after) < 0 || (n1_before | n0_after) < 0)
				return true;
			return false;
		}

		void RelaxEdges(Triangle_mesh& mesh)
		{
			bool ok = false;
			for (int i = 0; (!ok) && i < 100; ++i)
			{
				ok = true;
				auto e_end = mesh.edges_end();
				for (auto e_it = mesh.edges_begin(); e_it != e_end; ++e_it)
				{
					// Abbreviations for the two halfedges of _eh
					HalfedgeHandle h0 = mesh.halfedge_handle(*e_it, 0);
					HalfedgeHandle h1 = mesh.halfedge_handle(*e_it, 1);
					// Get the two end-vertices u and v of the edge
					Point u(mesh.point(mesh.to_vertex_handle(h0)));
					Point v(mesh.point(mesh.to_vertex_handle(h1)));
					// Get the two opposing vertices a and b
					Point a(mesh.point(mesh.to_vertex_handle(mesh.next_halfedge_handle(h0))));

					Point b(mesh.point(mesh.to_vertex_handle(mesh.next_halfedge_handle(h1))));

					// If the circumsphere criterion is fullfilled AND if the flip is
					// topologically admissible, we do it.

					if (InCircumsphere(a, u, v, b) || InCircumsphere(b, u, v, a))
					{
						if (!edge_flip_flips_normal(mesh,*e_it))
						{
							mesh.flip(*e_it);
							ok = false;
						}
					}
				}
			}
		}

	}

	MeshSmooth::MeshSmooth(Triangle_mesh& mesh) :
		m_mesh(mesh), tolerance(0), component(Normal), continuity(C0)
	{

	}

	MeshSmooth::~MeshSmooth()
	{

	}

	void MeshSmooth::Initialize(Component comp, Continuity cont)
	{
		component = comp;
		continuity = cont;
	}

	LaplaceSmooth::LaplaceSmooth(Triangle_mesh&mesh):
		MeshSmooth(mesh), lambda(0.6307f)
	{

	}

	void LaplaceSmooth::SmoothBoundary(unsigned int iter)
	{
		RemeshBoudnaryImpl(m_mesh);
		RelaxEdges(m_mesh);
		Smooth(iter);
	}

	LaplaceSmooth::~LaplaceSmooth()
	{

	}

	void LaplaceSmooth::Smooth(unsigned int iter)
	{
		
		for (unsigned i = 0; i < iter; ++i)
		{
			Umbrella(iter, lambda);
		}
	}

	void LaplaceSmooth::LocalSmooth(const std::vector<VertexHandle>&vhs, unsigned int iter)
	{
		
		for (unsigned i = 0; i < iter; ++i)
		{
			Umbrella(vhs, iter, lambda);
		}
	}

	void LaplaceSmooth::Umbrella(int iter, float scale)
	{
		const int nVertex = (int)m_mesh.n_vertices();
		for (int iv = 0; iv < nVertex; ++iv)
		{
			auto vbeg = VertexHandle(iv);
			SmoothScale(vbeg, scale);
		}
	}

	void LaplaceSmooth::Umbrella(const std::vector<VertexHandle>&vhs, int iter, float scale)
	{
		int nVhs = (int)vhs.size();
		for (int iv = 0; iv < nVhs; ++iv)
		{
			auto vbeg = vhs[iv];
			SmoothScale(vbeg, scale);
		}
	}



	void LaplaceSmooth::SmoothScale(const VertexHandle& vbeg, float scale)
	{
		if (!m_mesh.is_valid_handle(vbeg))
			return;
		else if (m_mesh.is_isolated(vbeg))
			return;
		else if (m_mesh.is_boundary(vbeg))
		{
			std::vector<HalfedgeHandle> vohs,vihs;
			std::vector<VertexHandle> vvhs;
			for (auto voh_iter = m_mesh.voh_begin(vbeg), voh_end = m_mesh.voh_end(vbeg); voh_iter != voh_end; ++voh_iter)
			{
				if (m_mesh.is_boundary(*voh_iter)) {
					vohs.push_back(*voh_iter);
					vvhs.push_back(m_mesh.to_vertex_handle(*voh_iter));
				}
			}
			for (auto vih_iter = m_mesh.vih_begin(vbeg), vih_end = m_mesh.vih_end(vbeg); vih_iter != vih_end; ++vih_iter)
			{
				if (m_mesh.is_boundary(*vih_iter)) {
					vohs.push_back(*vih_iter);
					vvhs.push_back(m_mesh.from_vertex_handle(*vih_iter));
				}
			}
			if (vvhs.size() == 2)
			{
				m_mesh.point(vbeg) = (m_mesh.point(vvhs.front()) + m_mesh.point(vbeg)* 2.0f + m_mesh.point(vvhs.back())) * 0.25f;
			}
			return;
		}
		else
		{
			std::vector<VertexHandle> vvhs;
			for (auto vvbeg = m_mesh.vv_cwbegin(vbeg), vvEd = m_mesh.vv_cwend(vbeg); vvbeg != vvEd; ++vvbeg) {
				vvhs.push_back(*vvbeg);
			}

			size_t n_count = vvhs.size();
			float w = 1.0f / float(n_count);
			Vec3f delp(0, 0, 0);
			for (auto vh : vvhs) {
				delp += w * (m_mesh.point(vh) - m_mesh.point(vbeg));
			}
			Vec3f vp = m_mesh.point(vbeg) + lambda * delp;
			m_mesh.set_point(vbeg, vp);
			return;
		}
	}

	void LaplaceSmooth::SetLambda(float l) { lambda = l; }
	
	float LaplaceSmooth::ComputeLamda(int n)
	{
		const float alpha = static_cast<float>((4.0 - 2.0 *
			std::cos(2.0 * M_PI / n)) / 9.0);
		return alpha;
	}

	TaubinSmooth::TaubinSmooth(Triangle_mesh&mesh):
		LaplaceSmooth(mesh), micro(0.0424f)
	{

	}

	TaubinSmooth::~TaubinSmooth()
	{

	}

	void TaubinSmooth::Smooth(unsigned int iter)
	{
		iter = (iter + 1) / 2;
		for (unsigned int i = 0; i < iter; ++i) {
			Umbrella(iter,lambda);
			Umbrella(iter ,-(lambda + micro));
		}
	}

	void TaubinSmooth::LocalSmooth(const std::vector<VertexHandle>&vhs, unsigned int iterations)
	{
		iterations = (iterations + 1) / 2;
		for (unsigned int i = 0; i < iterations; ++i) {
			Umbrella(vhs, iterations, lambda);
			Umbrella(vhs, iterations, -(lambda + micro));
		}
	}

	void TaubinSmooth::SetMicro(float m) { micro = m; }
	
}

