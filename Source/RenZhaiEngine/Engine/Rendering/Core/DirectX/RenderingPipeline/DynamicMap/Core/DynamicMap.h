#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../RenderTarget/Core/RenderTarget.h"

struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicMap :public IDirectXDeviceInterfece
{
public:
	FDynamicMap();

public:
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer);
	virtual void Init(int InWidth, int InHeight);
	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);

	virtual void BuildSRVOffset();
	virtual void BuildRTVOffset();
public:
	template<class T>
	void CreateRenderTarget();

	std::shared_ptr<FRenderTarget> GetRenderTarget() const { return RenderTarget; }

public:

	void SetSRVOffset(UINT InOffset) { SRVOffset = InOffset; }
	UINT GetSRVOffset() { return SRVOffset; }

	void SetRTVOffset(UINT InOffset) { RTVOffset = InOffset; }
	UINT GetRTVOffset() { return RTVOffset; }

	UINT GetWidth()const { return Width; }
	UINT GetHeight()const { return Height; }
protected:
	FGeometryMap* GeometryMap;//����Map
	FDirectXPipelineState* DirectXPipelineState;//���߶��� ���ڰ�

	FRenderLayerManage* RenderLayer;//��Ⱦ�㼶

	UINT Width;
	UINT Height;

	UINT SRVOffset;//�󶨵�Shader�Ĵ���
	UINT RTVOffset;//��ȾĿ���ƫ��

	std::shared_ptr<FRenderTarget> RenderTarget;//��ȾĿ��
};

template<class T>
void FDynamicMap::CreateRenderTarget()
{
	if (!RenderTarget)
	{
		RenderTarget = std::make_shared<T>();
	}
}
