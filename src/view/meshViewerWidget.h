#pragma once
//#include <OpenMesh/Tools/Utils/getopt.h>

#include "viewGLwidget.h"
#include "src/baseDataTypes.h"



class MeshViewerWidget : public ViewGLWidget
{
	Q_OBJECT

public:
	/// default constructor
	explicit MeshViewerWidget(QWidget* parent = 0);

	//void setOptions(const OpenMesh::IO::Options& opts) { _options = opts; }

	void open_mesh_gui(QString fname);

private:
	Triangle_Mesh m_triMesh;

private:
	//OpenMesh::IO::Options _options;
};

