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
#include "viewGLwidget.h"
#include "infoWidget.h"
#include "toolWidget.h"
#include "src/model/meshDataModel.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), m_meshData(nullptr)
{
	ui->setupUi(this);

	this->setWindowTitle("Sculptor3D");
	this->setWindowIcon(QIcon(":/sculptor3D.png"));

	m_view3DWidget = new Viewer(this);
	m_infoWidget = new InfoWidget(this);
	m_toolWidget = new ToolWidget(this);
	initializeUI();
}

MainWindow::~MainWindow()
{
	delete ui;
	cleanMeshData();
}


void MainWindow::initializeUI()
{
	m_actionGroup = new QActionGroup(this);
	m_actionGroup->setExclusive(true);
	connect(m_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateItem(QAction*)));

	QAction *actionOpen = new QAction("OpenFile", m_actionGroup);
	actionOpen->setIcon(QIcon(":/openFolder.png"));
	ui->menuFile->addAction(actionOpen);

	QAction *actionNewMesh = new QAction("NewMesh", m_actionGroup);
	actionNewMesh->setIcon(QIcon(":/newMesh.png"));
	ui->menuFile->addAction(actionNewMesh);

	QAction *actionSave = new QAction("Save", m_actionGroup);
	actionSave->setIcon(QIcon(":/save.png"));
	ui->menuFile->addAction(actionSave);

	QAction *actionQuit = new QAction("Quit", m_actionGroup);
	actionQuit->setIcon(QIcon(":/quit.png"));
	ui->menuFile->addAction(actionQuit);

	QAction *actionAbout = new QAction("About", m_actionGroup);
	ui->menuHelp->addAction(actionAbout);

	//view group
	QAction *actionAxis = new QAction("Axis", m_actionGroup);
	actionAxis->setIcon(QIcon(":/axis.png"));
	ui->menuView->addAction(actionAxis);

	QAction *actionPlain = new QAction("Plain", m_actionGroup);
	actionPlain->setIcon(QIcon(":/plain.png"));
	ui->menuView->addAction(actionPlain);

	QAction *actionFPS = new QAction("FPS", m_actionGroup);
	actionFPS->setIcon(QIcon(":/fps.png"));
	ui->menuView->addAction(actionFPS);

	QAction *actionCornerAxis = new QAction("Corner Axis", m_actionGroup);
	actionCornerAxis->setIcon(QIcon(":/cornerAxis.png"));
	ui->menuView->addAction(actionCornerAxis);

	//QHBoxLayout* horizontalLayout = new QHBoxLayout();
	//horizontalLayout->addWidget(m_toolWidget);
	//horizontalLayout->addWidget(m_view3DWidget);// 3d widget
	//horizontalLayout->addWidget(m_infoWidget);
	////set the stretch factor of 3d widget and infowidget, 3:1, [   ][] or the display mode will be [][]
	//horizontalLayout->setStretchFactor(m_infoWidget, 1);
	//horizontalLayout->setStretchFactor(m_view3DWidget, 3);
	//horizontalLayout->setStretchFactor(m_infoWidget, 1);
	//ui->centralwidget->setLayout(horizontalLayout);

	setCentralWidget(m_view3DWidget);
	addDockWidget(Qt::LeftDockWidgetArea, m_toolWidget);
	addDockWidget(Qt::RightDockWidgetArea, m_infoWidget);
}

void MainWindow::cleanMeshData()
{
	if (m_meshData)
	{
		delete m_meshData;
		m_meshData = nullptr;
	}
}

void MainWindow::updateItem(QAction *action)
{
	if (action->text() == "OpenFile") 
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			("Open File"), "D:/",
			"Image Files (*.stl *.obj)");

		cleanMeshData();
		m_meshData = new MeshDataModel();
		m_meshData->readMesh(fileName); // fill the mesh data model
		m_view3DWidget->setMeshDataModel(m_meshData);// view set the model data pointer

	}
	if (action->text() == "NewMesh") {
		//generate a new sphere

	}
	if (action->text() == "Save") {
		QMessageBox::information(NULL, "Title", "Save",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	if (action->text() == "Quit") {
		QApplication* app;
		app->exit(0);
	}
	if (action->text() == "About") {
		QString text = QString("Sculptor3D 1.0.0" " - ") + QObject::tr("a simple 3D modelling application") +
			QString("\n\n") + QString("Copyright @ 2020-2022 Alex") + QString("\n\n") +
			QObject::tr("Use and redistribute under the terms of the GNU General Public License");
		QMessageBox::about(this, QObject::tr("About Sculptor3D"), text);
	}
	if (action->text() == "Axis") {
		m_view3DWidget->toggleAxisIsDrawn();
	}
	if (action->text() == "Plain") {
		m_view3DWidget->toggleGridIsDrawn();
	}
	if (action->text() == "FPS") {
		m_view3DWidget->toggleFPSIsDisplayed();
	}
	if (action->text() == "Corner Axis") {
		m_view3DWidget->toggleCornerAxisDrawn();
	}
}