#pragma once
#include "../Core/RenderLayer.h"

class FOperationHandleRotPlaneRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOperationHandleRotPlaneRenderLayer();

	virtual void Draw(float DeltaTime);

	virtual void BuildShader();
	virtual void BuildPSO();
	virtual int GetRenderLayerType() const { return 11; }

	virtual void ResetPSO();
};