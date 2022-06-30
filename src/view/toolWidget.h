#pragma once
#include <QDockWidget>

namespace Ui {
	class ToolWidget;
}

class ToolWidget : public QDockWidget
{
	Q_OBJECT
public:
	ToolWidget(QWidget *parent = nullptr);
	~ToolWidget();
private:
	Ui::ToolWidget *ui;
};