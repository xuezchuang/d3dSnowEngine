#pragma once
#include "Core/MeshComponent.h"
#include "CustomMeshComponent.CodeReflection.h"

//模型读取类型
enum EMeshLoadAssetType
{
	MESH_OBJ,
	MESH_FBX,
};

struct FIEParam;

class CCustomMeshComponent :public CMeshComponent
{
	CODEREFLECTION()
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData,const string& InPath,const FIEParam& InParam);

	bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData);

	bool LoadObj(const string& InBuff, FMeshRenderingData& MeshData);

	void BuildKey(size_t& OutHashKey, const std::string& InPath, const FIEParam& InParam);
};
