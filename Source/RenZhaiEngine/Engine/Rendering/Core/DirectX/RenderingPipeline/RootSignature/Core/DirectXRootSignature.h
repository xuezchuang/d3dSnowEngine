#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../StaticSampler/StaticSamplerObject.h"
#include "../DirectXRootSignatureType.h"

//提供渲染内容的接口
struct FDirectXRootSignature :public IDirectXDeviceInterfece_Struct
{
	FDirectXRootSignature();

	virtual void BuildRootSignature(UINT InTextureNum = 1);

	virtual void SetGraphicsRootSignature();
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }

protected:
	ComPtr<ID3D12RootSignature> RootSignature;
	FStaticSamplerObject StaticSamplerObject;
};