#pragma once
#include "../../../Core/Engine.h"
#include "../../../Interface/DirectXDeviceInterfece.h"

namespace ConstructBuffer
{
	struct FConstructBuffer :public IDirectXDeviceInterfece_Struct
	{
		ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* InData, UINT64 InDataSize);
	};
}