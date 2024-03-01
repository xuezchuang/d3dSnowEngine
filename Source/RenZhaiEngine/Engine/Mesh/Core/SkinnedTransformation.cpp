#include "SkinnedTransformation.h"

FSkinnedTransformation::FSkinnedTransformation()
{
	for (int i = 0;i < 64 ;i++)
	{
		BoneTransforms[i] = EngineMath::IdentityMatrix4x4();
	}
}