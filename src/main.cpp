#include <QApplication>

#include "view\mainWindow.h"
//#include "mainwindow.h"

int main (int argc, char** argv)
{
	QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication app(argc, argv);


	MainWindow w;
	w.show();
	
	return app.exec();
}
