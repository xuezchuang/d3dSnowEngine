#pragma once
#include "../../StaticSampler/StaticSamplerObject.h"
#include "RootSignature.h"
#include "../Engine/Interface/DirectXDeviceInterfece.h"

class RootSignature;
//提供渲染内容的接口
struct FDirectXRootSignature :public IDirectXDeviceInterfece_Struct
{
	FDirectXRootSignature();

	virtual void BuildRootSignature(UINT InTextureNum = 1) = 0;

	virtual void SetGraphicsRootSignature();
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	//ID3D12RootSignature* GetRootSignature() { return m_RootSignature.GetSignature(); }
	RootSignature* GetRootSignature() { return &m_RootSignature; }
protected:
	RootSignature m_RootSignature;
	//ComPtr<ID3D12RootSignature> RootSignature;
	//FStaticSamplerObject StaticSamplerObject;
};