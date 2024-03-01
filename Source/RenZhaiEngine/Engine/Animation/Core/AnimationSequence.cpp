#include "AnimationSequence.h"

CAnimationSequence::CAnimationSequence()
{

}

void CAnimationSequence::ProcessAnimation(float InAnimationTime, CSkeleton* InSkeleton, std::vector<XMFLOAT4X4>& OutUnhandledBoneTransforms)
{
    for (int i = 0; i < SequenceData.size(); ++i)
    {
        SequenceData[i].ProcessBoneAnimation(InAnimationTime, InSkeleton, OutUnhandledBoneTransforms[i]);
    }
}

float CAnimationSequence::GetTimeLength()
{
    float Time = 0.f;
    for (auto& Tmp : SequenceData)
    {
        Time = max(Tmp.GetTimeLength(), Time);
    }

    return Time;
}
