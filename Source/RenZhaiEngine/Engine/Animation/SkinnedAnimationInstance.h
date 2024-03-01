#pragma once

#include "../EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"
#include "Core/SkinnedAnimationData.h"
#include "SkinnedAnimationInstance.CodeReflection.h"

struct FSkinnedTransformation;
class CSkinnedAnimationInstance :public CCoreMinimalObject
{
	CODEREFLECTION()

public:
	CSkinnedAnimationInstance();

	virtual void Tick(float DeltaTime);

	void TickAnimation(float InDeltaTime, FSkinnedTransformation& OutSkinnedTransformation);

	void ResetSkinnedTransformation(FSkinnedTransformation& OutSkinnedTransformation);

	FSkinnedAnimationData *GetAnimationData() { return &AnimationData; }
protected:
	FSkinnedAnimationData AnimationData;
};