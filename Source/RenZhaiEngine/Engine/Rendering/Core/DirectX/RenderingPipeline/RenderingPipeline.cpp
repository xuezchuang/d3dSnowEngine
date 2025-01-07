#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "RootSignature/DirectXRootSignatureType.h"
#include "StaticSampler/StaticSamplerObject.h"

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
	SSAO.UpdateCalculations(DeltaTime, ViewportInfo);
	GeometryMap.DynamicShadowCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
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
	//他原来只会一个一个绑定??? 一种类型的可以一起绑定...+
	FStaticSamplerObject StaticSamplerObject;
	StaticSamplerObject.BuildStaticSampler();
	m_RootSignature.Reset(11, StaticSamplerObject.SamplerDescs.size());
	m_RootSignature[Signature_Object].InitAsBufferCBV(0);
	m_RootSignature[Signature_ObjectSkinned].InitAsBufferCBV(1);
	m_RootSignature[Signature_Viewport].InitAsBufferCBV(2);
	m_RootSignature[Signature_Light].InitAsBufferCBV(3);
	m_RootSignature[Signature_Fog].InitAsBufferCBV(4);

	m_RootSignature[Signature_Material].InitAsBufferSRV(0, 1);


	m_RootSignature[Signature_Texture2D].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, GeometryMap.GetDrawTexture2DResourcesNumber(), D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_CubeMapTexture].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_ShadowMap].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_ShadowCubeMap].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_SSAO].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 1, D3D12_SHADER_VISIBILITY_PIXEL);


	m_RootSignature.InitStaticSampler(StaticSamplerObject.SamplerDescs);
	m_RootSignature.Finalize(L"RootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	DirectXPipelineState.BindRootSignature(&m_RootSignature);

	////构建模型
	//GeometryMap.Build();

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

	////构建常量缓冲区
	//GeometryMap.BuildMeshConstantBuffer();

	////构建材质常量缓冲区
	//GeometryMap.BuildMaterialShaderResourceView();

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

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//需要一个PSO
	DirectXPipelineState.PreDraw(DeltaTime);

	GeometryMap.SetDescriptorHeaps();
	RootSignature.SetGraphicsRootSignature();

	//渲染灯光材质贴图等(必须要放在 这个位置 否则天崩地裂)
	GeometryMap.Draw(DeltaTime);

	//渲染SSAO
	//SSAO.Draw(DeltaTime);
	RootSignature.SetGraphicsRootSignature();

	//存储我们的SSAO到指定的buffer
	SSAO.SaveToSSAOBuffer();

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

void FRenderingPipeline::Draw(FCommandContext& context, float DeltaTime)
{	
	context.SetRootSignature(m_RootSignature);
	//主视口
	GeometryMap.DrawViewport(DeltaTime);

	////CubeMap 覆盖原先被修改的CubeMap
	//GeometryMap.DrawCubeMapTexture(DeltaTime);

	////各类层级
	//RenderLayer.Draw(RENDERLAYER_BACKGROUND,DeltaTime);
	RenderLayer.Draw(context, RENDERLAYER_OPAQUE, DeltaTime);
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
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}
