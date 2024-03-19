#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Mesh/Core/MeshType.h"

class CMeshComponent;
enum ERenderingMeshType;
struct FRenderingDataSection :public FMeshSection
{
	FRenderingDataSection();

	UINT VertexOffsetPosition;//�ֲ�ƫ��
	UINT IndexOffsetPosition;//Indexƫ��
	int MeshObjectIndex;//Index
};

//�ṩ��Ⱦ���ݵĽӿ�
struct FRenderingData :public IDirectXDeviceInterfece_Struct, public std::enable_shared_from_this<FRenderingData>
{
public:
	FRenderingData();

public:
	std::vector<FRenderingDataSection> Sections;
public:
	int SkinnedMeshObjectIndex;//Index

	UINT IndexTotalSize;
	UINT VertexTotalxSize;

	UINT VertexOffsetPosition;//������ƫ��
	UINT VertexTypeSize;//�������͵Ĵ�С
	UINT IndexTypeSize;//Index���ʹ�С
public:
	BoundingBox Bounds;//AABB

	size_t MeshHash;
	int GeometryKey;

public:
	ERenderingMeshType GetMeshType() const;
	FVertexMeshData* GetMeshRenderingData();
	FSkinnedVertexMeshData* GetSkinnedMeshRenderingData();

public:
	UINT GetVertexSizeInBytes()const { return VertexTotalxSize * VertexTypeSize; }
	UINT GetIndexSizeInBytes()const { return IndexTotalSize * IndexTypeSize; }
public:
	DXGI_FORMAT IndexFormat;

	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 TextureTransform;

	CMeshComponent* Mesh;//key

	//�п�������Ƥ�п����Ǿ�̬����
	void *MeshRenderingData;
};