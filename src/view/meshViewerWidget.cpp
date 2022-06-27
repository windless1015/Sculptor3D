#include "meshViewerWidget.h"


//using namespace OpenMesh;
//using namespace OpenMesh::Attributes;

MeshViewerWidget::MeshViewerWidget(QWidget* parent)
{

}

void MeshViewerWidget::open_mesh_gui(QString fname)
{

	//m_triMesh.request_face_normals();
	//m_triMesh.request_face_colors();
	//m_triMesh.request_vertex_normals();
	//m_triMesh.request_vertex_colors();
	//m_triMesh.request_vertex_texcoords2D();

	//std::cout << "Loading from file '" << _filename << "'\n";
	//OpenMesh::IO::read_mesh(m_triMesh, fname);
	if(OpenMesh::IO::read_mesh(m_triMesh, fname.toStdString()))
	{
		int numFace = m_triMesh.n_faces();
		int numVert = m_triMesh.n_vertices();
		drawMesh(m_triMesh);
	}


	
}