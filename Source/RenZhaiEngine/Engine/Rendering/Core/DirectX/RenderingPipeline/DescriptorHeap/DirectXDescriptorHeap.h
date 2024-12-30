#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"

struct FDirectXDescriptorHeap :public IDirectXDeviceInterfece_Struct
{
	void Build(UINT InNumDescriptor);

	void SetDescriptorHeaps();

	ID3D12DescriptorHeap* GetHeap()const { return CBVHeap.Get(); }
protected:
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
};