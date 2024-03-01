#pragma once
#include "../../../Mesh/Core/MeshType.h"

struct FIEParam;
struct FIERenderData;

class CMeshComponent;

struct FMeshAssist
{
	static bool LoadFBXSkinnedMeshFromBuff(CMeshComponent* InMeshComponent, const string& InPath,FSkinnedMeshRenderingData& MeshData, const FIEParam& InParam);
	static bool LoadFBXMeshFromBuff(CMeshComponent* InMeshComponent, const string& InPath, FMeshRenderingData& MeshData, const FIEParam& InParam);

protected:
	template<class T>
	static bool LoadFBXFromBuff_Interior(
		CMeshComponent* InMeshComponent,
		const string& InPath,
		FRenderContent<T>& MeshData,
		const FIEParam& InParam, std::function<void(const FIERenderData&)> ExtendFunction);
};
