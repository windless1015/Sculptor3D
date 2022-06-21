#include "viewGLwidget.h"

ViewGLWidget::ViewGLWidget(QWidget *parent) : 
	QOpenGLWidget(parent)
{
}

ViewGLWidget::~ViewGLWidget()
{}

void ViewGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	const char *vertex_str = R"(#version 450
layout (location = 0) in vec3 vertices;
void main() {
gl_Position = vec4(vertices,1.0);
})";
	const char *fragment_str = R"(#version 450
uniform vec3 myColor;
out vec4 fragColor;
void main() {
fragColor = vec4(myColor,1.0);
})";

	bool success = m_shaderProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertex_str);
	if (!success) {
		qDebug() << "compiler vertex failed!" << m_shaderProgram.log();
	}
	//片段着色器
	success = m_shaderProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragment_str);
	if (!success) {
		qDebug() << "compiler fragment failed!" << m_shaderProgram.log();
	}
	success = m_shaderProgram.link();
	if (!success) {
		qDebug() << "link shader failed!" << m_shaderProgram.log();
	}

	//三角形的三个顶点
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f, // right
		0.0f,  0.5f, 0.0f  // top
	};

	m_vao.create();
	m_vao.bind();
	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices, sizeof(vertices));
	//setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0)
	int attr = 0;
	attr = m_shaderProgram.attributeLocation("vertices");
	//setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0)
	m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
	m_shaderProgram.enableAttributeArray(attr);
	m_vbo.release();
	m_vao.release();
}

void ViewGLWidget::paintGL()
{
	//清屏设置
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_shaderProgram.bind();
	m_shaderProgram.setUniformValue("myColor", QVector3D(0.0f, 0.0f, 1.0f));
	{
		QOpenGLVertexArrayObject::Binder vao_bind(&m_vao); Q_UNUSED(vao_bind);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	m_shaderProgram.release();
}

void ViewGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}