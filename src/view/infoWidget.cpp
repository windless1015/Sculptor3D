#include "infoWidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::InfoWidget)
{
	ui->setupUi(this);

	
}

InfoWidget::~InfoWidget()
{
	delete ui;
}

