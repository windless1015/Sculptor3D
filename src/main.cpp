#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include "view\mainWindow.h"

int main (int argc, char** argv)
{
	QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication app(argc, argv);

	QSurfaceFormat fmt;
	fmt.setRenderableType(QSurfaceFormat::OpenGL);
	fmt.setVersion(4, 5);
	fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(fmt);

	MainWindow w;
	w.resize(1500, 800);
	w.show();
	return app.exec();
}
