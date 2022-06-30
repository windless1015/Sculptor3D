#include "toolWidget.h"
#include "ui_toolwidget.h"
#include <QScrollArea>

ToolWidget::ToolWidget(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::ToolWidget)
{
	ui->setupUi(this);

	QScrollArea* scrollArea = new QScrollArea;
	//QTabWidget*  tabWidget = new QTabWidget;

	//tabWidget->addTab(this->initializeToolTipTab(), QObject::tr("Keys"));
	//tabWidget->addTab(&this->scene, QObject::tr("Scene"));
	scrollArea->setWidgetResizable(true);
	//scrollArea->setWidget(tabWidget);

	this->setWindowTitle("Tool");
	this->setWidget(scrollArea);
	this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}

ToolWidget::~ToolWidget()
{
	delete ui;
}
