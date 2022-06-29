#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "src/baseDataTypes.h"

class QMenu;

class ViewGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility
{
	Q_OBJECT
public:
	explicit ViewGLWidget(QWidget *parent = nullptr);
	~ViewGLWidget();

protected:
	void initializeGL() override;
	void resizeGL(int, int) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

public:
	virtual void fillMeshDataToArray(Triangle_mesh& inputMesh);

private:
	void initShader();
	//QOpenGLTexture *initTexture(const QString &imgpath);

private:
	QOpenGLShaderProgram m_shader;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;


	QOpenGLTexture *diffuseMap{ nullptr };
	QOpenGLTexture *specularMap{ nullptr };
	QPoint mousePos;
	QVector3D rotationAxis;
	QQuaternion rotationQuat;
	float projectionFovy{ 45.0f };
	QMenu *m_righMenu;
	int drawMode{ 0 };
	bool enableDepthTest{ false };
	bool enableCullBackFace{ false };

	QVector<QVector3D> m_meshVertxArray;
};