#pragma once
#include "Core/IEPropertyObject.h"
#include "MeshImportExportToolSkinnedType.h"

//参数 由上层传入
struct FIEParam
{
	FIEParam()
		:bOriginalCoordinate(true)
	{}

	bool bOriginalCoordinate;
};

//顶点数据
struct FIEVertex
{
	fvector_3d Position;
	fvector_3d Color;
	fvector_3d Normal;
	fvector_3d Tangent;
	fvector_2d UV;
	fvector_3d Binormal;
};

//我们提取的数据一定是三角形
struct FIETriangle
{
	FIETriangle()
		:MaterialID(0)
	{}

	FIEVertex Vertexs[3];
	int MaterialID;
};

//模型数据
struct FIEMesh
{
	FIEMesh()
	{}
	std::vector<FIETriangle> VertexData;
	std::vector<uint16_t> IndexData;

	std::vector<uint16_t> FBXDefaultIndexData;//为蒙皮准备
};

//材质名称
struct FIEMaterial
{
	FIEMaterial()
	{
		ADD_MEMBER_PROPERTY_POOL_IE(DiffuseColor);
		ADD_MEMBER_PROPERTY_POOL_IE(Ambient);
		ADD_MEMBER_PROPERTY_POOL_IE(Emissive);
		ADD_MEMBER_PROPERTY_POOL_IE(Specular);
		ADD_MEMBER_PROPERTY_POOL_IE(Shininess);
		ADD_MEMBER_PROPERTY_POOL_IE(Opacity);
		ADD_MEMBER_PROPERTY_POOL_IE(Reflectivity);
	}
	DEFINING_MEMBER_PROPERTY_POOL_IE

	DEFINING_MEMBER_PROPERTY_IE(double, 3, DiffuseColor);
	DEFINING_MEMBER_PROPERTY_IE(double, 3, Ambient);
	DEFINING_MEMBER_PROPERTY_IE(double, 3, Emissive);
	DEFINING_MEMBER_PROPERTY_IE(double, 3, Specular);
	DEFINING_MEMBER_PROPERTY_IE(double, 1, Shininess);
	DEFINING_MEMBER_PROPERTY_IE(double, 1, Opacity);
	DEFINING_MEMBER_PROPERTY_IE(double, 1, Reflectivity);

public:
	std::string ShadingModel;
	std::string Name;
};

//Model
struct FIEModel
{
	std::vector<FIEMesh> MeshData;
	std::map<int, FIEMaterial> MaterialMap;

	//骨架
	FIESkeleton Skeleton;

	//动画数据
	FIEAnimationData AnimationData;

	//模型本身的变换
	fmatrix_4x4 Matrix;

	//模型的名字
	std::string Name;

	//兼容FBX
	bool bBindPose;
	bool bLocalMatrix;
};

//渲染数据
struct FIERenderData
{
	std::vector<FIEModel> ModelData;
};