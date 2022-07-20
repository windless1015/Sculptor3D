#pragma once
#include <QObject>
#include "src/baseDataTypes.h"

class MeshDataModel : public QObject
{
	Q_OBJECT
public:
	MeshDataModel();
	~MeshDataModel();

public:
	void MeshDataModel::readMesh(const QString& fname);
	//const QVector<QVector3D> *getMeshVertsArr() { return m_meshVertxArray; };
	const Triangle_mesh& getMesh() { return m_triMesh; } ;
private:
	//void copyVertsDataToBuff(Triangle_mesh& mesh);
	void copyVertsDataToBuff(const float* vertices, unsigned int vertsCount);

private:
	QString m_meshFilePath;
private:
	Triangle_mesh m_triMesh;
	//QVector<QVector3D> *m_meshVertxArray;

	/*std::vector<unsigned int> m_indices;
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	std::vector<float> m_texCoords;
	std::vector<unsigned int> m_indices;
	std::vector<unsigned int> m_lineIndices;
	std::vector<float> m_interleavedVertices;*/
};