#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QGridLayout>
#include <QScrollArea>

#include "ui_mainwindow.h"
#include "mainWindow.h"
#include "meshViewerWidget.h"
#include "infoWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->setWindowTitle("Sculptor3D");
	this->setWindowIcon(QIcon(":/sculptor3D.png"));

	m_meshView3DWidget = new MeshViewerWidget(this);
	m_infoWidget = new InfoWidget(this);
	initializeUI();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initializeUI()
{
	QScrollArea* scrollArea = new QScrollArea;
	//QTabWidget*  tabWidget = new QTabWidget;

	//tabWidget->addTab(this->initializeToolTipTab(), QObject::tr("Keys"));
	//tabWidget->addTab(&this->scene, QObject::tr("Scene"));
	scrollArea->setWidgetResizable(true);
	//scrollArea->setWidget(tabWidget);


	m_infoWidget->setWindowTitle("Info");
	m_infoWidget->setWidget(scrollArea);
	m_infoWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	m_infoWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	horizontalLayout->addWidget(m_meshView3DWidget);// 3d widget
	horizontalLayout->addWidget(m_infoWidget);
	//set the stretch factor of 3d widget and infowidget, 3:1, [   ][] or the display mode will be [][]
	horizontalLayout->setStretchFactor(m_meshView3DWidget, 3);
	horizontalLayout->setStretchFactor(m_infoWidget, 1);
	ui->centralwidget->setLayout(horizontalLayout);

	//w.open_mesh_gui("D:/skull.stl");
	m_meshView3DWidget->open_mesh_gui("D:/Sphere.stl");
	//w.open_mesh_gui("D:/Icosahedron.stl");
	//w.open_mesh_gui("D:/cube.stl");
	//m_meshView3DWidget->show();
}