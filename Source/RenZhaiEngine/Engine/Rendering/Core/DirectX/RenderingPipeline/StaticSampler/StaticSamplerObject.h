#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "SamplerManager.h"

struct FStaticSamplerObject :public IDirectXDeviceInterfece_Struct
{
	FStaticSamplerObject();

	void BuildStaticSampler();
	
	//D3D12_STATIC_SAMPLER_DESC* GetData();
	int GetSize()const;


	//��̬������ʽ
	std::vector<SamplerDesc> SamplerDescs;
};