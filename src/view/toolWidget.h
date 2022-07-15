#pragma once
#include <QDockWidget>

namespace Ui {
	class toolWidget;
}

class ToolWidget : public QDockWidget
{
	Q_OBJECT
public:
	ToolWidget(QWidget *parent = nullptr);
	~ToolWidget();
private:
	void addButtonsUI();
private:
	Ui::toolWidget *ui;
};