#include "SkinnedMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/SkinnedMeshComponent.h"

GSkinnedMesh::GSkinnedMesh()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetMeshComponent(ConstructionObject<CSkinnedMeshComponent>(Param));
}

void GSkinnedMesh::Init()
{
	Super::Init();

}

void GSkinnedMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GSkinnedMesh::CreateMesh(const string& InPath, const FIEParam& InParam)
{
	CREATE_SKINNED_RENDER_DATA(CSkinnedMeshComponent, InPath, InParam);
}