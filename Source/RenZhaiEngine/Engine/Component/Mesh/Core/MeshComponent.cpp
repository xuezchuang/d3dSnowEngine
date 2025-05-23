#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"
#include "../../../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../../../Core/Construction/MacroConstruction.h"

CMeshComponent::CMeshComponent()
{
	MeshRenderLayerType = EMeshRenderLayerType::RENDERLAYER_OPAQUE;

	bCastShadow = true;
	bPickup = true;
	bVisible = true;
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType)
{
	MeshRenderLayerType = InRenderLayerType;
}

void CMeshComponent::SpawnDefaultMaterial()
{
	BUILD_OBJECT_PARAMETERS_BY_COMPONENT(, this);
	Materials.push_back(CreateObject<CMaterial>(Param, new CMaterial()));
}

void CMeshComponent::GetBoundingBox(BoundingBox& OutBoundingBox)
{
	FGeometry::FindRenderingDatas(
	[&](std::shared_ptr<FRenderingData>& InRenderingData)->EFindValueType
	{
		if (InRenderingData->Mesh == this)
		{
			OutBoundingBox = InRenderingData->Bounds;

			return EFindValueType::TYPE_COMPLETE;
		}

		return EFindValueType::TYPE_IN_PROGRAM;
	});
}

BoundingBox CMeshComponent::GetBoundingBox()
{
	BoundingBox AABB;
	GetBoundingBox(AABB);

	return AABB;
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();
}

bool CMeshComponent::IsDynamicReflection() const
{
	if (Materials.size() >= 1)
	{
		return 
			Materials[0]->IsDynamicReflection() && 
			MeshRenderLayerType == EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR;
	}

	return false;
}
