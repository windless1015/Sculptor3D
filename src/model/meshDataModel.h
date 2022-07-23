#pragma once
#include <QObject>
#include "src/baseDataTypes.h"
#include <vector>

class MeshDataModel
{
public:
	MeshDataModel();
	~MeshDataModel();

public:
	void MeshDataModel::readMesh(const QString& fname);
	const std::vector<Point> *getMeshVertsArr() { return m_meshVertxArray; };
	const Triangle_mesh& getMesh() { return m_triMesh; } ;
	void draw();
	bool set_texture(QImage& _texsrc);
private:
	void copyVertsDataToBuff(Triangle_mesh& mesh);
	//void copyVertsDataToBuff(const float* vertices, unsigned int vertsCount);

private:
	QString m_meshFilePath;
private:
	Triangle_mesh m_triMesh;
	OpenMesh::IO::Options m_meshOpt; //properties of mesh
	GLuint                 tex_id_;
	GLint                  tex_mode_;
	std::vector<Point> *m_meshVertxArray;

protected:
	void glVertex(const typename Triangle_mesh::VertexHandle _vh);
	void glVertex(const typename Triangle_mesh::Point& _p);
	void glNormal(const typename Triangle_mesh::VertexHandle _vh);
	//void glTexCoord(const typename Triangle_mesh::VertexHandle _vh);
	void glColor(const typename Triangle_mesh::VertexHandle _vh);
	// face properties
	void glNormal(const typename Triangle_mesh::FaceHandle _fh);
	void glColor(const typename Triangle_mesh::FaceHandle _fh);
	void glMaterial(const typename Triangle_mesh::FaceHandle _fh,
			int _f = GL_FRONT_AND_BACK, int _m = GL_DIFFUSE);

};