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
	void draw(int renderMode);
	bool set_texture(QImage& _texsrc);
private:
	void copyVertsDataToBuff(Triangle_mesh& mesh);
	//void copyVertsDataToBuff(const float* vertices, unsigned int vertsCount);

private:
	QString m_meshFilePath;
private:
	Triangle_mesh m_triMesh;
	OpenMesh::IO::Options m_meshOpt; //properties of mesh
	GLuint                 m_texId;
	GLint                  tex_mode_;
	std::vector<Point> *m_meshVertxArray;
	Triangle_mesh::ConstFaceIter  fIt = m_triMesh.faces_begin();
	Triangle_mesh::ConstFaceIter fEnd = m_triMesh.faces_end();
	Triangle_mesh::ConstFaceVertexIter fvIt;

};