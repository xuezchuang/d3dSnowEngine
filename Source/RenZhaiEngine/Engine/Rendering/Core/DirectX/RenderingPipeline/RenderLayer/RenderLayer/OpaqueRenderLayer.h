#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueRenderLayer();
	virtual ~FOpaqueRenderLayer();

	virtual void DrawObject(float DeltaTime, std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC = ERenderingConditions::RC_None);
	
	virtual void BuildShader() override;

	virtual int GetRenderLayerType() const override { return RENDERLAYER_OPAQUE; }

	virtual void BuildPSO() override;

private:
	ComPtr<ID3DBlob> m_ShaderVS;
	ComPtr<ID3DBlob> m_ShaderPS;
};