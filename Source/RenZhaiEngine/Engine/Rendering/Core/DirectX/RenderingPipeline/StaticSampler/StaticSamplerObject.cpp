#include "StaticSamplerObject.h"

FStaticSamplerObject::FStaticSamplerObject()
{

}

void FStaticSamplerObject::BuildStaticSampler()
{
	SamplerDescs.resize(4);

	{
		SamplerDesc& RefSamplerDesc = SamplerDescs[0];
		RefSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	}
	
	{
		SamplerDesc& RefSamplerDesc = SamplerDescs[1];
		RefSamplerDesc.MaxAnisotropy = 8;
	}

	{
		//Shader
		SamplerDesc& RefSamplerDesc = SamplerDescs[2];
		RefSamplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		RefSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		RefSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		RefSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		RefSamplerDesc.SetBorderColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
	}

	{
		//Depth
		SamplerDesc& RefSamplerDesc = SamplerDescs[3];
		RefSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		RefSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		RefSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		RefSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		RefSamplerDesc.SetBorderColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
//D3D12_STATIC_SAMPLER_DESC* FStaticSamplerObject::GetData()
//{
//	return SamplerDescs.data();
//}

int FStaticSamplerObject::GetSize() const
{
	return SamplerDescs.size();
}
