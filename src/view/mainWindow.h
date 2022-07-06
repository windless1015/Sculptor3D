#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ViewGLWidget;
class InfoWidget;
class ToolWidget;
class QAction;
class QActionGroup;
class MeshDataModel;
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
signals:
	void signalDisplayMesh(QString path);

private slots:
	void updateItem(QAction *action);
private:
	Ui::MainWindow *ui;
	ViewGLWidget* m_view3DWidget;
	InfoWidget* m_infoWidget;
	ToolWidget* m_toolWidget;
private:
	void initializeUI();
	void cleanMeshData();
private:
	QActionGroup* m_actionGroup;
private:
	MeshDataModel* m_meshData;
};