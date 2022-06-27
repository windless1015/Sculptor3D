#include "meshViewerWidget.h"


//using namespace OpenMesh;
//using namespace OpenMesh::Attributes;

MeshViewerWidget::MeshViewerWidget(QWidget* parent)
{

}

void MeshViewerWidget::open_mesh_gui(QString fname)
{
	m_triMesh.request_vertex_normals();
	if (!m_triMesh.has_vertex_normals())
	{
		return;
	}
	OpenMesh::IO::Options opt;

	if(OpenMesh::IO::read_mesh(m_triMesh, fname.toStdString(), opt))
	{
		int numFace = m_triMesh.n_faces();
		int numVert = m_triMesh.n_vertices();
		drawMesh(m_triMesh);

		if (!opt.check(OpenMesh::IO::Options::VertexNormal))
		{
			m_triMesh.request_face_normals();
			m_triMesh.update_normals();
			m_triMesh.release_face_normals();
		}

	}


	
}