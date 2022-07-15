#include "toolWidget.h"
#include "ui_toolwidget.h"
#include <QScrollArea>
#include <QLayout>
#include <QPushButton>

ToolWidget::ToolWidget(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::toolWidget)
{
	ui->setupUi(this);

	//QScrollArea* scrollArea = new QScrollArea;
	//QTabWidget*  tabWidget = new QTabWidget;

	//scrollArea->setWidgetResizable(true);
	//scrollArea->setWidget(tabWidget);

	this->setWindowTitle("Tool");
	//this->setWidget(scrollArea);
	this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	this->setMinimumWidth(150);
	this->setMaximumWidth(250);
	//addButtonsUI();

}

ToolWidget::~ToolWidget()
{
	delete ui;
}

void ToolWidget::addButtonsUI()
{
	QPushButton *btnDraw = new QPushButton("Draw");
	QPushButton *btnSmooth = new QPushButton("Smooth");
	QVBoxLayout *vLayout = new QVBoxLayout(this);
	vLayout->addWidget(btnDraw);
	vLayout->addWidget(btnSmooth);
	this->setLayout(vLayout);
	btnDraw->show();
}