#include "viewGLwidget.h"

ViewGLWidget::ViewGLWidget(QWidget *parent) : 
	QOpenGLWidget(parent)
{
}

ViewGLWidget::~ViewGLWidget()
{
	makeCurrent();
	m_vbo.destroy();
	m_lightingVao.destroy();
	m_lampVao.destroy();
	delete diffuseMap;
	delete specularMap;
	doneCurrent();
}

void ViewGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	initShader();

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbo.create();

	//light vao
	m_lightingVao.create();
	m_lightingVao.bind();
	m_vbo.bind();
	m_vbo.allocate(vertices, sizeof(vertices));
	//setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0)
	m_lightingShader.setAttributeBuffer(0, GL_FLOAT, sizeof(GLfloat) * 0, 3, sizeof(GLfloat) * 8);
	m_lightingShader.enableAttributeArray(0);
	m_lightingShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
	m_lightingShader.enableAttributeArray(1);
	m_lightingShader.setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
	m_lightingShader.enableAttributeArray(2);
	m_vbo.release();
	m_lightingVao.release();

	//lamp vao
	m_lampVao.create();
	m_lampVao.bind();
	m_vbo.bind();
	//setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0)
	m_lampShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
	m_lampShader.enableAttributeArray(0);
	m_vbo.release();
	m_lampVao.release();

	//����
	diffuseMap = initTexture(":/container2.png");
	specularMap = initTexture(":/container2_specular.png");
	//shader configuration
	m_lightingShader.bind();
	m_lightingShader.setUniformValue("material.diffuse", 0);
	m_lightingShader.setUniformValue("material.specular", 1);
	m_lightingShader.release();

	//m_timer.start();
}


static QVector3D cubePositions[] = {
	QVector3D(0.0f,  0.0f,  0.0f),
	QVector3D(0.0f, -4.0f,  0.0f),
	QVector3D(0.0f,  4.0f,  0.0f),
	QVector3D(1.0f, -5.0f, 1.0f),
	QVector3D(-1.5f, -2.2f, -2.5f),
	QVector3D(-3.8f, -2.0f, -7.3f),
	QVector3D(2.4f, -0.4f, -3.5f),
	QVector3D(-1.7f, -3.0f, -6.5f),
	QVector3D(5.3f, -2.0f, -2.5f),
	QVector3D(-1.3f,  1.0f, -1.5f)
};

void ViewGLWidget::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//draw lighting
	m_lightingShader.bind();
	QMatrix4x4 view;
	view.translate(0.0f, 0.0f, -10.0f);
	view.rotate(45, QVector3D(1.0f, 0.8f, 0.0f));
	m_lightingShader.setUniformValue("view", view);
	QMatrix4x4 projection; //͸��ͶӰ
	projection.perspective(45.0f, 1.0f * width() / height(), 0.1f, 100.0f);
	m_lightingShader.setUniformValue("projection", projection);
	QMatrix4x4 model;//ģ�;���
					 //lightingShader.setUniformValue("model", model);
					 //��ΪҪ��ȡ�Ƶ�λ�ã�������ǰ��Ƶ�model����
	model = QMatrix4x4();
	model.translate(QVector3D(0.0f, 2.0f, 0.0f));
	model.scale(0.2f);
	QVector3D light_pos = model.map(QVector3D(0.0f, 0.0f, 0.0f));
	//QVector3D direction_pos = QVector3D(0.0f, -10.0f, 0.0f);
	QMatrix4x4 vv = view.inverted(); //�������۲��λ��
	QVector3D view_pos = vv.map(QVector3D(0.0f, 0.0f, 0.0f));
	m_lightingShader.setUniformValue("light.position", light_pos);
	m_lightingShader.setUniformValue("light.constant", 1.0f); //������
	m_lightingShader.setUniformValue("light.linear", 0.09f); //һ����
	m_lightingShader.setUniformValue("light.quadratic", 0.032f); //������
	m_lightingShader.setUniformValue("viewPos", view_pos);
	//����-light properties
	QVector3D light_color = QVector3D(1.0f, 1.0f, 1.0f);
	QVector3D diffuse_color = light_color * 0.5f; // decrease the influence
	QVector3D ambient_color = diffuse_color * 0.2f; // low influence
	m_lightingShader.setUniformValue("light.ambient", ambient_color);
	m_lightingShader.setUniformValue("light.diffuse", diffuse_color);
	m_lightingShader.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

	//����-material properties
	//shininessӰ�쾵��߹��ɢ��/�뾶
	m_lightingShader.setUniformValue("material.shininess", 64.0f);
	m_lightingVao.bind();
	//��2d����
	//bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	diffuseMap->bind();
	//bind specular map
	glActiveTexture(GL_TEXTURE1);
	specularMap->bind();
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//������ӱ��ڶԱ�
	for (unsigned int i = 0; i < 10; i++) {
		//ģ�;���
		QMatrix4x4 box_model;
		//ƽ��
		box_model.translate(cubePositions[i]);
		float angle = 20.0f * i;
		//��ת
		box_model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
		//������ɫ��������
		m_lightingShader.setUniformValue("model", box_model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	m_lightingVao.release();
	m_lightingShader.release();

	//draw lamp
	m_lampShader.bind();
	m_lampShader.setUniformValue("view", view);
	m_lampShader.setUniformValue("projection", projection);
	m_lampShader.setUniformValue("model", model);
	m_lampVao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_lampVao.release();
	m_lampShader.release();
}

void ViewGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}



void ViewGLWidget::initShader()
{
	//uniform from cpu to gpu
	if (!m_lightingShader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Vertex, ":/shader/lightShader.vert")) {
		qDebug() << "compiler vertex error" << m_lightingShader.log();
	}
	if (!m_lightingShader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Fragment, ":/shader/lightShader.frag")) {
		qDebug() << "compiler fragment error" << m_lightingShader.log();
	}

	if (!m_lightingShader.link()) {
		qDebug() << "link shaderprogram error" << m_lightingShader.log();
	}

	if (!m_lampShader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Vertex, ":/shader/lampShader.vert")) {
		qDebug() << "compiler vertex error" << m_lampShader.log();
	}
	if (!m_lampShader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Fragment, ":/shader/lampShader.frag")) {
		qDebug() << "compiler fragment error" << m_lampShader.log();
	}

	if (!m_lampShader.link()) {
		qDebug() << "link shaderprogram error" << m_lampShader.log();
	}
}

QOpenGLTexture *ViewGLWidget::initTexture(const QString &imgpath)
{
	QOpenGLTexture *texture = new QOpenGLTexture(QImage(imgpath), QOpenGLTexture::GenerateMipMaps);
	if (!texture->isCreated()) {
		qDebug() << "Failed to create texture";
	}
	//set the texture wrapping parameters
	//equals glTexParameteri(GLtexture_2D, GLtexture_WRAP_S, GL_REPEAT);
	texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
	texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//
	//set texture filtering parameters
	//�ȼ���glTexParameteri(GLtexture_2D, GLtexture_MIN_FILTER, GL_LINEAR);
	texture->setMinificationFilter(QOpenGLTexture::Linear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	return texture;
}
