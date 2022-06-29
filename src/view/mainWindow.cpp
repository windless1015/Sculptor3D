#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QGridLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>

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
	m_actionGroup = new QActionGroup(this);
	m_actionGroup->setExclusive(true);
	connect(m_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateItem(QAction*)));

	QAction *actionOpen = new QAction("OpenFile", m_actionGroup);
	ui->menuFile->addAction(actionOpen);

	QAction *actionNewMesh = new QAction("NewMesh", m_actionGroup);
	ui->menuFile->addAction(actionNewMesh);


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


	//m_meshView3DWidget->displayMesh("D:/Sphere.stl");


}


void MainWindow::updateItem(QAction *action)
{
	if (action->text() == "OpenFile") 
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			("Open File"), "D:/",
			"Image Files (*.stl *.obj)");
		m_meshView3DWidget->displayMesh(fileName);

	}
	if (action->text() == "NewMesh") {
		QMessageBox::information(NULL, "Title", "NewMesh",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}