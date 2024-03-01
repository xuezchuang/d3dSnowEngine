#pragma once
#include "MeshType.h"

struct FSkinnedTransformation
{
	FSkinnedTransformation();

	XMFLOAT4X4 BoneTransforms[64];
};
