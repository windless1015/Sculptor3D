#include <QVector>
#include <QVector3D>
#include "meshDataModel.h"



MeshDataModel::MeshDataModel() : m_meshVertxArray(nullptr),
	m_meshFilePath("")
{
	m_meshVertxArray = new QVector<QVector3D>();
}

MeshDataModel::~MeshDataModel()
{
	if (m_meshVertxArray) 
	{
		delete m_meshVertxArray;
	}
}


void MeshDataModel::readMesh(const QString& fname)
{
	m_triMesh.request_vertex_normals();
	m_triMesh.request_face_normals();
	if (!m_triMesh.has_vertex_normals() || !m_triMesh.has_face_normals())
	{
		return;
	}
	OpenMesh::IO::Options opt;

	if (OpenMesh::IO::read_mesh(m_triMesh, fname.toStdString(), opt))
	{
		copyVertsDataToBuff(m_triMesh);
		if (!opt.check(OpenMesh::IO::Options::VertexNormal))
		{
			m_triMesh.request_face_normals();
			m_triMesh.update_normals();
			m_triMesh.release_face_normals();
		}
		m_meshFilePath = fname;
	}
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
			Point p = mesh.point(*fv_ccw_it);
			QVector3D v3(p[0], p[1], p[2]);
			m_meshVertxArray->push_back(v3);
			index++;
		}
	}
}
