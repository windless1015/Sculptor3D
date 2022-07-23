#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "src/baseDataTypes.h"
#include <QGLViewer\qglviewer.h>
#include "src/utility/sphere.h"

//class QMenu;
//class MeshDataModel;
//class ViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility
//{
//	Q_OBJECT
//public:
//	explicit ViewGLWidget(QWidget *parent = nullptr);
//	~ViewGLWidget();
//
//protected:
//	void initializeGL() override;
//	void resizeGL(int, int) override;
//	void paintGL() override;
//
//	void mousePressEvent(QMouseEvent *event) override;
//	void mouseReleaseEvent(QMouseEvent *event) override;
//	void mouseMoveEvent(QMouseEvent *event) override;
//	void wheelEvent(QWheelEvent *event) override;
//
//public:
//	void setMeshDataModel(MeshDataModel* mesh);
//	void updateVBOBuffer(const QVector<QVector3D>& vertsArray);
//
//private:
//	void initShader();
//	//QOpenGLTexture *initTexture(const QString &imgpath);
//
//private:
//	QOpenGLShaderProgram m_shader;
//	QOpenGLVertexArrayObject m_vao;
//	QOpenGLBuffer m_vbo;
//
//
//	QOpenGLTexture *diffuseMap{ nullptr };
//	QOpenGLTexture *specularMap{ nullptr };
//	QPoint mousePos;
//	QVector3D rotationAxis;
//	QQuaternion rotationQuat;
//	float projectionFactor{ 45.0f };
//	QMenu *m_righMenu;
//	int drawMode{ 0 };
//	bool enableDepthTest{ false };
//	bool enableCullBackFace{ false };
//
//	MeshDataModel* m_meshDataInGLWidget;
//};


class MeshDataModel;
class QMenu;
class Viewer : public QGLViewer, protected QOpenGLFunctions_4_5_Compatibility
{
public:
	Viewer(QWidget *parent);
	~Viewer();
protected:
	virtual void init();
	virtual void postDraw();
	virtual void draw();
	virtual void postSelection(const QPoint &point);
	virtual void drawWithNames();
	void mousePressEvent(QMouseEvent *event) override;

public:
	void setMeshDataModel(MeshDataModel* mesh);
	void updateVBOBuffer(const QVector<QVector3D>& vertsArray);
	void toggleCornerAxisDrawn();
	void toggleDepthTest();
	void toggleCullBackFace();
	void toggleFillMode();
	void toggleLineMode();
	void togglePointMode();
	void newSphere();
	void setNewMeshFlag(int isNew) { newFlag = isNew; };
private:
	void initShader();
	void drawCornerAxis();
	void drawSphere();
	void iniGL();
private:
	QOpenGLShaderProgram m_shader;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;

	MeshDataModel* m_meshDataPtr;

	Sphere sphereShape;

	int drawMode{ 0 };
	bool enableDepthTest{ false };
	bool enableCullBackFace{ false };
	bool m_isDrawnCornerAxis;

	QFont m_textFont;
	bool newFlag;
private:
	qglviewer::Vec orig, dir, selectedPoint;
};