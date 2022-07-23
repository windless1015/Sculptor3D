
//#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows

//#include <GL/GL.h>
//#include <gl/GLU.h>
#include <QImage>
#include <qgl.h>
#include "meshDataModel.h"

MeshDataModel::MeshDataModel()
{
	m_meshFilePath = "";
	m_meshVertxArray = new std::vector<Point>();
	tex_id_ = 0;
}

MeshDataModel::~MeshDataModel()
{
	if (m_meshVertxArray) 
	{
		delete m_meshVertxArray;
	}
}

void MeshDataModel::glVertex(const typename Triangle_mesh::VertexHandle _vh)
{
	glVertex3fv(&m_triMesh.point(_vh)[0]);
}

void MeshDataModel::glVertex(const typename Triangle_mesh::Point& _p)
{
	glVertex3fv(&_p[0]);
}

void MeshDataModel::glNormal(const typename Triangle_mesh::VertexHandle _vh)
{
	glNormal3fv(&m_triMesh.normal(_vh)[0]);
}

//void MeshDataModel::glTexCoord(const typename Triangle_mesh::VertexHandle _vh)
//{
//	glTexCoord2fv(&m_triMesh.texcoord(_vh)[0]);
//}

void MeshDataModel::glColor(const typename Triangle_mesh::VertexHandle _vh)
{
	glColor3ubv(&m_triMesh.color(_vh)[0]);
}

// face properties

void MeshDataModel::glNormal(const typename Triangle_mesh::FaceHandle _fh)
{
	glNormal3fv(&m_triMesh.normal(_fh)[0]);
}

void MeshDataModel::glColor(const typename Triangle_mesh::FaceHandle _fh)
{
	glColor3ubv(&m_triMesh.color(_fh)[0]);
}

void MeshDataModel::glMaterial(const typename Triangle_mesh::FaceHandle _fh, int _f, int _m)
{
	OpenMesh::Vec3f c = OpenMesh::color_cast<OpenMesh::Vec3f>(m_triMesh.color(_fh));
	OpenMesh::Vec4f m(c[0], c[1], c[2], 1.0f);

	glMaterialfv(_f, _m, &m[0]);
}





void MeshDataModel::readMesh(const QString& fname)
{
	m_triMesh.request_face_normals();
	m_triMesh.request_face_colors();
	m_triMesh.request_vertex_normals();
	m_triMesh.request_vertex_colors();
	m_triMesh.request_vertex_texcoords2D();
	
	bool hasVertNorms = m_triMesh.has_vertex_normals();
	bool hasFaceNorms = m_triMesh.has_face_normals();
	bool hasFaceColors = m_triMesh.has_face_colors();
	bool hasVertColors = m_triMesh.has_vertex_colors();
	bool hasFaceTexture = m_triMesh.has_vertex_texcoords2D();
	if (!hasVertNorms || !hasFaceNorms || !hasFaceColors || !hasVertColors || !hasFaceTexture)
	{
		return;
	}

	if (OpenMesh::IO::read_mesh(m_triMesh, fname.toStdString(), m_meshOpt))
	{
		//copyVertsDataToBuff(m_triMesh);
		// update face and vertex normals     
		if (!m_meshOpt.check(OpenMesh::IO::Options::FaceNormal))
			m_triMesh.update_face_normals();
		else
			std::cout << "File provides face normals\n";

		if (!m_meshOpt.check(OpenMesh::IO::Options::VertexNormal))
			m_triMesh.update_vertex_normals();
		else
			std::cout << "File provides vertex normals\n";

		// check for possible color information
		if (m_meshOpt.check(OpenMesh::IO::Options::VertexColor))
		{
			std::cout << "File provides vertex colors\n";
			//add_draw_mode("Colored Vertices");
		}
		else
			m_triMesh.release_vertex_colors();

		if (m_meshOpt.check(OpenMesh::IO::Options::FaceColor))
		{
			std::cout << "File provides face colors\n";
			/*add_draw_mode("Solid Colored Faces");
			add_draw_mode("Smooth Colored Faces");*/
		}
		else
			m_triMesh.release_face_colors();

		if (m_meshOpt.check(OpenMesh::IO::Options::VertexTexCoord))
			std::cout << "File provides texture coordinates\n";


		m_meshFilePath = fname;
	}
}

void MeshDataModel::draw()
{
	typename Triangle_mesh::ConstFaceIter    fIt(m_triMesh.faces_begin()),
		fEnd(m_triMesh.faces_end());

	typename Triangle_mesh::ConstFaceVertexIter fvIt;

	//draw mode  Solid Smooth
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_triMesh.points());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, m_triMesh.vertex_normals());

	if (tex_id_ && m_triMesh.has_vertex_texcoords2D())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, m_triMesh.texcoords2D());
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_id_);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_mode_);
	}

	glBegin(GL_TRIANGLES);
	for (; fIt != fEnd; ++fIt)
	{
		fvIt = m_triMesh.cfv_iter(*fIt);
		glArrayElement(fvIt->idx());
		++fvIt;
		glArrayElement(fvIt->idx());
		++fvIt;
		glArrayElement(fvIt->idx());
	}
	glEnd();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (tex_id_ && m_triMesh.has_vertex_texcoords2D())
	{
		glDisable(GL_TEXTURE_2D);
	}



}

bool MeshDataModel::set_texture(QImage& _texsrc)
{
	//if (!m_meshOpt.vertex_has_texcoord())
	//	return false;

	//{
	//	// adjust texture size: 2^k * 2^l
	//	int tex_w, w(_texsrc.width());
	//	int tex_h, h(_texsrc.height());

	//	for (tex_w = 1; tex_w <= w; tex_w <<= 1) {};
	//	for (tex_h = 1; tex_h <= h; tex_h <<= 1) {};
	//	tex_w >>= 1;
	//	tex_h >>= 1;
	//	_texsrc = _texsrc.scaled(tex_w, tex_h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	//}

	//QImage texture(QGLWidget::convertToGLFormat(_texsrc));

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//if (m_texId > 0)
	//{
	//	glDeleteTextures(1, &m_texId);
	//}
	//glGenTextures(1, &m_texId);
	//glBindTexture(GL_TEXTURE_2D, m_texId);

	//// glTexGenfv( GL_S, GL_SPHERE_MAP, 0 );
	//// glTexGenfv( GL_T, GL_SPHERE_MAP, 0 );

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D,       // target
	//	0,                   // level
	//	GL_RGBA,             // internal format
	//	texture.width(),     // width  (2^n)
	//	texture.height(),    // height (2^m)
	//	0,                   // border
	//	GL_RGBA,             // format
	//	GL_UNSIGNED_BYTE,    // type
	//	texture.bits());    // pointer to pixels

	//std::cout << "Texture loaded\n";
	return true;
}

void MeshDataModel::copyVertsDataToBuff(Triangle_mesh& mesh)
{
	m_meshVertxArray->clear();

	//traverse each face of the mesh
	for (Triangle_mesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		FaceHandle fh = *f_it;
		//traverse three points on this face, counterclock size
		/*for (Triangle_mesh::FVIter fv_it = inputMesh.fv_begin(fh); fv_it != inputMesh.fv_end(fh); ++fv_it)
		{
		Point p = inputMesh.point(*fv_it);
		QVector3D v3(p[0], p[1], p[2]);
		m_meshVertxArray.push_back(v3);
		qDebug() << p[0] << "," << p[1] << "," << p[2];
		}*/

		int index = 0;
		auto fv_ccw_it = mesh.fv_ccwiter(fh);
		for (; fv_ccw_it.is_valid(); ++fv_ccw_it)//counterclock traverse the face
		{
			Point p = mesh.point(*fv_ccw_it);//QVector3D v3(p[0], p[1], p[2]);
			m_meshVertxArray->push_back(p);
			index++;
		}
	}
}
//
//void MeshDataModel::copyVertsDataToBuff(const float* indices, unsigned int indicesSize)
//{
//	m_indices.clear();
//	memcpy(m_indices.data(), indices, indicesSize);
//}