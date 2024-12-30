#pragma once
#include "Core/DirectXRootSignature.h"
#include "../StaticSampler/StaticSamplerObject.h"

//�ṩ��Ⱦ���ݵĽӿ�
struct FDefaultDirectXRootSignature :public FDirectXRootSignature
{
private:
	typedef FDirectXRootSignature Super;

public:
	FDefaultDirectXRootSignature();

	virtual void BuildRootSignature(UINT InTextureNum = 1);

	virtual void SetGraphicsRootSignature();
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);
};