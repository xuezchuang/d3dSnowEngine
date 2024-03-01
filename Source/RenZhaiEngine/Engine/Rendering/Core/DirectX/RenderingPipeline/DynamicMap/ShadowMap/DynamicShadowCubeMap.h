#pragma once
#include "../Core/DynamicCubeMap.h"


class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicShadowCubeMap :public FDynamicCubeMap
{
	typedef FDynamicCubeMap Super;

public:
	FDynamicShadowCubeMap();

	virtual void OnResetSize(int InWidth, int InHeight);
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer);
	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
public:

	virtual void BuildDepthStencilDescriptor();

protected:
	virtual void BuildRenderTargetRTV();
	virtual void BuildRenderTargetSRV();
};