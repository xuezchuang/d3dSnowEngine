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

//动画相关
struct MESHIMPORTEXPORTTOOL_API FIEKey
{
	FIEKey()
		:Scale(1.f)
	{}

	double KeyTime;

	fvector_3d Translation;
	fvector_3d Scale;
	fvector_4d Rotation;//有可能是四元数 也有可能是欧拉角
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

//蒙皮网格相关
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

	int LinkIndex;//控制的是哪个顶点 顶点的Index
	float WeightValue;//这个顶点被控制的权重
	int LinkBoneIndex;//链接的骨骼是哪个
};

//骨骼
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

//骨架
typedef std::vector<FIEBone> FIEBones;

struct MESHIMPORTEXPORTTOOL_API FIESkeleton
{
	FIEBones Bones;
	vector<int> BoneInherits;//层次
};