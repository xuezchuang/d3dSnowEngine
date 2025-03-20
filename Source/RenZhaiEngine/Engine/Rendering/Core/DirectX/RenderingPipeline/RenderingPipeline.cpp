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
{																									//初始化GPS描述
	DirectXPipelineState.ResetGPSDesc();

	//渲染层级的初始化
	RenderLayer.Init(&GeometryMap,&DirectXPipelineState);

	//统一排序对渲染层级进行排序
	RenderLayer.Sort();

	//读取贴图纹理
	GeometryMap.LoadTexture();

	////构建雾气
	//GeometryMap.BuildFog();

	////构建动态的CubeMap
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

	//构建根签名
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DResourcesNumber());
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());

	//构建模型
	GeometryMap.Build();

	////构建动态反射Mesh
	//GeometryMap.BuildDynamicReflectionMesh();

	////构建常量描述堆
	//GeometryMap.BuildDescriptorHeap();

	//SSAO.BuildDescriptors();

	////初始化CubeMap 摄像机
	//DynamicCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));

	////构建深度模板描述
	//DynamicCubeMap.BuildDepthStencilDescriptor();

	////构建RTVDes
	//DynamicCubeMap.BuildRenderTargetDescriptor();

	////构建深度模板
	//DynamicCubeMap.BuildDepthStencil();

	////构建阴影
	//GeometryMap.BuildShadow();

	//构建常量缓冲区
	GeometryMap.BuildMeshConstantBuffer();

	//构建材质常量缓冲区
	GeometryMap.BuildMaterialShaderResourceView();

	////构建灯光常量缓冲区
	//GeometryMap.BuildLightConstantBuffer();

	////构建我们的视口常量缓冲区视图
	//GeometryMap.BuildViewportConstantBufferView();

	////构建贴图
	//GeometryMap.BuildTextureConstantBuffer();

	////构建雾气常量缓冲区
	//GeometryMap.BuildFogConstantBuffer();

	////蒙皮
	//GeometryMap.BuildSkinnedMeshConstantBuffer();

	////构建SSAO
	//SSAO.Build();

	////存储一个默认的GPS描述数据
	//DirectXPipelineState.SaveGPSDescAsDefault();

	//通过层级来构建PSO
	RenderLayer.BuildPSO();
}

void FRenderingPipeline::PreDraw(GraphicsContext& gfxContext, float DeltaTime)
{
	//DirectXPipelineState.PreDraw(context, DeltaTime);
	//GeometryMap.SetDescriptorHeaps();

	gfxContext.SetRootSignature(*RootSignature.GetRootSignature());

	GeometryMap.Draw(gfxContext,DeltaTime);

	//渲染SSAO
	//SSAO.Draw(DeltaTime);
	//RootSignature.SetGraphicsRootSignature();

	//存储我们的SSAO到指定的buffer
	//SSAO.SaveToSSAOBuffer();

	//主视口清除画布
	//ClearMainSwapChainCanvas();

	////重新绑定贴图
	//GeometryMap.Draw2DTexture(DeltaTime);

	////渲染
	//GeometryMap.DynamicShadowCubeMap.PreDraw(DeltaTime);

	////渲染阴影
	//GeometryMap.DrawShadow(DeltaTime);

	////动态反射
	//if(DynamicCubeMap.IsExitDynamicReflectionMesh())
	//{
	//	DynamicCubeMap.PreDraw(DeltaTime);
	//}

	//RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(GraphicsContext& gfxContext, float DeltaTime)
{	
	//主视口
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
	////CubeMap 覆盖原先被修改的CubeMap
	//GeometryMap.DrawCubeMapTexture(DeltaTime);

	////各类层级
	//RenderLayer.Draw(RENDERLAYER_BACKGROUND,DeltaTime);
	RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);
	//RenderLayer.Draw(RENDERLAYER_SKINNED_OPAQUE, DeltaTime);//渲染动画层
	//RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);

	////渲染选择层
	//RenderLayer.Draw(RENDERLAYER_SELECT, DeltaTime);
	//
	////渲染旋转面片
	//RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE_ROT_PLANE, DeltaTime);

	////渲染操作手柄
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
