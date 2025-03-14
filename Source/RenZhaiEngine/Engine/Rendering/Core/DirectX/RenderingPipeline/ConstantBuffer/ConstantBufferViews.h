#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"

struct FConstantBufferViews :public IDirectXDeviceInterfece_Struct
{
	void CreateConstant(UINT ObjectSize,UINT ObjectCount,bool bConstBuffer = true);

	void Update(int Index, const void* InData);

	void BuildConstantBuffer(
		CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,
		UINT InConstantBufferNum,
		UINT InHandleOffset = 0);

	UINT GetConstantBufferByteSize();

	ID3D12Resource* GetBuffer() { return Constant->GetBuffer(); }
protected:
	shared_ptr<FRenderingResourcesUpdate> Constant;
};