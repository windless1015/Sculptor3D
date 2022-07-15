//#include <QCursor>
//#include <QMouseEvent>
//#include <QWheelEvent>
//#include <QMenu>
//#include <QPainter>
//#include <QtMath>
//#include "viewGLwidget.h"
//#include "src/model/meshDataModel.h"
//
//ViewGLWidget::ViewGLWidget(QWidget *parent) : 
//	QOpenGLWidget(parent), m_meshDataInGLWidget(nullptr)
//{
//
//	rotationQuat = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 30.0f);
//	rotationQuat *= QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -10.0f);
//	//右键菜单
//	m_righMenu = new QMenu(this);
//	m_righMenu->addAction("Toggle depth test", [this] {
//		enableDepthTest = !enableDepthTest;
//		update();
//	});
//	m_righMenu->addAction("Toggle cull backface", [this] {
//		enableCullBackFace = !enableCullBackFace;
//		update();
//	});
//	m_righMenu->addAction("Set Fill Mode", [this] {
//		drawMode = 0;
//		update();
//	});
//	m_righMenu->addAction("Set Line Mode", [this] {
//		drawMode = 1;
//		update();
//	});
//	m_righMenu->addAction("Set Point Mode", [this] {
//		drawMode = 2;
//		update();
//	});
//
//}
//
//ViewGLWidget::~ViewGLWidget()
//{
//	if (!isValid())
//		return;
//	makeCurrent();
//	m_vao.destroy();
//	m_vbo.destroy();
//	doneCurrent();
//}
//
//void ViewGLWidget::initializeGL()
//{
//	initializeOpenGLFunctions();
//	//set the background color of the opengl window
//	//glClearColor(0.5f, 0.54f, 0.527f, 1.0f);
//	float rgb[3] = { 220,220,220};
//	glClearColor(rgb[0] / 256.0f, rgb[1] / 256.0f, rgb[2] / 256.0f, 1.0f);
//
//	initShader();
//
//	m_vao.create();
//	m_vao.bind();
//	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//	m_vbo.create();
//	m_vbo.bind();
//
//	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat)* 3);
//	m_shader.enableAttributeArray(0);
//	m_vbo.release();
//	m_vao.release();
//
//	
//}
//
//
//
//void ViewGLWidget::paintGL()
//{
//	if (!m_meshDataInGLWidget || m_meshDataInGLWidget->getMeshVertsArr()->isEmpty())
//	{
//		return;
//	}
//
//	if (enableDepthTest) {
//		glEnable(GL_DEPTH_TEST);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//	else {
//		glClear(GL_COLOR_BUFFER_BIT);
//	}
//	if (enableCullBackFace) {
//		glEnable(GL_CULL_FACE);
//	}
//	else {
//		glDisable(GL_CULL_FACE);
//	}
//	if (drawMode == 0) {
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	}
//	else if (drawMode == 1)  //line mode
//	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	}
//	else {
//		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//		glPointSize(4.0f);
//	}
//
//	QOpenGLVertexArrayObject::Binder vao_bind(&m_vao); 
//	Q_UNUSED(vao_bind); //this equals to m_vao.bind(); and m_vao.release();
//	m_shader.bind();
//
//	//观察矩阵
//	QMatrix4x4 view;
//	view.translate(0.0f, 0.0f, -5.0f);
//	view.rotate(rotationQuat);
//	//透视投影
//	QMatrix4x4 projection;
//	projection.perspective(projectionFactor, 1.0f * width() / height(), 0.1f, 100.0f);
//	//模型矩阵
//	QMatrix4x4 model;
//
//	//m_shader.setUniformValue("mvp", projection * viewMatrix);
//	m_shader.setUniformValue("mvp", projection * view * model);
//
//
//	//m_meshVertxArray stores three points of every face, so GL_TRIANGLES will be used because they display the single triangle
//	const QVector<QVector3D>* vertsArr = m_meshDataInGLWidget->getMeshVertsArr();
//	glDrawArrays(GL_TRIANGLES, 0, vertsArr->size());
//	m_shader.release();
//
//}
//
//void ViewGLWidget::resizeGL(int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//
//void ViewGLWidget::mousePressEvent(QMouseEvent *event)
//{
//	event->accept();
//	if (event->button() == Qt::RightButton) {
//		m_righMenu->popup(QCursor::pos());
//	}
//	else {
//		mousePos = event->pos();
//	}
//}
//void ViewGLWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//	event->accept();
//}
//
//void ViewGLWidget::mouseMoveEvent(QMouseEvent *event)
//{
//	event->accept();
//	//参照示例cube
//	QVector2D diff = QVector2D(event->pos()) - QVector2D(mousePos);
//	mousePos = event->pos();
//	QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
//	rotationAxis = (rotationAxis + n).normalized();
//	
//	rotationQuat = QQuaternion::fromAxisAndAngle(rotationAxis, 2.0f) * rotationQuat;
//
//	update();
//}
//
//void ViewGLWidget::wheelEvent(QWheelEvent *event)
//{
//	event->accept();
//	if (event->delta() < 0) {
//		projectionFactor += 2.0f;
//		if (projectionFactor > 500)
//			projectionFactor = 500;
//	}
//	else {
//		projectionFactor -= 2.0f;
//		if (projectionFactor < 1)
//			projectionFactor = 1;
//	}
//	update();
//}
//
//
//
//
//void ViewGLWidget::initShader()
//{
//	static const char *shader_vertex = R"(#version 450 core
//layout (location = 0) in vec3 inPos;
//uniform mat4 mvp;
//void main()
//{
//gl_Position = mvp * vec4(inPos, 1.0);
//})";
//
//static const char *shader_fragment = R"(#version 450 core
//out vec4 fragColor;
//void main()
//{
//fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f); //set the color with Blue
//})";
//
//	if (!m_shader.addCacheableShaderFromSourceCode(
//		QOpenGLShader::Vertex, shader_vertex)) {
//		qDebug() << "compiler vertex error" << m_shader.log();
//	}
//	if (!m_shader.addCacheableShaderFromSourceCode(
//		QOpenGLShader::Fragment, shader_fragment)) {
//		qDebug() << "compiler fragment error" << m_shader.log();
//	}
//
//	if (!m_shader.link()) {
//		qDebug() << "link shaderprogram error" << m_shader.log();
//	}
//
//
//	//uniform from cpu to gpu
//	/*if (!m_shader.addCacheableShaderFromSourceFile(
//		QOpenGLShader::Vertex, ":/shader/sphere.vert")) {
//		qDebug() << "compiler vertex error" << m_shader.log();
//	}
//	if (!m_shader.addCacheableShaderFromSourceFile(
//		QOpenGLShader::Fragment, ":/shader/sphere.frag")) {
//		qDebug() << "compiler fragment error" << m_shader.log();
//	}
//
//	if (!m_shader.link()) {
//		qDebug() << "link shaderprogram error" << m_shader.log();
//	}*/
//
//}
//
//
//
//
//
////QOpenGLTexture *ViewGLWidget::initTexture(const QString &imgpath)
////{
////	QOpenGLTexture *texture = new QOpenGLTexture(QImage(imgpath), QOpenGLTexture::GenerateMipMaps);
////	if (!texture->isCreated()) {
////		qDebug() << "Failed to create texture";
////	}
////	//set the texture wrapping parameters
////	//equals glTexParameteri(GLtexture_2D, GLtexture_WRAP_S, GL_REPEAT);
////	texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
////	texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//
////	//set texture filtering parameters
////	//等价于glTexParameteri(GLtexture_2D, GLtexture_MIN_FILTER, GL_LINEAR);
////	texture->setMinificationFilter(QOpenGLTexture::Linear);
////	texture->setMagnificationFilter(QOpenGLTexture::Linear);
////	return texture;
////}
//
//
////void ViewGLWidget::fillMeshDataToArray(Triangle_mesh& inputMesh)
////{
////	m_meshVertxArray.clear();
////	//traverse each face of the mesh
////	for (Triangle_mesh::FaceIter f_it = inputMesh.faces_begin(); f_it != inputMesh.faces_end(); ++f_it)
////	{
////		FaceHandle fh = *f_it;
////		//traverse three points on this face, counterclock size
////		/*for (Triangle_mesh::FVIter fv_it = inputMesh.fv_begin(fh); fv_it != inputMesh.fv_end(fh); ++fv_it)
////		{
////			Point p = inputMesh.point(*fv_it);
////			QVector3D v3(p[0], p[1], p[2]);
////			m_meshVertxArray.push_back(v3);
////			qDebug() << p[0] << "," << p[1] << "," << p[2];
////		}*/
////
////		int index = 0;
////		auto fv_ccw_it = inputMesh.fv_ccwiter(fh);
////		for (; fv_ccw_it.is_valid(); ++fv_ccw_it)
////		{
////			//counterclock traverse the face
////			Point p = inputMesh.point(*fv_ccw_it);
////			QVector3D v3(p[0], p[1], p[2]);
////			m_meshVertxArray.push_back(v3);
////			index++;
////		}
////
////	}
////
////	//update vbo data
////	m_vbo.bind();
////	m_vbo.allocate((void *)m_meshVertxArray.data(), sizeof(GLfloat) * m_meshVertxArray.size() * 3);
////	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
////	m_shader.enableAttributeArray(0);
////	m_vbo.release();
////
////	update();
////}
//
//void ViewGLWidget::setMeshDataModel(MeshDataModel* mesh) 
//{ 
//	m_meshDataInGLWidget = mesh;
//	const QVector<QVector3D>* vertsArr = m_meshDataInGLWidget->getMeshVertsArr();
//	updateVBOBuffer(*vertsArr);
//	
//	update(); // make it effective and opengl will call paintgl
//}
//
//
//void ViewGLWidget::updateVBOBuffer(const QVector<QVector3D>& vertsArray)
//{
//	//update vbo data
//	m_vbo.bind();
//	m_vbo.allocate((void *)vertsArray.data(), sizeof(GLfloat) * vertsArray.size() * 3);
//	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
//	m_shader.enableAttributeArray(0);
//	m_vbo.release();
//}

#include <QMouseEvent>
#include <QMenu>
#include "viewGLwidget.h"
#include "src/model/meshDataModel.h"

// Constructor must call the base class constructor.
Viewer::Viewer(QWidget *parent) : QGLViewer(parent) {
	restoreStateFromFile();
	m_meshDataPtr = nullptr;
	m_isDrawnCornerAxis = true;


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

Viewer::~Viewer()
{
	if (!isValid())
		return;
	makeCurrent();
	m_vao.destroy();
	m_vbo.destroy();
	doneCurrent();
}

void Viewer::init()
{
	initializeOpenGLFunctions();

	initShader();

	m_vao.create();
	m_vao.bind();
	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbo.create();
	m_vbo.bind();

	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat)* 3);
	m_shader.enableAttributeArray(0);
	m_vbo.release();
	m_vao.release();


	// Increase the material shininess, so that the difference between
	// the two versions of the spiral is more visible.
	/*glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
	GLfloat specular_color[4] = { 0.8f, 0.8f, 0.8f, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);*/

	setAxisIsDrawn(true);
	setFPSIsDisplayed(true);
	setGridIsDrawn(true);
}



void Viewer::draw() 
{
	if (!m_meshDataPtr || m_meshDataPtr->getMeshVertsArr()->isEmpty())
	{
		return;
	}

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
	else if (drawMode == 1)  //line mode
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(4.0f);
	}


	QOpenGLVertexArrayObject::Binder vao_bind(&m_vao); 
	Q_UNUSED(vao_bind); //this equals to m_vao.bind(); and m_vao.release();
	//m_shader.bind();
	
	//////view matrix

	/*GLfloat mvp[16];
	this->camera()->getModelViewProjectionMatrix(mvp);
	QMatrix4x4 MVPMatrix(mvp);
	MVPMatrix.normalMatrix();*/

	//view.translate(0.0f, 0.0f, -5.0f);
	////view.rotate(rotationQuat);
	////project matrix
	//QMatrix4x4 projection;
	//projection.perspective(45.0f, 1.0f * width() / height(), 0.1f, 100.0f);
	////model matrix
	//QMatrix4x4 model;
	////

	//m_shader.setUniformValue("mvp", MVPMatrix);
	
	
	//m_meshVertxArray stores three points of every face, so GL_TRIANGLES will be used because they display the single triangle
	const QVector<QVector3D>* vertsArr = m_meshDataPtr->getMeshVertsArr();
	glDrawArrays(GL_TRIANGLES, 0, vertsArr->size());
	//m_shader.release();
}

void Viewer::updateVBOBuffer(const QVector<QVector3D>& vertsArray)
{
	//update vbo data
	m_vbo.bind();
	m_vbo.allocate((void *)vertsArray.data(), sizeof(GLfloat) * vertsArray.size() * 3);
	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
	m_shader.enableAttributeArray(0);
	m_vbo.release();
}

void Viewer::setMeshDataModel(MeshDataModel* mesh)
{ 
	m_meshDataPtr = mesh;
	const QVector<QVector3D>* vertsArr = m_meshDataPtr->getMeshVertsArr();
	updateVBOBuffer(*vertsArr);
	
	update(); // make it effective and opengl will call paintgl
}

void Viewer::toggleCornerAxisDrawn()
{
	m_isDrawnCornerAxis = !m_isDrawnCornerAxis;
}

void Viewer::initShader()
{
	static const char *shader_vertex = R"(#version 450 core
layout (location = 0) in vec3 inPos;
uniform mat4 mvp;
void main()
{
gl_Position = mvp * vec4(inPos, 1.0);
})";

static const char *shader_fragment = R"(#version 450 core
out vec4 fragColor;
void main()
{
fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f); //set the color with Blue
})";

	if (!m_shader.addCacheableShaderFromSourceCode(
		QOpenGLShader::Vertex, shader_vertex)) {
		qDebug() << "compiler vertex error" << m_shader.log();
	}
	if (!m_shader.addCacheableShaderFromSourceCode(
		QOpenGLShader::Fragment, shader_fragment)) {
		qDebug() << "compiler fragment error" << m_shader.log();
	}

	if (!m_shader.link()) {
		qDebug() << "link shaderprogram error" << m_shader.log();
	}


	//uniform from cpu to gpu
	/*if (!m_shader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Vertex, ":/shader/sphere.vert")) {
		qDebug() << "compiler vertex error" << m_shader.log();
	}
	if (!m_shader.addCacheableShaderFromSourceFile(
		QOpenGLShader::Fragment, ":/shader/sphere.frag")) {
		qDebug() << "compiler fragment error" << m_shader.log();
	}

	if (!m_shader.link()) {
		qDebug() << "link shaderprogram error" << m_shader.log();
	}*/

}

void Viewer::mousePressEvent(QMouseEvent *event)
{
	event->accept();
	if (event->button() == Qt::RightButton) {
		m_righMenu->popup(QCursor::pos());
	}
	//remember to call parent mousePressEvent function
	QGLViewer::mousePressEvent(event);
}

// The thumbnail has to be drawn at the very end to allow a correct
// display of the visual hints (axes, grid, etc).
void Viewer::postDraw() {
	QGLViewer::postDraw();
	if(m_isDrawnCornerAxis)
		drawCornerAxis();
}

void Viewer::drawCornerAxis()
{
	int viewport[4];
	int scissor[4];

	// The viewport and the scissor are changed to fit the lower left
	// corner. Original values are saved.
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetIntegerv(GL_SCISSOR_BOX, scissor);

	// Axis viewport size, in pixels
	const int size = 150;
	glViewport(0, 0, size, size);
	glScissor(0, 0, size, size);

	// The Z-buffer is cleared to make the axis appear over the
	// original image.
	glClear(GL_DEPTH_BUFFER_BIT);

	// Tune for best line rendering
	glDisable(GL_LIGHTING);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(camera()->orientation().inverse().matrix());

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	// The viewport and the scissor are restored.
	glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}