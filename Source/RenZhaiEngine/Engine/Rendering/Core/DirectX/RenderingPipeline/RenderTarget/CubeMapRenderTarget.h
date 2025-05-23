#pragma once
#include "Core/RenderTarget.h"

//提供渲染内容的接口
class FCubeMapRenderTarget :public FRenderTarget
{
	typedef FRenderTarget Super;
	friend class FDynamicCubeMap;
public:
	FCubeMapRenderTarget();

	virtual void Init(UINT InWidth,UINT InHeight,DXGI_FORMAT InFormat);

public:
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPURenderTargetView(int Index);
private:
	virtual void BuildRenderTargetMap();
	virtual void BuildSRVDescriptors();
	virtual void BuildRTVDescriptors();

private:
	//RTV
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> CPURenderTargetView;
};