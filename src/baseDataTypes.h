#pragma once
/* #include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyConnectivity.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<>  Triangle_Mesh;
//typedef OpenMesh::PolyMesh_ArrayKernelT<> Triangle_Mesh;

typedef OpenMesh::Vec3f  Vec3f, Point3f;

typedef OpenMesh::VertexHandle VertexHandle;

typedef OpenMesh::FaceHandle FaceHandle; */



#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
//#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> Triangle_mesh;
typedef OpenMesh::PolyMesh_ArrayKernelT<> Triangle_meshF;
typedef OpenMesh::PolyMesh_ArrayKernelT<> Polygon_mesh;

typedef Triangle_mesh::Edge     Edge;
typedef Triangle_mesh::Halfedge Halfedge;
typedef Triangle_mesh::FaceHandle     FaceHandle;
typedef Triangle_mesh::VertexHandle   VertexHandle;
typedef Triangle_mesh::EdgeHandle     EdgeHandle;
typedef Triangle_mesh::HalfedgeHandle HalfedgeHandle;
typedef Triangle_mesh::Point          Point;
typedef Triangle_mesh::Normal         Normal;


