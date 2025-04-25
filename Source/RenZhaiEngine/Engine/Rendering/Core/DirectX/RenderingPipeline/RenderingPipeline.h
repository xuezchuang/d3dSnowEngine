#pragma once
#include "../../../../Interface/DirectXDeviceInterfece.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DefaultDirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "DynamicMap/CubeMap/DynamicReflectionCubeMap.h"
#include "RenderLayer/RenderLayerManage.h"
#include "UI/IMGUIPipeline.h"
#include "AmbientOcclusion/ScreenSpace/ScreenSpaceAmbientOcclusion.h"
#include "GFSDK_SSAO.h"
//#include "RootSignature.h"

class CMeshComponent;
//提供渲染内容的接口
class FRenderingPipeline :public IDirectXDeviceInterfece
{
public:
	FRenderingPipeline();

	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& MeshData);
	void BuildMesh(const size_t InMeshHash, CMeshComponent *InMesh,const FMeshRenderingData& MeshData);
	void DuplicateMesh(CMeshComponent* InMesh,std::shared_ptr<FRenderingData>& MeshData);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	virtual void OnResetSize(int InWidth, int InHeight);

	void BuildPipeline();

	virtual void PreDraw(GraphicsContext& gfxContext, float DeltaTime);
	virtual void Draw(GraphicsContext& gfxContext, float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	void InitializeHBAOPlus();
	void InitializeHBAOParameters();
	void RenderHBAOPlus(GraphicsContext& gfxContext, float DeltaTime);
public:
	FRenderLayerManage* GetRenderLayer() const { return const_cast<FRenderLayerManage*>(&RenderLayer); }
protected:

	//渲染层级
	FRenderLayerManage RenderLayer;

	FDirectXPipelineState DirectXPipelineState;
	FDefaultDirectXRootSignature RootSignature;
	FGeometryMap GeometryMap;

	FDynamicReflectionCubeMap DynamicCubeMap;
	

	FScreenSpaceAmbientOcclusion SSAO;

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_Scissor;

private:
	GFSDK_SSAO_Parameters   mAOParameters;
	GFSDK_SSAO_Context_D3D12* mSSAOContext;
	DescriptorHeap mSSAODescriptorHeapCBVSRVUAV;
	DescriptorHeap mSSAODescriptorHeapRTV;


//protected:
	//RootSignature m_RootSignature;
};