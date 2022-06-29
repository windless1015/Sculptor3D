#pragma once
#include <QDockWidget>

namespace Ui {
	class InfoWidget;
}

class InfoWidget : public QDockWidget
{
	Q_OBJECT
public:
	InfoWidget(QWidget *parent = nullptr);
	~InfoWidget();
private:
	Ui::InfoWidget *ui;
};