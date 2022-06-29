#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

#include <QGridLayout>
#include "ui_mainwindow.h"
#include "mainWindow.h"
#include "meshViewerWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->setWindowTitle("Sculptor3D");
	this->setWindowIcon(QIcon(":/sculptor3D.png"));

	w = new MeshViewerWidget(this);
	initializeUI();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initializeUI()
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addWidget(w);
	ui->centralwidget->setLayout(layout);

	//w.open_mesh_gui("D:/skull.stl");
	w->open_mesh_gui("D:/Sphere.stl");
	//w.open_mesh_gui("D:/Icosahedron.stl");
	//w.open_mesh_gui("D:/cube.stl");
	w->show();
}