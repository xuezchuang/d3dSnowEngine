#pragma once
#include "../Core/DynamicMap.h"

class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicShadowMap :public FDynamicMap
{
	typedef FDynamicMap Super;
public:

	FDynamicShadowMap();

public:
	virtual void OnResetSize(int InWidth, int InHeight);
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	virtual void Init(int InWidth, int InHeight);
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer);
	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
public:
	void GetViewportMatrix(XMFLOAT4X4 &OutViewMatrix, XMFLOAT4X4 &OutProjectMatrix);
public:
	void DrawShadowMapTexture(float DeltaTime);
public:
	void SetViewportPosition(const fvector_3d& InPosition);
	void SetViewportRotation(const fvector_3d& InRotation);
	void BuildViewMatrix(float DeltaTime);
	void BuildParallelLightMatrix(
		const fvector_3d& InDirection,
		const fvector_3d& InTargetPosition,
		float InRadius = 100.f);

	void BuildSpotLightMatrix(
		const fvector_3d& InDirection,
		const fvector_3d& InPosition,
		float InRadius = 100.f);

public:
	virtual void BuildViewport(const fvector_3d& InCenterPoint);

public:
	virtual void BuildRenderTargetDescriptor();
	virtual void BuildDepthStencilDescriptor();

protected:
	virtual void BuildRenderTargetSRV();

protected:
	GClientViewport* ShadowViewport;//
};