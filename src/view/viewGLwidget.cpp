#include <QCursor>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QtMath>
#include "viewGLwidget.h"

ViewGLWidget::ViewGLWidget(QWidget *parent) : 
	QOpenGLWidget(parent)
{
	rotationQuat = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 90.0f);

	//右键菜单
	m_righMenu = new QMenu(this);
	m_righMenu->addAction("Toggle depth test", [this] {
		enableDepthTest = !enableDepthTest;
		update();
	});
	m_righMenu->addAction("Toggle cull backface", [this] {
		enableCullBackFace = !enableCullBackFace;
		update();
	});
	m_righMenu->addAction("Set Fill Mode", [this] {
		drawMode = 0;
		update();
	});
	m_righMenu->addAction("Set Line Mode", [this] {
		drawMode = 1;
		update();
	});
	m_righMenu->addAction("Set Point Mode", [this] {
		drawMode = 2;
		update();
	});

}

ViewGLWidget::~ViewGLWidget()
{
	if (!isValid())
		return;
	makeCurrent();
	m_vao.destroy();
	m_vbo.destroy();
	doneCurrent();
}

void ViewGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	initShader();


	//m_meshVertxArray.clear();
	//float a, x, y;
	//const float out_r = 1.0f;//外圆半径
	//const float in_r = 0.5f;//内圆半径
	//const float section_r = (out_r - in_r) / 2;//截面圆环半径
	//const int section_slice = 24;//截面圆边定点数
	//const int torus_slice = 24;//整体圆环截面数
	//						   //1.先求一个截面的顶点做基准
	//QVector<QVector3D> section_vec;
	//for (int i = 0; i<section_slice; i++)
	//{
	//	a = qDegreesToRadians(360.0f / section_slice*i);
	//	x = section_r*cos(a);
	//	y = section_r*sin(a);
	//	QVector3D vec(x, y, 0);
	//	section_vec.push_back(vec);
	//}
	//////2.再对截面定点进行坐标变换，形成一个环面的顶点
	//////3.对顶点进行组织，使之能围成三角
	//QMatrix4x4 cur_mat;//当前截面的变换矩阵
	//QMatrix4x4 next_mat;//下一个截面的变换矩阵
	//next_mat.setToIdentity();
	//next_mat.translate(out_r - section_r, 0, 0);
	//for (int i = 0; i<torus_slice; i++)
	//{
	//	cur_mat = next_mat;
	//	next_mat.setToIdentity();
	//	next_mat.rotate(360.0f / torus_slice*(i + 1), 0, 1.0f, 0);
	//	next_mat.translate(out_r - section_r, 0, 0);
	//	for (int j = 0; j<section_vec.size(); j++)
	//	{
	//		m_meshVertxArray.push_back(cur_mat * section_vec.at(j));
	//		m_meshVertxArray.push_back(next_mat * section_vec.at(j));
	//	}
	//	m_meshVertxArray.push_back(m_meshVertxArray.at(m_meshVertxArray.size() - section_vec.size() * 2));
	//	m_meshVertxArray.push_back(m_meshVertxArray.at(m_meshVertxArray.size() - section_vec.size() * 2));
	//}

	/*m_vao.create();
	m_vao.bind();
	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate((void *)m_meshVertxArray.data(), sizeof(GLfloat) * m_meshVertxArray.size() * 3);
	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
	m_shader.enableAttributeArray(0);
	m_vbo.release();
	m_vao.release();*/










	m_vao.release();
	m_vbo.release();

	m_vao.create();
	m_vao.bind();
	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate((void *)m_meshVertxArray.data(), sizeof(GLfloat) * m_meshVertxArray.size() * 3);
	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
	m_shader.enableAttributeArray(0);
	m_vbo.release();
	m_vao.release();

	//set the background color of the opengl window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}


static QVector3D cubePositions = {
	QVector3D(0.0f,  0.0f,  0.0f),
};

void ViewGLWidget::paintGL()
{
	if (enableDepthTest) {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
	}
	if (enableCullBackFace) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
	if (drawMode == 0) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (drawMode == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(4.0f);
	}

	m_shader.bind();
	//观察矩阵
	QMatrix4x4 view;
	float radius = 3.0f;
	view.translate(0.0f, 0.0f, -radius);
	view.rotate(rotationQuat);
	//透视投影
	QMatrix4x4 projection;
	projection.perspective(projectionFovy, 1.0f * width() / height(), 0.1f, 100.0f);
	m_shader.setUniformValue("mvp", projection * view);
	if (!m_meshVertxArray.isEmpty())
	{
		QOpenGLVertexArrayObject::Binder vao_bind(&m_vao); Q_UNUSED(vao_bind);
		//使用当前激活的着色器和顶点属性配置和VBO（通过VAO间接绑定）来绘制图元
		//void glDrawArrays(GLenum mode​, GLint first​, GLsizei count​);
		//参数1为图元类型
		//参数2指定顶点数组的起始索引
		//参数3指定顶点个数
		//GL_TRIANGLE_STRIP三角形带
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_meshVertxArray.size());
	}
	m_shader.release();

	////设置为fill，不然会影响QPainter绘制的图
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//QPainter painter(this);
	//painter.setPen(Qt::white);
	//painter.setFont(QFont("Microsoft YaHei", 14));
	//painter.drawText(20, 40, "Click right mouse button popup menu");
}

void ViewGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void ViewGLWidget::mousePressEvent(QMouseEvent *event)
{
	event->accept();
	if (event->button() == Qt::RightButton) {
		m_righMenu->popup(QCursor::pos());
	}
	else {
		mousePos = event->pos();
	}
}
void ViewGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	event->accept();
}

void ViewGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	event->accept();
	//参照示例cube
	QVector2D diff = QVector2D(event->pos()) - QVector2D(mousePos);
	mousePos = event->pos();
	QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
	rotationAxis = (rotationAxis + n).normalized();
	//不能对换乘的顺序
	rotationQuat = QQuaternion::fromAxisAndAngle(rotationAxis, 3.0f) * rotationQuat;

	update();
}

void ViewGLWidget::wheelEvent(QWheelEvent *event)
{
	event->accept();
	//fovy越小，模型看起来越大
	if (event->delta() < 0) {
		//鼠标向下滑动为-，这里作为zoom out
		projectionFovy += 5.5f;
		if (projectionFovy > 90)
			projectionFovy = 90;
	}
	else {
		//鼠标向上滑动为+，这里作为zoom in
		projectionFovy -= 5.5f;
		if (projectionFovy < 1)
			projectionFovy = 1;
	}
	update();
}










void ViewGLWidget::initShader()
{
	//uniform from cpu to gpu
	if (!m_shader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Vertex, ":/shader/sphere.vert")) {
		qDebug() << "compiler vertex error" << m_shader.log();
	}
	if (!m_shader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Fragment, ":/shader/sphere.frag")) {
		qDebug() << "compiler fragment error" << m_shader.log();
	}

	if (!m_shader.link()) {
		qDebug() << "link shaderprogram error" << m_shader.log();
	}

}

//QOpenGLTexture *ViewGLWidget::initTexture(const QString &imgpath)
//{
//	QOpenGLTexture *texture = new QOpenGLTexture(QImage(imgpath), QOpenGLTexture::GenerateMipMaps);
//	if (!texture->isCreated()) {
//		qDebug() << "Failed to create texture";
//	}
//	//set the texture wrapping parameters
//	//equals glTexParameteri(GLtexture_2D, GLtexture_WRAP_S, GL_REPEAT);
//	texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
//	texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//
//	//set texture filtering parameters
//	//等价于glTexParameteri(GLtexture_2D, GLtexture_MIN_FILTER, GL_LINEAR);
//	texture->setMinificationFilter(QOpenGLTexture::Linear);
//	texture->setMagnificationFilter(QOpenGLTexture::Linear);
//	return texture;
//}


void ViewGLWidget::drawMesh(const Triangle_Mesh& inputMesh)
{
	m_meshVertxArray.clear();
	auto v_end = inputMesh.vertices_end();
	//traverse all the verts
	for (auto v_it = inputMesh.vertices_begin(); v_it != v_end; ++v_it)
	{
		VertexHandle vh = *v_it;
		//p为顶点坐标
		Point3f p = inputMesh.point(vh);
		QVector3D v3(p[0], p[1], p[2]);
		m_meshVertxArray.push_back(v3);
	}

}