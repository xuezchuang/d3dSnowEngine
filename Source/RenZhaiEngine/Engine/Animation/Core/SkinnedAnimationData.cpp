#include "SkinnedAnimationData.h"
#include "AnimationSequence.h"
#include "../../Math/EngineMath.h"
#include "../../Mesh/Core/SkinnedTransformation.h"

FSkinnedAnimationData::FSkinnedAnimationData()
{
	Skeleton = nullptr;
	AnimationTime = 0.f;
}

void FSkinnedAnimationData::TickAnimation(float InDeltaTime, FSkinnedTransformation& OutSkinnedTransformation)
{
	//������ δ����Ķ���
	int BoneNumber = Skeleton->Bones.size();
	std::vector<XMFLOAT4X4> UnhandledBoneTransforms(BoneNumber);

	//������ ��Ҫ�Ƕ������ 
	if (ProcessAnimation(CurrentAnimationSequence, AnimationTime, Skeleton, UnhandledBoneTransforms))
	{
		//������Ϻ� ���Transforms
		std::vector<XMFLOAT4X4> BoneTransforms(BoneNumber);
		ProcessBoneTransforms(UnhandledBoneTransforms,BoneTransforms);

		//�͹���Transform��� תΪ�������ǹ�����Transform
		ProcessBoneTransforms(BoneTransforms, OutSkinnedTransformation);
	}
}

bool FSkinnedAnimationData::ProcessAnimation(
	const std::string& InSequenceName,
	float InAnimationTime,
	CSkeleton* InSkeleton,
	std::vector<XMFLOAT4X4>& OutUnhandledBoneTransforms)
{
	auto Sequence = Sequences.find(InSequenceName);
	if (Sequence != Sequences.end())
	{
		Sequence->second->ProcessAnimation(InAnimationTime, InSkeleton, OutUnhandledBoneTransforms);

		return true;
	}

	return false;
}

void FSkinnedAnimationData::ProcessBoneTransforms(const std::vector<XMFLOAT4X4>& UnhandledBoneTransforms, std::vector<XMFLOAT4X4>& OutBoneTransforms)
{
	//Root������˷�
	OutBoneTransforms[0] = UnhandledBoneTransforms[0];

	for (int i = 1; i < Skeleton->Bones.size(); i++)
	{
		XMMATRIX CurrentUnhandledBoneTransforms = XMLoadFloat4x4(&UnhandledBoneTransforms[i]);
	
		int ParentIndex = Skeleton->BoneHierarchy[i];
		XMMATRIX CurrentBoneParentTransforms = XMLoadFloat4x4(&OutBoneTransforms[ParentIndex]);
	
		XMMATRIX BoneTransforms = XMMatrixMultiply(CurrentUnhandledBoneTransforms,CurrentBoneParentTransforms);
	
		XMStoreFloat4x4(&OutBoneTransforms[i], BoneTransforms);
	}
}

void FSkinnedAnimationData::ProcessBoneTransforms(const std::vector<XMFLOAT4X4>& InBoneTransforms, FSkinnedTransformation& OutSkinnedTransformation)
{
	for (int i = 0; i < Skeleton->Bones.size(); i++)
	{
		XMFLOAT4X4 BoneMatrix = EngineMath::ToFloat4x4(Skeleton->Bones[i].BoneMatrix);;
		XMMATRIX BoneTransforms = XMLoadFloat4x4(&BoneMatrix);

		XMMATRIX RelativeBoneTransforms = XMLoadFloat4x4(&InBoneTransforms[i]);

		XMMATRIX FinalTransform = XMMatrixMultiply(BoneTransforms,RelativeBoneTransforms);

		XMStoreFloat4x4(&OutSkinnedTransformation.BoneTransforms[i],FinalTransform);
	}
}

float FSkinnedAnimationData::GetTimeLength(const std::string& InSequenceName)
{
	auto Iter = Sequences.find(InSequenceName);
	if (Iter != Sequences.end())
	{
		return Iter->second->GetTimeLength();
	}

	return 0.0f;
}

float FSkinnedAnimationData::GetCurrentAnimationTimeLength()
{
	return GetTimeLength(CurrentAnimationSequence);
}
