#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPipelineType.h"
#include "CommandContext.h"

class RootSignature;

//提供渲染内容的接口
struct FDirectXPipelineState :public IDirectXDeviceInterfece_Struct
{
public:
	FDirectXPipelineState();

	void PreDraw(GraphicsContext& context, float DeltaTime);
	void PostDraw(float DeltaTime);

	void ResetGPSDesc();

	//绑定输入布局
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);
	
	//绑定根签名
	void BindRootSignature(RootSignature* InRootSignature);
	RootSignature* GetRootSignature();
	//绑定顶点着色器和像素着色器
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	//构建参数
	void BuildParam();

	//BuildPSO
	void Build(int InPSOType);
	
	//绘制的时候调用
	void ResetPSO(int InPSOType);

	//只提供不透明层使用
	void ResetPSO();
public:
	FORCEINLINE D3D12_GRAPHICS_PIPELINE_STATE_DESC &GetGPSDesc() { return GPSDesc; }

public:
	void SetFillMode(bool bWireframe);

	//渲染目标
	void SetRenderTarget(int Index,const D3D12_RENDER_TARGET_BLEND_DESC &InRenderTargetBlend);

	void SetRasterizerState(const CD3DX12_RASTERIZER_DESC &InRasterizerDesc);
	void SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc);
	
	//将当前设置作为默认
	void SaveGPSDescAsDefault();

	//按键捕获
public:
	void CaptureKeyboardKeys();

private:
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC DefaultGPSDesc;

	EPipelineState PipelineState;
private:
	RootSignature* m_RootSignature;
};