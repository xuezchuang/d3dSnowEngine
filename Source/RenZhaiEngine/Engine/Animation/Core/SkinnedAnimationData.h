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

	//�Ǽ�
	CSkeleton* Skeleton;

	//��ǰ�Ķ���Transform
	std::vector<XMFLOAT4X4> CurrentAnimationBoneTransforms;

	//��������
	std::map<std::string, CAnimationSequence*> Sequences;

	//��¼ʱ��
	float AnimationTime;
};