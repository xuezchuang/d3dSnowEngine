#pragma once
#include "MeshConstruction.h"
#include "../../Mesh/Core/Mesh.h"

namespace ActorMeshConstruction
{
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderData(CMeshManage* InManage,GMesh *InGMesh, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T, FVertex>(InManage, InGMesh->GetMeshComponent<T>(), Params...);
	}

	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderDataByComponent(CMeshManage* InManage, T* InMeshComponent, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T, FVertex>(InManage, InMeshComponent, Params...);
	}

	template<class T, typename ...ParamTypes>
	T* CreateSkinnedMeshRenderData(CMeshManage* InManage, GMesh* InGMesh, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T, FSkinnedVertex>(InManage, InGMesh->GetMeshComponent<T>(), Params...);
	}

	template<class T, typename ...ParamTypes>
	T* CreateSkinnedMeshRenderDataByComponent(CMeshManage* InManage, T* InMeshComponent, ParamTypes &&...Params)
	{
		return MeshConstruction::CreateMeshComponent<T, FSkinnedVertex>(InManage, InMeshComponent, Params...);
	}
}
