#include "SkinnedAnimationInstance.h"
#include "../Mesh/Core/SkinnedTransformation.h"

CSkinnedAnimationInstance::CSkinnedAnimationInstance()
{

}

void CSkinnedAnimationInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void CSkinnedAnimationInstance::TickAnimation(float InDeltaTime, FSkinnedTransformation& OutSkinnedTransformation)
{
	AnimationData.AnimationTime += InDeltaTime * 0.02f;

	float MaxAnimationTime = AnimationData.GetCurrentAnimationTimeLength();
	if (AnimationData.AnimationTime >= MaxAnimationTime)
	{
		AnimationData.AnimationTime = 0.f;

		//Reset
		ResetSkinnedTransformation(OutSkinnedTransformation);
	}
	else
	{
		AnimationData.TickAnimation(InDeltaTime, OutSkinnedTransformation);
	}
}

void CSkinnedAnimationInstance::ResetSkinnedTransformation(FSkinnedTransformation& OutSkinnedTransformation)
{
	for (int i = 0; i < AnimationData.Skeleton->Bones.size(); i++)
	{
		OutSkinnedTransformation.BoneTransforms[i] = EngineMath::ToFloat4x4(AnimationData.Skeleton->Bones[i].BoneMatrix);
	}
}
