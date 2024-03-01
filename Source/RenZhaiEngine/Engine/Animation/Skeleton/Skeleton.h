#pragma once

#include "../../EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"
#include "Bone.h"
#include "Skeleton.CodeReflection.h"

class CSkeleton :public CCoreMinimalObject
{
	CODEREFLECTION()

public:
	CSkeleton(){}
public:
	std::vector<int> BoneHierarchy;
	std::vector<FBone> Bones;
};