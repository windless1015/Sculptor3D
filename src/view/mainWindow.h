#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MeshViewerWidget;
class InfoWidget;
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private:
	Ui::MainWindow *ui;
	MeshViewerWidget* m_meshView3DWidget;
	InfoWidget* m_infoWidget;
private:
	void initializeUI();
};