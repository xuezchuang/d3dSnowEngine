#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueShadowRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueShadowRenderLayer();

	virtual void Draw(GraphicsContext& context, float DeltaTime);

	virtual void BuildShader();

	void BuildVientianeShadowShader();

	virtual int GetRenderLayerType() const { return 8; }

	virtual void BuildPSO();

	virtual void ResetPSO();
	virtual void ResetPSO(EPipelineState InPipelineState);
};