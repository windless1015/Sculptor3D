#pragma once
//#include  <MeshProcess/OpenMeshWarpper.h>



namespace deform
{

	class MeshSmooth
	{
	public:
		using Vec3f = Triangle_mesh::Point;
		enum Component {
			Tangential,         ///< Smooth tangential direction
			Normal,             ///< Smooth normal direction
			TangentialNormal    ///< Smooth tangential and normal direction
		};
		enum Continuity {
			C0,
			C1,
			C2
		};
	public:
		MeshSmooth(Triangle_mesh& mesh);
		virtual ~MeshSmooth();
	public:
		void Initialize(Component comp, Continuity cont);
	public:
		virtual void SmoothBoundary(unsigned int) = 0;
		virtual void Smooth(unsigned int) = 0;
		virtual void LocalSmooth(const std::vector<VertexHandle>&vhs, unsigned int iter) = 0;
	protected:
		Triangle_mesh& m_mesh;
	protected:
		float tolerance;
		Component   component;
		Continuity  continuity;
	};

	class LaplaceSmooth : public MeshSmooth
	{
	public:
		LaplaceSmooth(Triangle_mesh&);
		virtual ~LaplaceSmooth();
	public:
		virtual void SmoothBoundary(unsigned int iter=5u);
		virtual void Smooth(unsigned int iter=5u);
		virtual void LocalSmooth(const std::vector<VertexHandle>&vhs,unsigned int iter=5u);
		void SetLambda(float l);
		static float ComputeLamda(int n);
	protected:
		void Umbrella(int iter,float scale);
		void Umbrella(const std::vector<VertexHandle>&vhs, int iter,float scale);
		void SmoothScale(const VertexHandle& vh,float scale);
	protected:
		float lambda;
	};

	class  TaubinSmooth : public LaplaceSmooth
	{
	public:
		TaubinSmooth(Triangle_mesh&);
	public:

		virtual ~TaubinSmooth();
		void Smooth(unsigned int iter=5u);
		void LocalSmooth(const std::vector<VertexHandle>&vhs, unsigned int iter = 5u);
		void SetMicro(float m);

	protected:
		float micro;
	};


	class CurvatureAlignSmooth :public MeshSmooth
	{
	public:

	};
}