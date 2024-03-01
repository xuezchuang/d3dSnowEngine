#pragma once

#include "../../EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"
#include "SkinnedAnimationData.h"
#include "BoneAnimation.h"
#include "AnimationSequence.CodeReflection.h"

class CAnimationSequence :public CCoreMinimalObject
{
	CODEREFLECTION()

public:
	CAnimationSequence();

	void ProcessAnimation(float InAnimationTime, CSkeleton* InSkeleton, std::vector<XMFLOAT4X4>& OutUnhandledBoneTransforms);
public:
	float GetTimeLength();

	std::vector<FBoneAnimation> SequenceData;
};