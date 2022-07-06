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
	const QVector<QVector3D> *getMeshVertsArr() { return m_meshVertxArray; };
private:
	void copyVertsDataToBuff(Triangle_mesh& mesh);

private:
	QString m_meshFilePath;
private:
	Triangle_mesh m_triMesh;
	QVector<QVector3D> *m_meshVertxArray;
};