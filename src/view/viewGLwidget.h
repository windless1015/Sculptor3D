#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QTimer>





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

private:
	void initShader();
	QOpenGLTexture *initTexture(const QString &imgpath);

private:
	QOpenGLShaderProgram m_lightingShader, m_lampShader;
	QOpenGLVertexArrayObject m_lightingVao, m_lampVao;

	QOpenGLBuffer m_vbo;
	QOpenGLTexture *diffuseMap{ nullptr };
	QOpenGLTexture *specularMap{ nullptr };
	QTimer m_timer;
	int rotate{ 0 };

};