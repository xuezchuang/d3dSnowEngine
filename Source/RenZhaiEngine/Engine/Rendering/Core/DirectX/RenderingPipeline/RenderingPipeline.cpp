#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "RootSignature/DirectXRootSignatureType.h"
#include "StaticSampler/StaticSamplerObject.h"
#include "CommandContext.h"
#include "BufferManager.h"
#include "Display.h"
#include "SSAO.h"
#include "../Engine/Core/Camera.h"
#include "../Engine/Mesh/Core/Mesh.h"
#include "../Engine/Core/World.h"
#include "../Engine/Component/Mesh/Core/MeshComponent.h"
#include <D3D12.h>

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
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);

	//统一排序对渲染层级进行排序
	RenderLayer.Sort();

	//读取贴图纹理
	GeometryMap.LoadTexture();

	////构建雾气
	//GeometryMap.BuildFog();

	//构建动态的CubeMap
	DynamicCubeMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayer);

	//SSAO.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//SSAO.Init(GetVieportWidth(),GetVieportHeight());

	GeometryMap.DynamicShadowMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayer);

	GeometryMap.DynamicShadowCubeMap.Init(&GeometryMap, &DirectXPipelineState, &RenderLayer);

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

	//构建阴影
	GeometryMap.BuildShadow();

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

	InitializeHBAOPlus();
}

void FRenderingPipeline::PreDraw(GraphicsContext& gfxContext, float DeltaTime)
{
	if (1)
	{
		RenderHBAOPlus(gfxContext, DeltaTime);
		return;
	}
	//DirectXPipelineState.PreDraw(context, DeltaTime);
	//GeometryMap.SetDescriptorHeaps();
	// 

	gfxContext.SetRootSignature(*RootSignature.GetRootSignature());
	// Begin rendering depth
	gfxContext.TransitionResource(Graphics::g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	gfxContext.ClearDepth(Graphics::g_SceneDepthBuffer);
	//Graphics::g_SceneColorBuffer.SetClearColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	//gfxContext.SetRenderTarget(Graphics::g_SceneColorBuffer.GetRTV(), Graphics::g_SceneDepthBuffer.GetDSV_DepthReadOnly());
	gfxContext.SetDepthStencilTarget(Graphics::g_SceneDepthBuffer.GetDSV());

	if (m_Viewport.Width == 0)
	{
		DepthBuffer* DSV = &Graphics::g_SceneDepthBuffer;

		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = (float)DSV->GetWidth();
		m_Viewport.Height = (float)DSV->GetHeight();
		m_Viewport.MinDepth = 1.0f;
		m_Viewport.MaxDepth = 0.0f;

		m_Scissor.left = 0;
		m_Scissor.right = DSV->GetWidth();
		m_Scissor.top = 0;
		m_Scissor.bottom = DSV->GetWidth();
	}
	gfxContext.SetViewportAndScissor(m_Viewport, m_Scissor);
	gfxContext.FlushResourceBarriers();
	GeometryMap.Draw(gfxContext, DeltaTime);
	RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);

	float ProjMat = GetCamera()->GetAspect();
	SSAO::Render(gfxContext, &ProjMat, 1.0, 10000.0f);
	
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
	
	//GeometryMap.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	// 
	gfxContext.TransitionResource(Graphics::g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	gfxContext.TransitionResource(Graphics::g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	gfxContext.ClearDepth(Graphics::g_SceneDepthBuffer);
	gfxContext.ClearColor(Graphics::g_SceneColorBuffer);
	gfxContext.SetRenderTarget(Graphics::g_SceneColorBuffer.GetRTV(), Graphics::g_SceneDepthBuffer.GetDSV());
	//gfxContext.SetViewportAndScissor(0, 0, Graphics::g_SceneColorBuffer.GetWidth(), Graphics::g_SceneColorBuffer.GetHeight());
	if (m_Viewport.Width == 0)
	{
		DepthBuffer* DSV = &Graphics::g_SceneDepthBuffer;

		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = (float)DSV->GetWidth();
		m_Viewport.Height = (float)DSV->GetHeight();
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_Scissor.left = 0;
		m_Scissor.right = DSV->GetWidth();
		m_Scissor.top = 0;
		m_Scissor.bottom = DSV->GetHeight();
	}
	//gfxContext.SetDepthStencilTarget(Graphics::g_SceneDepthBuffer.GetDSV());
	gfxContext.SetViewportAndScissor(m_Viewport, m_Scissor);
	gfxContext.FlushResourceBarriers();
	////CubeMap 覆盖原先被修改的CubeMap
	//GeometryMap.DrawCubeMapTexture(DeltaTime);

	////各类层级
	//RenderLayer.Draw(RENDERLAYER_BACKGROUND,DeltaTime);
	gfxContext.SetRootSignature(*RootSignature.GetRootSignature());
	GeometryMap.Draw(gfxContext, DeltaTime);
	RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);
	//RenderLayer.Draw(RENDERLAYER_SKINNED_OPAQUE, DeltaTime);//渲染动画层
	//RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);

	////渲染选择层
	//RenderLayer.Draw(RENDERLAYER_SELECT, DeltaTime);
	//
	////渲染旋转面片
	//RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE_ROT_PLANE, DeltaTime);

	//渲染操作手柄
	RenderLayer.Draw(gfxContext, RENDERLAYER_OPERATION_HANDLE, DeltaTime);

	//DirectXPipelineState.CaptureKeyboardKeys();

	//gfxContext.TransitionResource(Graphics::g_SceneDepthBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}

void FRenderingPipeline::InitializeHBAOPlus()
{
	InitializeHBAOParameters();

	auto CopySRVToHeap = [](DepthBuffer& buffer, DescriptorHeap& heap, uint32_t heapSlot) -> DescriptorHandle
	{
		DescriptorHandle dstHandle = heap[heapSlot];
		Graphics::g_Device->CopyDescriptorsSimple(
			1,
			dstHandle,
			buffer.GetDepthSRV(),
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);
		return dstHandle;
	};

	mSSAODescriptorHeapCBVSRVUAV.Create(L"HBAO SRV Heap", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2 + GFSDK_SSAO_NUM_DESCRIPTORS_CBV_SRV_UAV_HEAP_D3D12);
	mSSAODescriptorHeapRTV.Create(L"HBAO RTV Heap", D3D12_DESCRIPTOR_HEAP_TYPE_RTV, GFSDK_SSAO_NUM_DESCRIPTORS_CBV_SRV_UAV_HEAP_D3D12);

	CopySRVToHeap(Graphics::g_SceneDepthBuffer, mSSAODescriptorHeapCBVSRVUAV, 0);
	CopySRVToHeap(Graphics::g_SceneBackDepthBuffer, mSSAODescriptorHeapCBVSRVUAV, 1);


	GFSDK_SSAO_DescriptorHeaps_D3D12 DescriptorHeaps;

	DescriptorHeaps.CBV_SRV_UAV.pDescHeap = mSSAODescriptorHeapCBVSRVUAV.GetHeapPointer();
	DescriptorHeaps.CBV_SRV_UAV.BaseIndex = 2;

	DescriptorHeaps.RTV.pDescHeap = mSSAODescriptorHeapRTV.GetHeapPointer();
	DescriptorHeaps.RTV.BaseIndex = 0;

	GFSDK_SSAO_CustomHeap CustomHeap;
	CustomHeap.new_ = ::operator new;
	CustomHeap.delete_ = ::operator delete;

	const UINT NodeMask = 1;

	GFSDK_SSAO_Status status = GFSDK_SSAO_CreateContext_D3D12(Graphics::g_Device, NodeMask, DescriptorHeaps, &mSSAOContext, &CustomHeap);
	assert(status == GFSDK_SSAO_OK);
}

void FRenderingPipeline::InitializeHBAOParameters()
{
	mAOParameters = {};

	mAOParameters.Radius = 2.f;
	mAOParameters.Bias = 0.2f;
	mAOParameters.PowerExponent = 2.f;
	mAOParameters.Blur.Enable = true;
	mAOParameters.Blur.Sharpness = 32.f;
	mAOParameters.Blur.Radius = GFSDK_SSAO_BLUR_RADIUS_4;
	mAOParameters.DepthStorage = GFSDK_SSAO_FP32_VIEW_DEPTHS;
	mAOParameters.EnableDualLayerAO = false;
}

void FRenderingPipeline::RenderHBAOPlus(GraphicsContext& gfxContext, float DeltaTime)
{
	gfxContext.SetRootSignature(*RootSignature.GetRootSignature());
	// Begin rendering depth
	gfxContext.TransitionResource(Graphics::g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	gfxContext.ClearColor(Graphics::g_SceneColorBuffer);

	gfxContext.TransitionResource(Graphics::g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	gfxContext.TransitionResource(Graphics::g_SceneBackDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	gfxContext.ClearDepth(Graphics::g_SceneDepthBuffer);
	//gfxContext.GetCommandList()->ClearDepthStencilView(Graphics::g_SceneDepthBuffer.GetDSV(), D3D12_CLEAR_FLAG_DEPTH, 1.0, Graphics::g_SceneDepthBuffer.GetClearStencil(), 0, nullptr);
	gfxContext.ClearDepth(Graphics::g_SceneBackDepthBuffer);
	//Graphics::g_SceneColorBuffer.SetClearColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	//gfxContext.SetRenderTarget(Graphics::g_SceneColorBuffer.GetRTV(), Graphics::g_SceneDepthBuffer.GetDSV_DepthReadOnly());
	

	if (m_Viewport.Width == 0)
	{
		DepthBuffer* DSV = &Graphics::g_SceneDepthBuffer;

		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = (float)DSV->GetWidth();
		m_Viewport.Height = (float)DSV->GetHeight();
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_Scissor.left = 0;
		m_Scissor.right = DSV->GetWidth();
		m_Scissor.top = 0;
		m_Scissor.bottom = DSV->GetHeight();
	}
	gfxContext.SetViewportAndScissor(m_Viewport, m_Scissor);
	gfxContext.FlushResourceBarriers();


	float ProjMat = GetCamera()->GetAspect();

	GFSDK_SSAO_InputData_D3D12 InputData = {};
	InputData.DepthData.DepthTextureType = GFSDK_SSAO_HARDWARE_DEPTHS;

	if (mAOParameters.EnableDualLayerAO)
	{
		GMesh* mesh_2 = dynamic_cast<GMesh*>(GetWorld()->GetActors().at(1));
		if (NULL != mesh_2)
		{
			mesh_2->GetMeshComponent()->SetVisible(false);
		}

		gfxContext.SetDepthStencilTarget(Graphics::g_SceneDepthBuffer.GetDSV());
		GeometryMap.Draw(gfxContext, DeltaTime);
		RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);


		if (NULL != mesh_2)
		{
			mesh_2->GetMeshComponent()->SetVisible(true);
		}
		GMesh* mesh_1 = dynamic_cast<GMesh*>(GetWorld()->GetActors().at(0));
		if (NULL != mesh_1)
		{
			mesh_1->GetMeshComponent()->SetVisible(false);
		}

		gfxContext.SetDepthStencilTarget(Graphics::g_SceneBackDepthBuffer.GetDSV());
		GeometryMap.Draw(gfxContext, DeltaTime);
		RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);


		if (NULL != mesh_1)
		{
			mesh_1->GetMeshComponent()->SetVisible(true);
		}

		//CD3DX12_GPU_DESCRIPTOR_HANDLE DepthSrvGpuHandle(mSSAODescriptorHeapCBVSRVUAV->GetGPUDescriptorHandleForHeapStart());
		//InputData.DepthData.FullResDepthTextureSRV.pResource = mDepthStencil[0].Get();
		//InputData.DepthData.FullResDepthTextureSRV.GpuHandle = DepthSrvGpuHandle.ptr;

		//DepthSrvGpuHandle.Offset(1, mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		//InputData.DepthData.FullResDepthTexture2ndLayerSRV.pResource = mDepthStencil[1].Get();
		//InputData.DepthData.FullResDepthTexture2ndLayerSRV.GpuHandle = DepthSrvGpuHandle.ptr;
	}
	else
	{
		gfxContext.SetDepthStencilTarget(Graphics::g_SceneDepthBuffer.GetDSV());
		GeometryMap.Draw(gfxContext, DeltaTime);
		RenderLayer.Draw(gfxContext, RENDERLAYER_OPAQUE, DeltaTime);

		InputData.DepthData.FullResDepthTextureSRV.pResource = Graphics::g_SceneDepthBuffer.GetResource();
		InputData.DepthData.FullResDepthTextureSRV.GpuHandle = mSSAODescriptorHeapCBVSRVUAV[0].GetGpuPtr();
		
	}

	//XMMATRIX ProjectMatrix = XMLoadFloat4x4(&GetCamera()->ProjectMatrix);
	//XMMATRIX ViewMatrixRH = XMMatrixMultiply(ProjectMatrix, XMMatrixScaling(1.0f, 1.0f, 1.0f));

	XMMATRIX ViewMatrixRH = XMLoadFloat4x4(&GetCamera()->ProjectMatrixRH);
	// DepthData
	InputData.DepthData.ProjectionMatrix.Data = GFSDK_SSAO_Float4x4((const GFSDK_SSAO_FLOAT*)&ViewMatrixRH);
	InputData.DepthData.ProjectionMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	InputData.DepthData.MetersToViewSpaceUnits = 1.0f;
	InputData.NormalData.Enable = false;

	GFSDK_SSAO_RenderMask RenderMask = GFSDK_SSAO_RENDER_AO;

	// Set SSAO descriptor heap
	{
		ID3D12DescriptorHeap* descHeaps[] = { mSSAODescriptorHeapCBVSRVUAV.GetHeapPointer() };
		gfxContext.GetCommandList()->SetDescriptorHeaps(ARRAYSIZE(descHeaps), descHeaps);
	}

	GFSDK_SSAO_Output_D3D12 Output;
	GFSDK_SSAO_RenderTargetView_D3D12 rtv{};
	rtv.pResource = Graphics::g_SceneColorBuffer.GetResource();
	rtv.CpuHandle = Graphics::g_SceneColorBuffer.GetRTV().ptr;

	Output.pRenderTargetView = &rtv;

	

	{
		
		GFSDK_SSAO_Status status = mSSAOContext->RenderAO(Graphics::g_CommandManager.GetCommandQueue(), gfxContext.GetCommandList(), InputData, mAOParameters, Output, RenderMask);
		assert(status == GFSDK_SSAO_OK);
	}
	gfxContext.TransitionResource(Graphics::g_SceneColorBuffer, D3D12_RESOURCE_STATE_PRESENT);
}
