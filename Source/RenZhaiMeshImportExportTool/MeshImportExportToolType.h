#pragma once
#include "Core/IEPropertyObject.h"
#include "MeshImportExportToolSkinnedType.h"

//���� ���ϲ㴫��
struct FIEParam
{
	FIEParam()
		:bOriginalCoordinate(true)
	{}

	bool bOriginalCoordinate;
};

//��������
struct FIEVertex
{
	fvector_3d Position;
	fvector_3d Color;
	fvector_3d Normal;
	fvector_3d Tangent;
	fvector_2d UV;
	fvector_3d Binormal;
};

//������ȡ������һ����������
struct FIETriangle
{
	FIETriangle()
		:MaterialID(0)
	{}

	FIEVertex Vertexs[3];
	int MaterialID;
};

//ģ������
struct FIEMesh
{
	FIEMesh()
	{}
	std::vector<FIETriangle> VertexData;
	std::vector<uint16_t> IndexData;

	std::vector<uint16_t> FBXDefaultIndexData;//Ϊ��Ƥ׼��
};

//��������
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

	//�Ǽ�
	FIESkeleton Skeleton;

	//��������
	FIEAnimationData AnimationData;

	//ģ�ͱ���ı任
	fmatrix_4x4 Matrix;

	//ģ�͵�����
	std::string Name;

	//����FBX
	bool bBindPose;
	bool bLocalMatrix;
};

//��Ⱦ����
struct FIERenderData
{
	std::vector<FIEModel> ModelData;
};