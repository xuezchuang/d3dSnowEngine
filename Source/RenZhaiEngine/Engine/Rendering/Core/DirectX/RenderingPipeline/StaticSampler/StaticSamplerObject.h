#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "SamplerManager.h"

struct FStaticSamplerObject :public IDirectXDeviceInterfece_Struct
{
	FStaticSamplerObject();

	void BuildStaticSampler();
	
	//D3D12_STATIC_SAMPLER_DESC* GetData();
	int GetSize()const;


	//静态采样方式
	std::vector<SamplerDesc> SamplerDescs;
};