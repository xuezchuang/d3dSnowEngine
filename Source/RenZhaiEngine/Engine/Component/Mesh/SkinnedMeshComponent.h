#pragma once
#include "Core/MeshComponent.h"
#include "SkinnedMeshComponent.CodeReflection.h"

struct FIEParam;
class CSkinnedAnimationInstance;
struct FSkinnedTransformation;

//模型读取类型
class CSkinnedMeshComponent :public CMeshComponent
{
	CODEREFLECTION()

	CSkinnedAnimationInstance* SkinnedAnimationInstance;
public:
	CSkinnedMeshComponent();

	void CreateMesh(FSkinnedMeshRenderingData& MeshData, const string& InPath,const FIEParam& InParam);

	virtual ERenderingMeshType GetMeshType() const { return ERenderingMeshType::SKINNED_MESH_TYPE; }

	void TickAnimation(float InDeltaTime,FSkinnedTransformation &OutSkinnedTransformation);
public:
	CSkinnedAnimationInstance* GetAnimationInstance()const { return SkinnedAnimationInstance; }

public:
	void BuildKey(size_t& OutHashKey, const std::string& InPath,const FIEParam& InParam);
};