#include "OpaqueRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "BufferManager.h"
#include "Display.h"
//#include "../RenderWindow.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	RenderPriority = 2000;
}

FOpaqueRenderLayer::~FOpaqueRenderLayer()
{
	RenderPriority = 2000;
}

void FOpaqueRenderLayer::Draw(GraphicsContext& context, float DeltaTime)
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

	std::wstring ShaderPath = BuildShadersPaths(L"Hello");
	VertexShader.BuildShaders(ShaderPath, "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	//PixelShader.BuildShaders(ShaderPath, "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());

	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//std::wstring ShaderPath = BuildShadersPaths(L"Test");
	//VertexShader.BuildShaders(ShaderPath, "VS", "vs_5_1");
	//PixelShader.BuildShaders(ShaderPath, "PS", "ps_5_1");

	//InputElementDesc =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//};
}

void FOpaqueRenderLayer::BuildPSO()
{
	Super::BuildPSO();
	m_PSO.SetRootSignature(*DirectXPipelineState->GetRootSignature());

	D3D12_RASTERIZER_DESC RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	D3D12_BLEND_DESC BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	D3D12_DEPTH_STENCIL_DESC DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//RasterizerState = Graphics::RasterizerDefault;
	//RasterizerState = Graphics::RasterizerTwoSided;
	RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//m_PSO.SetRasterizerState(RasterizerDefaultCw);
	//D3D12_BLEND_DESC BlendDesc = CD3DX12_BLEND_DESC();
	//BlendState = Graphics::BlendDisable;
	//
	DepthStencilState = Graphics::DepthStateReadWrite;
	// D3D12_DEPTH_STENCIL_DESC DepthState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//m_PSO.SetDepthStencilState(DepthState);

	//DepthStencilState.DepthEnable = true;
	//DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//DepthStencilState.StencilEnable = false;

	m_PSO.SetRasterizerState(RasterizerState);
	m_PSO.SetBlendState(BlendState);
	m_PSO.SetDepthStencilState(DepthStencilState);

	m_PSO.SetInputLayout(InputElementDesc.size(), InputElementDesc.data());


	m_PSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	DXGI_FORMAT ColorFormat = Graphics::g_SceneColorBuffer.GetFormat();
	DXGI_FORMAT DepthFormat = Graphics::g_SceneDepthBuffer.GetFormat();

	m_PSO.SetRenderTargetFormats(1, &ColorFormat, DepthFormat);
	//m_PSO.SetRenderTargetFormats(0, NULL, DepthFormat);

	m_PSO.SetVertexShader(VertexShader.GetBufferPointer(), VertexShader.GetBufferSize());
	//m_PSO.SetPixelShader(PixelShader.GetBufferPointer(), PixelShader.GetBufferSize());
	m_PSO.Finalize();
}

void FOpaqueRenderLayer::ResetPSO(GraphicsContext& context)
{
	//DirectXPipelineState->ResetPSO();
	context.SetPipelineState(m_PSO);
}
