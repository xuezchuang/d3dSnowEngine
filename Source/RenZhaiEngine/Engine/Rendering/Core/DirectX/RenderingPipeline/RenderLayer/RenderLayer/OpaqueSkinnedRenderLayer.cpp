#include "OpaqueSkinnedRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueSkinnedRenderLayer::FOpaqueSkinnedRenderLayer()
{
	RenderPriority = 2006;
}

void FOpaqueSkinnedRenderLayer::Draw(float DeltaTime)
{
	//���õ�ǰ��PSO
	ResetPSO();

	Super::Draw(DeltaTime);
}

void FOpaqueSkinnedRenderLayer::DrawObject(float DeltaTime, std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC)
{
	Super::DrawObject(DeltaTime, InWeakRenderingData, RC);
}

void FOpaqueSkinnedRenderLayer::BuildShader()
{
	//����Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	std::wstring ShaderPath = BuildShadersPaths(L"Hello");
	VertexShader.BuildShaders(ShaderPath, "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(ShaderPath, "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	//���벼��
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{"BONEINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueSkinnedRenderLayer::BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& InMacro)
{
	Super::BuildShaderMacro(InMacro);

	{
		ShaderType::FShaderMacro ShaderMacro;
		char TextureNumBuff[10] = { 0 };
		ShaderMacro.Name = "SKINNED";
		ShaderMacro.Definition = "1";

		InMacro.push_back(ShaderMacro);
	}
}

void FOpaqueSkinnedRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	//��������
	DirectXPipelineState->Build(Opaque_Skinned);
}

void FOpaqueSkinnedRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(Opaque_Skinned);
}
