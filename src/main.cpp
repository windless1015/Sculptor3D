#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
//#include "view\viewGLwidget.h"
#include "view\meshViewerWidget.h"


int main (int argc, char** argv)
{
	QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication app(argc, argv);

	QSurfaceFormat fmt;
	fmt.setRenderableType(QSurfaceFormat::OpenGL);
	fmt.setVersion(4, 5);
	fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(fmt);

	//ViewGLWidget w;
	MeshViewerWidget w;
	//w.open_mesh_gui("D:/skull.stl");
	w.open_mesh_gui("D:/Sphere.stl");
	//w.open_mesh_gui("D:/Icosahedron.stl");
	//w.open_mesh_gui("D:/cube.stl");
	w.show();
	return app.exec();
}
