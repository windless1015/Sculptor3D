#include "viewGLwidget.h"

ViewGLWidget::ViewGLWidget(QWidget *parent) : 
	QOpenGLWidget(parent)
{
	/*connect(&m_timer, &QTimer::timeout, this, [this]() 
	{
		rotate += 1;
		if (isVisible()) {
			update();
		}
	});
	m_timer.setInterval(50);*/
}

ViewGLWidget::~ViewGLWidget()
{
	/*if (!isValid())
		return;*/
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

	//纹理
	//diffuseMap = initTexture(":/container2.png");
	diffuseMap = initTexture("D:/projects/Sculptor3D/Sculptor3D/resources/container2.png");
	specularMap = initTexture("D:/projects/Sculptor3D/Sculptor3D/resources/container2_specular.png");
	//shader configuration
	m_lightingShader.bind();
	m_lightingShader.setUniformValue("material.diffuse", 0);
	m_lightingShader.setUniformValue("material.specular", 1);
	m_lightingShader.release();

	//m_timer.start();
}

//绘制多个盒子
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
	QMatrix4x4 projection; //透视投影
	projection.perspective(45.0f, 1.0f * width() / height(), 0.1f, 100.0f);
	m_lightingShader.setUniformValue("projection", projection);
	QMatrix4x4 model;//模型矩阵
					 //lightingShader.setUniformValue("model", model);
					 //因为要获取灯的位置，所以提前算灯的model矩阵
	model = QMatrix4x4();
	model.translate(QVector3D(0.0f, 2.0f, 0.0f));
	model.scale(0.2f);
	QVector3D light_pos = model.map(QVector3D(0.0f, 0.0f, 0.0f));
	//QVector3D direction_pos = QVector3D(0.0f, -10.0f, 0.0f);
	QMatrix4x4 vv = view.inverted(); //逆矩阵求观察点位置
	QVector3D view_pos = vv.map(QVector3D(0.0f, 0.0f, 0.0f));
	m_lightingShader.setUniformValue("light.position", light_pos);
	m_lightingShader.setUniformValue("light.constant", 1.0f); //常数项
	m_lightingShader.setUniformValue("light.linear", 0.09f); //一次项
	m_lightingShader.setUniformValue("light.quadratic", 0.032f); //二次项
	m_lightingShader.setUniformValue("viewPos", view_pos);
	//光照-light properties
	QVector3D light_color = QVector3D(1.0f, 1.0f, 1.0f);
	QVector3D diffuse_color = light_color * 0.5f; // decrease the influence
	QVector3D ambient_color = diffuse_color * 0.2f; // low influence
	m_lightingShader.setUniformValue("light.ambient", ambient_color);
	m_lightingShader.setUniformValue("light.diffuse", diffuse_color);
	m_lightingShader.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

	//材质-material properties
	//shininess影响镜面高光的散射/半径
	m_lightingShader.setUniformValue("material.shininess", 64.0f);
	m_lightingVao.bind();
	//绑定2d纹理
	//bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	diffuseMap->bind();
	//bind specular map
	glActiveTexture(GL_TEXTURE1);
	specularMap->bind();
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//多个盒子便于对比
	for (unsigned int i = 0; i < 10; i++) {
		//模型矩阵
		QMatrix4x4 box_model;
		//平移
		box_model.translate(cubePositions[i]);
		float angle = 20.0f * i;
		//旋转
		box_model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
		//传入着色器并绘制
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
	//lingting shader
	//in输入，out输出,uniform从cpu向gpu发送
	const char *lighting_vertex = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
})";
	const char *lighting_fragment = R"(#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear*distance +light.quadratic*(distance*distance));

    vec3 result = (ambient + diffuse + specular)*attenuation;
    FragColor = vec4(result, 1.0);
})";

	//将source编译为指定类型的着色器，并添加到此着色器程序
	if (!m_lightingShader.addCacheableShaderFromSourceCode(
		QOpenGLShader::Vertex, lighting_vertex)) {
		qDebug() << "compiler vertex error" << m_lightingShader.log();
	}
	if (!m_lightingShader.addCacheableShaderFromSourceCode(
		QOpenGLShader::Fragment, lighting_fragment)) {
		qDebug() << "compiler fragment error" << m_lightingShader.log();
	}
	//使用addShader()将添加到该程序的着色器链接在一起。
	if (!m_lightingShader.link()) {
		qDebug() << "link shaderprogram error" << m_lightingShader.log();
	}

	//lamp shader
	const char *lamp_vertex = R"(#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
gl_Position = projection * view * model * vec4(aPos, 1.0f);
})";
	const char *lamp_fragment = R"(#version 330 core
out vec4 FragColor;
void main()
{
FragColor = vec4(1.0);
})"; // set alle 4 vector values to 1.0

	if (!m_lampShader.addCacheableShaderFromSourceCode(
		QOpenGLShader::Vertex, lamp_vertex)) {
		qDebug() << "compiler vertex error" << m_lampShader.log();
	}
	if (!m_lampShader.addCacheableShaderFromSourceCode(
		QOpenGLShader::Fragment, lamp_fragment)) {
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
	//等于glTexParameteri(GLtexture_2D, GLtexture_WRAP_S, GL_REPEAT);
	texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
	texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//
																			 //set texture filtering parameters
																			 //等价于glTexParameteri(GLtexture_2D, GLtexture_MIN_FILTER, GL_LINEAR);
	texture->setMinificationFilter(QOpenGLTexture::Linear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	return texture;
}
