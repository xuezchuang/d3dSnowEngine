#pragma once
#include "../Core/RenderLayer.h"

class FOperationHandleRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOperationHandleRenderLayer();

	virtual void BuildShader()override;
	virtual void BuildPSO()override;
	virtual int GetRenderLayerType() const override { return RENDERLAYER_OPERATION_HANDLE; }
};