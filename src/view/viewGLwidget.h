#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>






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
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLShaderProgram m_shaderProgram;

};