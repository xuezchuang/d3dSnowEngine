#pragma once
#include "../../EngineMinimal.h"
#include "MeshImportExportToolSkinnedType.h"

class CSkeleton;
struct FBoneAnimation :public FIEBoneAnimation
{
public:
	void ProcessBoneAnimation(float InAnimationTime, CSkeleton* InSkeleton,XMFLOAT4X4& OutUnhandledBoneTransforms);
	void ProcessBoneAnimation(const FIEKey &InKey,XMFLOAT4X4& OutUnhandledBoneTransforms);
	void ProcessBoneAnimation(float InAnimationTime, XMFLOAT4X4& OutUnhandledBoneTransforms);
public:
	float GetTimeLength();
};