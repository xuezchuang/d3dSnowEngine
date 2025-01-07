#include "OpaqueRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../RenderWindow.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	RenderPriority = 2000;
}

FOpaqueRenderLayer::~FOpaqueRenderLayer()
{
	RenderPriority = 2000;
}

void FOpaqueRenderLayer::Draw(FCommandContext& context, float DeltaTime)
{
	//重置当前的PSO
	//ResetPSO();

	context.SetPipelineState(m_PSO);

	Super::Draw(context,DeltaTime);
}

void FOpaqueRenderLayer::DrawObject(float DeltaTime, std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC)
{
	//Super::DrawObject(DeltaTime, InWeakRenderingData, RC);
}

void FOpaqueRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	m_ShaderVS = D3D12RHI::Get().CreateShader(getpath(L"Resources/Shit_Shaders/Hello.hlsl"), "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	m_ShaderPS = D3D12RHI::Get().CreateShader(getpath(L"Resources/Shit_Shaders/Hello.hlsl"), "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());

	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

}

void FOpaqueRenderLayer::BuildPSO()
{
	Super::BuildPSO();
	m_PSO.SetRootSignature(*DirectXPipelineState->GetRootSignature());
	m_PSO.SetRasterizerState(FPipelineState::RasterizerDefault);
	m_PSO.SetBlendState(FPipelineState::BlendDisable);
	m_PSO.SetDepthStencilState(FPipelineState::DepthStateDisabled);
	m_PSO.SetInputLayout(InputElementDesc.size(), InputElementDesc.data());


	m_PSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_PSO.SetRenderTargetFormats(1, &RenderWindow::Get().GetColorFormat(), DXGI_FORMAT_UNKNOWN);

	m_PSO.SetVertexShader(CD3DX12_SHADER_BYTECODE(m_ShaderVS.Get()));
	m_PSO.SetPixelShader(CD3DX12_SHADER_BYTECODE(m_ShaderPS.Get()));
	m_PSO.Finalize();
}

void FOpaqueRenderLayer::ResetPSO(FCommandContext& context)
{
	//DirectXPipelineState->ResetPSO();
	context.SetPipelineState(m_PSO);
}
