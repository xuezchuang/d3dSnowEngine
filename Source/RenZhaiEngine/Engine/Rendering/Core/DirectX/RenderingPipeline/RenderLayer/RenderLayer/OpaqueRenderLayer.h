#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueRenderLayer();
	virtual ~FOpaqueRenderLayer();
	virtual void Draw(GraphicsContext& context, float DeltaTime);
	virtual void DrawObject(float DeltaTime, std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC = ERenderingConditions::RC_None);
	
	virtual void BuildShader();

	virtual int GetRenderLayerType() const { return 0; }

	virtual void BuildPSO();

	virtual void ResetPSO(GraphicsContext& GfxContext);
private:
	ComPtr<ID3DBlob> m_ShaderVS;
	ComPtr<ID3DBlob> m_ShaderPS;
};