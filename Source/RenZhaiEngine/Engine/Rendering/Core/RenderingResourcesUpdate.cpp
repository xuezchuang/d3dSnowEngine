#include "RenderingResourcesUpdate.h"

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)
	{
		UploadBuffer->Unmap(0, NULL);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount, bool bConstBuffer)
{
	assert(InDevice);
	
	if (bConstBuffer)
	{
		ElementSize = GetConstantBufferByteSize(InElemetSize);
	}
	else
	{
		ElementSize = InElemetSize;
	}

	CD3DX12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElemetCount);
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&UploadBuffer)));

	ANALYSIS_HRESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index * ElementSize], InData, ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSzie)
{
	//222
	//256
	//454
	//512
	//(InTypeSzie + 255)& ~255;

	/*if (!(InTypeSzie % 256))
	{
		float NewFloat =  (float) InTypeSzie / 256.f;
		int Num = (NewFloat += 1);
		InTypeSzie = Num * 256;
	}*/

	//456
	//(456 + 255) & ~255;
	//711 & ~255;
	//0x02c7 & ~0x00ff
	//0x02c7 & 0xff00
	//0x0200
	return (InTypeSzie + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}
