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
	void BuildUIPipeline();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

public:
	FRenderLayerManage* GetRenderLayer() const { return const_cast<FRenderLayerManage*>(&RenderLayer); }
	FIMGUIPipeline* GetUIPipeline() { return &UIPipeline; }
protected:

	//渲染层级
	FRenderLayerManage RenderLayer;

	FDirectXPipelineState DirectXPipelineState;
	FDefaultDirectXRootSignature RootSignature;
	FGeometryMap GeometryMap;

	FDynamicReflectionCubeMap DynamicCubeMap;
	FIMGUIPipeline UIPipeline;

	FScreenSpaceAmbientOcclusion SSAO;
};