﻿#include "infoWidget.h"
#include "ui_infowidget.h"
#include <QScrollArea>


InfoWidget::InfoWidget(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::InfoWidget)
{
	ui->setupUi(this);

	QScrollArea* scrollArea = new QScrollArea;
	//QTabWidget*  tabWidget = new QTabWidget;

	//tabWidget->addTab(this->initializeToolTipTab(), QObject::tr("Keys"));
	//tabWidget->addTab(&this->scene, QObject::tr("Scene"));
	scrollArea->setWidgetResizable(true);
	//scrollArea->setWidget(tabWidget);

	this->setWindowTitle("Info");
	this->setWidget(scrollArea);
	this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);


}

InfoWidget::~InfoWidget()
{
	delete ui;
}

