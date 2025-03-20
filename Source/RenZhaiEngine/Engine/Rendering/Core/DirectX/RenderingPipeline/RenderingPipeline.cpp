#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "RootSignature/DirectXRootSignatureType.h"
#include "StaticSampler/StaticSamplerObject.h"
#include "CommandContext.h"
#include "BufferManager.h"
#include "Display.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash, InMesh, MeshData);
}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash,InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData)
{
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//SSAO.UpdateCalculations(DeltaTime, ViewportInfo);
	//GeometryMap.DynamicShadowCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::OnResetSize(int InWidth, int InHeight)
{
	SSAO.OnResetSize(InWidth,InHeight);
	DynamicCubeMap.OnResetSize(InWidth, InHeight);
	GeometryMap.OnResetSize(InWidth, InHeight);
	RenderLayer.OnResetSize(InWidth, InHeight);
}

void FRenderingPipeline::BuildPipeline()
{																									//��ʼ��GPS����
	DirectXPipelineState.ResetGPSDesc();

	//��Ⱦ�㼶�ĳ�ʼ��
	RenderLayer.Init(&GeometryMap,&DirectXPipelineState);

	//ͳһ�������Ⱦ�㼶��������
	RenderLayer.Sort();

	//��ȡ��ͼ����
	GeometryMap.LoadTexture();

	////��������
	//GeometryMap.BuildFog();

	////������̬��CubeMap
	//DynamicCubeMap.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//SSAO.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//SSAO.Init(GetVieportWidth(),GetVieportHeight());
	//
	//GeometryMap.DynamicShadowMap.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//GeometryMap.DynamicShadowCubeMap.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//������ǩ��
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DResourcesNumber());
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());

	//����ģ��
	GeometryMap.Build();

	////������̬����Mesh
	//GeometryMap.BuildDynamicReflectionMesh();

	////��������������
	//GeometryMap.BuildDescriptorHeap();

	//SSAO.BuildDescriptors();

	////��ʼ��CubeMap �����
	//DynamicCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));

	////�������ģ������
	//DynamicCubeMap.BuildDepthStencilDescriptor();

	////����RTVDes
	//DynamicCubeMap.BuildRenderTargetDescriptor();

	////�������ģ��
	//DynamicCubeMap.BuildDepthStencil();

	////������Ӱ
	//GeometryMap.BuildShadow();

	//��������������
	GeometryMap.BuildMeshConstantBuffer();

	//�������ʳ���������
	GeometryMap.BuildMaterialShaderResourceView();

	////�����ƹⳣ��������
	//GeometryMap.BuildLightConstantBuffer();

	////�������ǵ��ӿڳ�����������ͼ
	//GeometryMap.BuildViewportConstantBufferView();

	////������ͼ
	//GeometryMap.BuildTextureConstantBuffer();

	////������������������
	//GeometryMap.BuildFogConstantBuffer();

	////��Ƥ
	//GeometryMap.BuildSkinnedMeshConstantBuffer();

	////����SSAO
	//SSAO.Build();

	////�洢һ��Ĭ�ϵ�GPS��������
	//DirectXPipelineState.SaveGPSDescAsDefault();

	//ͨ���㼶������PSO
	RenderLayer.BuildPSO();
}

void FRenderingPipeline::PreDraw(GraphicsContext& gfxContext, float DeltaTime)
{
	//DirectXPipelineState.PreDraw(context, DeltaTime);
	//GeometryMap.SetDescriptorHeaps();

	gfxContext.SetRootSignature(*RootSignature.GetRootSignature());

	GeometryMap.Draw(gfxContext,DeltaTime);

	//��ȾSSAO
	//SSAO.Draw(DeltaTime);
	//RootSignature.SetGraphicsRootSignature();

	//�洢���ǵ�SSAO��ָ����buffer
	//SSAO.SaveToSSAOBuffer();

	//���ӿ��������
	//ClearMainSwapChainCanvas();

	////���°���ͼ
	//GeometryMap.Draw2DTexture(DeltaTime);

	////��Ⱦ
	//GeometryMap.DynamicShadowCubeMap.PreDraw(DeltaTime);

	////��Ⱦ��Ӱ
	//GeometryMap.DrawShadow(DeltaTime);

	////��̬����
	//if(DynamicCubeMap.IsExitDynamicReflectionMesh())
	//{
	//	DynamicCubeMap.PreDraw(DeltaTime);
	//}

	//RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(GraphicsContext& gfxContext, float DeltaTime)
{	
	//���ӿ�
	gfxContext.SetRootSignature(*RootSignature.GetRootSignature());
	//GeometryMap.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	// 
	gfxContext.TransitionResource(Graphics::g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	gfxContext.TransitionResource(Graphics::g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	gfxContext.ClearColor(Graphics::g_SceneColorBuffer);
	gfxContext.ClearDepth(Graphics::g_SceneDepthBuffer);
	gfxContext.SetRenderTarget(Graphics::g_SceneColorBuffer.GetRTV(), Graphics::g_SceneDepthBuffer.GetDSV_DepthReadOnly());
	gfxContext.SetViewportAndScissor(0, 0, Graphics::g_SceneColorBuffer.GetWidth(), Graphics::g_SceneColorBuffer.GetHeight());

	GeometryMap.Draw(gfxContext, DeltaTime);

	// 
	//gfxContext.FlushResourceBarriers();

//	CommandContext.SetRenderTargets(1, &BackBuffer.GetRTV(), BufferManager::g_SceneDepthZ.GetDSV());
//	CommandContext.ClearColor(BackBuffer);
//	CommandContext.ClearDepth(BufferManager::g_SceneDepthZ);
//
//	CommandContext.SetViewportAndScissor(0, 0, m_GameDesc.Width, m_GameDesc.Height);
	////CubeMap ����ԭ�ȱ��޸ĵ�CubeMap
	//GeometryMap.DrawCubeMapTexture(DeltaTime);

	////����㼶
	//RenderLayer.Draw(RENDERLAYER_BACKGROUND,DeltaTime);
	RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);
	//RenderLayer.Draw(RENDERLAYER_SKINNED_OPAQUE, DeltaTime);//��Ⱦ������
	//RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);

	////��Ⱦѡ���
	//RenderLayer.Draw(RENDERLAYER_SELECT, DeltaTime);
	//
	////��Ⱦ��ת��Ƭ
	//RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE_ROT_PLANE, DeltaTime);

	////��Ⱦ�����ֱ�
	//RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE, DeltaTime);

	//DirectXPipelineState.CaptureKeyboardKeys();

	//gfxContext.TransitionResource(Graphics::g_SceneDepthBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}
