#include "BoneAnimation.h"
#include "../../Math/EngineMath.h"
#include "../Skeleton/Skeleton.h"

void FBoneAnimation::ProcessBoneAnimation(float InAnimationTime, CSkeleton* InSkeleton,XMFLOAT4X4& OutUnhandledBoneTransforms)
{
    if (KeyAnim.size() == 0)
    {
        for (auto& TmpBone : InSkeleton->Bones)
        {
            if (TmpBone.Name == Name)
            {
                OutUnhandledBoneTransforms = EngineMath::ToFloat4x4(TmpBone.BoneMatrix);
            }
        }
    }
    else if(InAnimationTime <= KeyAnim.front().KeyTime)
    {
        ProcessBoneAnimation(KeyAnim.front(), OutUnhandledBoneTransforms);
    }
    else if (InAnimationTime >= KeyAnim.back().KeyTime)
    {
        ProcessBoneAnimation(KeyAnim.back(), OutUnhandledBoneTransforms);
    }
    else
    {
        ProcessBoneAnimation(InAnimationTime, OutUnhandledBoneTransforms);
    }
}

void FBoneAnimation::ProcessBoneAnimation(const FIEKey& InKey, XMFLOAT4X4& OutUnhandledBoneTransforms)
{
    XMFLOAT3 Scale = EngineMath::ToFloat3(InKey.Scale);
    XMFLOAT3 Translation = EngineMath::ToFloat3(InKey.Translation);
    XMFLOAT4 Rotation = EngineMath::ToFloat4(InKey.Rotation);

    XMVECTOR ScaleVector = XMLoadFloat3(&Scale);
    XMVECTOR TranslationVector = XMLoadFloat3(&Translation);
    XMVECTOR QuatVector = XMLoadFloat4(&Rotation);

    XMVECTOR ZeroVector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMStoreFloat4x4(&OutUnhandledBoneTransforms,
        XMMatrixAffineTransformation(
            ScaleVector,
            ZeroVector,
            QuatVector,
            TranslationVector));
}

void FBoneAnimation::ProcessBoneAnimation(float InAnimationTime, XMFLOAT4X4& OutUnhandledBoneTransforms)
{
    for (int i = 0; i < KeyAnim.size() - 1; ++i)
    {
        if (InAnimationTime >= KeyAnim[i].KeyTime && 
            InAnimationTime <= KeyAnim[i+1].KeyTime)
        {
            float LerpPercent = (InAnimationTime - KeyAnim[i].KeyTime) / (KeyAnim[i + 1].KeyTime - KeyAnim[i].KeyTime);
       
            //获取
            XMFLOAT3 Scale = EngineMath::ToFloat3(KeyAnim[i].Scale);
            XMFLOAT3 ScaleNest = EngineMath::ToFloat3(KeyAnim[i + 1].Scale);

            XMFLOAT3 Translation = EngineMath::ToFloat3(KeyAnim[i].Translation);
            XMFLOAT3 TranslationNest = EngineMath::ToFloat3(KeyAnim[i + 1].Translation);

            XMFLOAT4 Quat = EngineMath::ToFloat4(KeyAnim[i].Rotation);
            XMFLOAT4 QuatNest = EngineMath::ToFloat4(KeyAnim[i+1].Rotation);

            //转换
            XMVECTOR ScaleVector = XMLoadFloat3(&Scale);
            XMVECTOR ScaleVectorNest = XMLoadFloat3(&ScaleNest);

            XMVECTOR TranslationVector = XMLoadFloat3(&Translation);
            XMVECTOR TranslationVectorNest = XMLoadFloat3(&TranslationNest);

            XMVECTOR QuatVector = XMLoadFloat4(&Quat);
            XMVECTOR QuatVectorNest = XMLoadFloat4(&QuatNest);

            //插值
            XMVECTOR LerpScale = XMVectorLerp(ScaleVector, ScaleVectorNest, LerpPercent);
            XMVECTOR LerpTranslation = XMVectorLerp(TranslationVector, TranslationVectorNest, LerpPercent);
            XMVECTOR LerpQuat = XMQuaternionSlerp(QuatVector, QuatVectorNest, LerpPercent);

            //合成
            XMVECTOR ZeroVector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

            XMStoreFloat4x4(&OutUnhandledBoneTransforms,
                XMMatrixAffineTransformation(
                    LerpScale,
                    ZeroVector,
                    LerpQuat,
                    LerpTranslation));
        }
    }
}

float FBoneAnimation::GetTimeLength()
{
    if (KeyAnim.size() == 0)
    {
        return 0.0f;
    }
  
    return KeyAnim[KeyAnim.size() - 1].KeyTime;
}
