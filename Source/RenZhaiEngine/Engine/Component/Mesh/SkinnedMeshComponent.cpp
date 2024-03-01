#include "SkinnedMeshComponent.h"
#include "Core/MeshAssist.h"
#include "../../Animation/SkinnedAnimationInstance.h"

CSkinnedMeshComponent::CSkinnedMeshComponent()
{
	FCreateObjectParam Param;
	Param.Outer = this;
	Param.ParentComponent = this;

	SkinnedAnimationInstance = CreateObject<CSkinnedAnimationInstance>(Param, new CSkinnedAnimationInstance());//NewObject;

	MeshRenderLayerType = EMeshRenderLayerType::RENDERLAYER_SKINNED_OPAQUE;
}

void CSkinnedMeshComponent::CreateMesh(FSkinnedMeshRenderingData& MeshData, const string& InPath, const FIEParam& InParam)
{
	FMeshAssist::LoadFBXSkinnedMeshFromBuff(this, InPath, MeshData, InParam);
}

void CSkinnedMeshComponent::TickAnimation(float InDeltaTime, FSkinnedTransformation& OutSkinnedTransformation)
{
	if (SkinnedAnimationInstance)
	{
		SkinnedAnimationInstance->TickAnimation(InDeltaTime, OutSkinnedTransformation);
	}
}

void CSkinnedMeshComponent::BuildKey(size_t& OutHashKey, const std::string& InPath, const FIEParam& InParam)
{
	std::hash<string> FloatHash;

	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}
