#include "infoWidget.h"
#include "ui_infowidget.h"
#include <QScrollArea>
#include <QTableWidget>

InfoWidget::InfoWidget(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::InfoWidget)
{
	ui->setupUi(this);

	QScrollArea* scrollArea = new QScrollArea;
	QTabWidget*  tabWidget = new QTabWidget;

	QWidget *testWidget1 = new QWidget();
	tabWidget->addTab(testWidget1, QObject::tr("Mesh Info"));
	QWidget *testWidget2 = new QWidget();
	tabWidget->addTab(testWidget2, QObject::tr("Operation Info"));
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(tabWidget);

	this->setWindowTitle("Info");
	this->setWidget(scrollArea);
	this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	this->setMinimumWidth(150);
	this->setMaximumWidth(200);
}

InfoWidget::~InfoWidget()
{
	delete ui;
}

