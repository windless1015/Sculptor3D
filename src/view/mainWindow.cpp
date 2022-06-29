#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <QGridLayout>
#include "ui_mainwindow.h"
#include "mainWindow.h"
#include "meshViewerWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	initializeUI();


	//this->setWindowTitle("My Window");
	//this->setMinimumSize(500, 400);
	//this->setWindowFlag(Qt::Widget);


	//QMenuBar *menu_bar = new QMenuBar(this);
	//this->setMenuBar(menu_bar);
	//QToolBar *tool_bar = new QToolBar(this);                  
	//this->addToolBar(tool_bar);
	//QStatusBar *status_bar = new QStatusBar(this);
	//this->setStatusBar(status_bar);

	//QMenu *file_menu = new QMenu("File(&F)", menu_bar);
	//QMenu *edit_menu = new QMenu("Edit(&E)", menu_bar);
	//QMenu *help_menu = new QMenu("Help(&H)", menu_bar);


	//QAction *new_action = new QAction("New(&N)");
	//QAction *open_action = new QAction("Open(&O)");
	//QAction *save_action = new QAction("Save(&S)");


	//new_action->setStatusTip("Open a new file");
	///*open_action->setStatusTip("open");
	//save_action->setStatusTip("Save");*/

	//file_menu->addAction(new_action);
	//file_menu->addAction(open_action);
	//file_menu->addSeparator();
	//file_menu->addAction(save_action);

	//edit_menu->addAction("Cut(&T)");
	//help_menu->addAction("About(&A)");

	//menu_bar->addMenu(file_menu);
	//menu_bar->addMenu(edit_menu);
	//menu_bar->addMenu(help_menu);

	//tool_bar->addAction(new_action);
	//tool_bar->addAction(open_action);
	//tool_bar->addAction(save_action);



	QSurfaceFormat fmt;
	fmt.setRenderableType(QSurfaceFormat::OpenGL);
	fmt.setVersion(4, 5);
	fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(fmt);


	

	

}

MainWindow::~MainWindow()
{
	//delete ui;
}

void MainWindow::initializeUI()
{

	w = new MeshViewerWidget(this);
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addWidget(w);
	ui->centralwidget->setLayout(layout);

	//w.open_mesh_gui("D:/skull.stl");
	w->open_mesh_gui("D:/Sphere.stl");
	//w.open_mesh_gui("D:/Icosahedron.stl");
	//w.open_mesh_gui("D:/cube.stl");
	w->show();
}