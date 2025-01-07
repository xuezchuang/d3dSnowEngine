#pragma once

#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"
#include "../../../../../../Shader/Core/ShaderType.h"
#include "../../RenderingPipelineType.h"
#include "../PipelineState.h"
#include "../CommandContext.h"

struct FDirectXPipelineState;
struct FGeometryMap;
struct FRenderingData;
struct FViewportInfo;

DEFINITION_SIMPLE_SINGLE_DELEGATE(FBuildPSODelegate, void, D3D12_GRAPHICS_PIPELINE_STATE_DESC&);

class FRenderLayer 
	:public IDirectXDeviceInterfece
	,public std::enable_shared_from_this<FRenderLayer>
{
	friend struct FGeometry;
	friend struct FGeometryMap;

public:
	FBuildPSODelegate BuildPSODelegate;

public:
	FRenderLayer();

	//基础注册环节
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(FCommandContext& context, float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	virtual void DrawObject(FCommandContext& context,float DeltaTime,std::weak_ptr<FRenderingData>& InWeakRenderingData,ERenderingConditions RC = ERenderingConditions::RC_None);
	virtual void FindObjectDraw(float DeltaTime, const CMeshComponent* InKey);

	virtual void BuildPSO();

	void RegisterRenderLayer();

	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro> &InMacro);

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//单独设置PSO
	virtual void ResetPSO(FCommandContext& GfxContext);
	virtual void ResetPSO(EPipelineState InPipelineState);

	//渲染 不包含设置PSO
	virtual void DrawMesh(FCommandContext& context,float DeltaTime,ERenderingConditions RC = ERenderingConditions::RC_None);
public:
	std::wstring BuildShadersPaths(const std::wstring& InShadersHLSLName);
public:
	const UINT GetRenderPriority()const { return RenderPriority; }

	void Add(std::weak_ptr<FRenderingData> InRenderingData);
	void Remove(std::weak_ptr<FRenderingData> InRenderingData);

	void Clear();
public:
	virtual void BuildShader() {};
	virtual int GetRenderLayerType()const = 0;
protected:
	UINT RenderPriority;

protected:
	FShader VertexShader;
	FShader PixelShader;
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;

	std::vector<std::weak_ptr<FRenderingData>> RenderDatas;

	FGeometryMap* GeometryMap;
	FDirectXPipelineState* DirectXPipelineState;
protected:

	FGraphicsPipelineState m_PSO;
};