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
#include <QMessageBox>
#include "viewGLwidget.h"
#include "src/model/meshDataModel.h"

// Constructor must call the base class constructor.
Viewer::Viewer(QWidget *parent) : QGLViewer(parent) {
	restoreStateFromFile();
	m_meshDataPtr = nullptr;
	m_isDrawnCornerAxis = true;
	newFlag = false;
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

	/*initShader();

	m_vao.create();
	m_vao.bind();
	m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbo.create();
	m_vbo.bind();

	m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat)* 3);
	m_shader.enableAttributeArray(0);
	m_vbo.release();
	m_vao.release();*/

	//generate sphere

	iniGL();

	//set font size is 50
	m_textFont.setPointSize(50);

	setAxisIsDrawn(true);
	setFPSIsDisplayed(true);
	setGridIsDrawn(true);
	//setBackgroundColor(QColor(220, 215, 211));
	setTextIsEnabled(true); //open text display


	glLineWidth(3.0);
	glPointSize(10.0);
}

void Viewer::drawWithNames()
{
	glPushName(0);
	drawSphere();
	glPopName();
}

void Viewer::postSelection(const QPoint &point) {
	// Compute orig and dir, used to draw a representation of the intersecting
	// line
	camera()->convertClickToLine(point, orig, dir);

	// Find the selectedPoint coordinates, using camera()->pointUnderPixel().
	bool found;
	selectedPoint = camera()->pointUnderPixel(point, found);
	selectedPoint -= 0.01f * dir; // Small offset to make point clearly visible.
								  // Note that "found" is different from (selectedObjectId()>=0) because of the
								  // size of the select region.
	if (selectedName() == -1)
		QMessageBox::information(this, "No selection",
			"No object selected under pixel " +
			QString::number(point.x()) + "," +
			QString::number(point.y()));
	else {
		QMessageBox::information(
			this, "Selection",
			"Spiral number " + QString::number(selectedName()) +
			" selected under pixel " + QString::number(point.x()) + "," +
			QString::number(point.y()));
			qDebug() << "select point: " << selectedPoint.x << ", " << selectedPoint.y << ", " << selectedPoint.z;
	}
}

void Viewer::draw() 
{
	if (enableDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (enableCullBackFace) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (drawMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (drawMode == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (drawMode == 2)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(4.0f);
	}
	drawText(50, 50, "FDU",m_textFont);

	if(newFlag)
		drawSphere();
	if(m_meshDataPtr)
		m_meshDataPtr->draw(0);//0,solid smooth  1,wire frame  2,solid flat  3,points
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

	//const QVector<QVector3D>* vertsArr = m_meshDataPtr->getMeshVertsArr();
	//updateVBOBuffer(*vertsArr);
	//update(); // make it effective and opengl will call paintgl
}

void Viewer::toggleCornerAxisDrawn()
{
	m_isDrawnCornerAxis = !m_isDrawnCornerAxis;
}
void Viewer::toggleDepthTest()
{
	enableDepthTest = !enableDepthTest;
}

void Viewer::toggleCullBackFace()
{
	enableCullBackFace = !enableCullBackFace;
}
void Viewer::toggleFillMode()
{
	drawMode = 0;
}

void Viewer::toggleLineMode()
{
	drawMode = 1;
}

void Viewer::togglePointMode()
{
	drawMode = 2;
}

void Viewer::newSphere()
{
	sphereShape.set(1.0f, 36, 18);
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


void Viewer::drawSphere()
{
	// clear buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// tramsform modelview matrix
	//glTranslatef(0, 0, -cameraDistance);

	// set material
	float ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
	float diffuse[] = { 0.7f, 0.7f, 0.7f, 1 };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1 };
	float shininess = 128;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	// line color
	float lineColor[] = { 0.2f, 0.2f, 0.2f, 1 };


	//glBindTexture(GL_TEXTURE_2D, 0);
	//sphere2.drawWithLines(lineColor);

	glBindTexture(GL_TEXTURE_2D, sphereShape.returnTexureId());
	sphereShape.draw();

}

void Viewer::iniGL()
{
	glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

	// enable /disable features
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);                   // background color
	glClearStencil(0);                          // clear stencil buffer
	glClearDepth(1.0f);                         // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);




	/******************************************/
	/* this website can be used to check the light effect and position of light
	http://www.cs.toronto.edu/~jacobson/phong-demo/
	reference: http://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/slides/08-shading.pdf
	*/
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { 0.1f, 0.2f, 0.3f, 1.0f };  // ambient light
	GLfloat lightKd[] = { .5f, .5f, .5f, 1.0f };  // diffuse light
	GLfloat lightKs[] = { 0.8f, 0.9f, 1.0f, 1.0f };           // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = { -2.4, 1.9, 0.4, 0 }; // directional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration


	sphereShape.loadTexture("D:/projects/Sculptor3D/Sculptor3D/resources/earth.bmp", true);
	//sphere2.loadTexture("D:/projects/Sculptor3D/Sculptor3D/resources/leather.bmp", true);
	//sphere2.loadTexture("D:/projects/Sculptor3D/Sculptor3D/resources/moon.bmp", true);
	//sphere2.loadTexture("earth.bmp", true);
}