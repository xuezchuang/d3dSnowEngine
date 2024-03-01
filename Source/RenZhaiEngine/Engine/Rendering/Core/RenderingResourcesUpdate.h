#pragma once
#include "../../EngineMinimal.h"

class FRenderingResourcesUpdate :public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount, bool bConstBuffer = true);

	void Update(int Index, const void* InData);

	UINT GetConstantBufferByteSize(UINT InTypeSzie);
	UINT GetConstantBufferByteSize();

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); }
private:
	ComPtr<ID3D12Resource> UploadBuffer;
	UINT ElementSize;
	BYTE* Data;
};