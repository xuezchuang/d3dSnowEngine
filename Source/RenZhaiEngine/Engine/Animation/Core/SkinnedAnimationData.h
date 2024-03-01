#pragma once
#include "../../EngineMinimal.h"
#include "../Skeleton/Skeleton.h"

class CAnimationSequence;
struct FSkinnedTransformation;
struct FSkinnedAnimationData
{
	FSkinnedAnimationData();

	void TickAnimation(float InDeltaTime, FSkinnedTransformation& OutSkinnedTransformation);
	

public:
	float GetTimeLength(const std::string& InSequenceName);
	float GetCurrentAnimationTimeLength();

public:
	bool ProcessAnimation(const std::string& InSequenceName, float InAnimationTime,CSkeleton* InSkeleton, std::vector<XMFLOAT4X4>& OutUnhandledBoneTransforms);
	void ProcessBoneTransforms(const std::vector<XMFLOAT4X4> &UnhandledBoneTransforms,std::vector<XMFLOAT4X4>& OutBoneTransforms);
	void ProcessBoneTransforms(const std::vector<XMFLOAT4X4>& InBoneTransforms,FSkinnedTransformation& OutSkinnedTransformation);
public:
	std::string CurrentAnimationSequence;

	//骨架
	CSkeleton* Skeleton;

	//当前的动画Transform
	std::vector<XMFLOAT4X4> CurrentAnimationBoneTransforms;

	//动画序列
	std::map<std::string, CAnimationSequence*> Sequences;

	//记录时间
	float AnimationTime;
};