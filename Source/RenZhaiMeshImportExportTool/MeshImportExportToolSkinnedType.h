#pragma once
#include "MeshImportExportTool.h"

struct FIEKeyValue
{
	FIEKeyValue()
		:Time(-1.0)
		, Value(0)
	{}
	double Time;
	float Value;
};

//�������
struct MESHIMPORTEXPORTTOOL_API FIEKey
{
	FIEKey()
		:Scale(1.f)
	{}

	double KeyTime;

	fvector_3d Translation;
	fvector_3d Scale;
	fvector_4d Rotation;//�п�������Ԫ�� Ҳ�п�����ŷ����
};

struct MESHIMPORTEXPORTTOOL_API FIEBoneAnimation
{
	std::string Name;

	std::vector<FIEKey> KeyAnim;
};

typedef std::vector<FIEBoneAnimation> FIEAnimationSequence;

struct MESHIMPORTEXPORTTOOL_API FIEAnimationData
{
	std::map<string, FIEAnimationSequence> AnimationSequences;
};

//��Ƥ�������
struct MESHIMPORTEXPORTTOOL_API FIESkinnedWidget
{
	FIESkinnedWidget()
		:LinkIndex(0)
		,WeightValue(0.f)
	{}

	FIESkinnedWidget(int InLinkIndex, float InWeightValue)
		:LinkIndex(InLinkIndex)
		,WeightValue(InWeightValue)
		,LinkBoneIndex(0)
	{}

	FIESkinnedWidget(int InLinkIndex, float InWeightValue, int InLinkBoneIndex)
		:LinkIndex(InLinkIndex)
		, WeightValue(InWeightValue)
		, LinkBoneIndex(InLinkBoneIndex)
	{}

	int LinkIndex;//���Ƶ����ĸ����� �����Index
	float WeightValue;//������㱻���Ƶ�Ȩ��
	int LinkBoneIndex;//���ӵĹ������ĸ�
};

//����
struct MESHIMPORTEXPORTTOOL_API FIEBone
{
	FIEBone()
		:bBindPose(false)
	{}

	FIEBone(const std::string& InNewBoneName)
		:Name(InNewBoneName)
		, bBindPose(false)
		, bLocalMatrix(false)
	{}

	std::string Name;

	fmatrix_4x4 BoneMatrix;
	bool bBindPose;
	bool bLocalMatrix;

	std::vector<FIESkinnedWidget> Widgets;
};

//�Ǽ�
typedef std::vector<FIEBone> FIEBones;

struct MESHIMPORTEXPORTTOOL_API FIESkeleton
{
	FIEBones Bones;
	vector<int> BoneInherits;//���
};