#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueSkinnedRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueSkinnedRenderLayer();

	virtual void Draw(float DeltaTime);
	virtual void DrawObject(float DeltaTime, std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC = ERenderingConditions::RC_None);
	
	virtual void BuildShader();
	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& InMacro);

	virtual int GetRenderLayerType() const { return 23; }

	virtual void BuildPSO();

	virtual void ResetPSO();
};