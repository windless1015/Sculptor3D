#pragma once
//#include <OpenMesh/Tools/Utils/getopt.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "viewGLwidget.h"

typedef OpenMesh::TriMesh_ArrayKernelT<>  Triangle_Mesh;



class MeshViewerWidget : public ViewGLWidget
{
	Q_OBJECT

public:
	/// default constructor
	explicit MeshViewerWidget(QWidget* parent = 0);

	void setOptions(const OpenMesh::IO::Options& opts) { _options = opts; }

	void open_mesh_gui(QString fname);

private:
	Triangle_Mesh m_triMesh;

private:
	OpenMesh::IO::Options _options;
};

