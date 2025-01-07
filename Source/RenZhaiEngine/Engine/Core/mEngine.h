#pragma once
#include "include/Game.h"
#include "include/CommandContext.h"
#include "include/D3D12RHI.h"
#include "include/d3dx12.h"
#include "include/RootSignature.h"
#include "include/PipelineState.h"
#include "include/ColorBuffer.h"
#include "include/Camera.h"
#include "include/Light.h"

class CEditorEngine;
class CDirectXRenderingEngine;
class FRenderingPipeline;

class Atmospheric
{
public:

	void OnStartup();
	void OnUpdate();
	void OnRender(FCommandContext& CommandContext);

	FCamera* m_Camera;
	GameDesc* m_GameDesc;
private:
	void SetupCameraLight();
	void SetupMesh();
	void SetupShaders();
	void SetupPipelineState();
	void ScatteringPassCS(FCommandContext& GfxContext);
	void ScatteringPassPS(FCommandContext& GfxContext);
	void PostProcess(FCommandContext& GfxContext);

private:
	__declspec(align(16)) struct
	{
		FMatrix		InvProjectionMatrix;
		FMatrix		InvViewMatrix;
		Vector4f	ScreenParams;
		Vector4f	LightDirAndIntensity;
		Vector4f	EarthCenterAndRadius;
		Vector2f	DensityScaleHeight;
		float		AtmosphereRadius;
		float		MieG;
		float		MieCoef;
		Vector3f	RayleiCoef;
	} m_Constants;

	FRootSignature m_ScatteringSignature;
	FRootSignature m_PostSignature;

	FComputePipelineState m_ScatteringCSPSO;
	FGraphicsPipelineState m_ScatteringPSPSO;
	FGraphicsPipelineState m_PostPSO;

	FColorBuffer m_ScatteringBuffer;

	ComPtr<ID3DBlob> m_ScatteringCS;
	ComPtr<ID3DBlob> m_ScreenQuadVS;
	ComPtr<ID3DBlob> m_PostPS;
	ComPtr<ID3DBlob> m_ScatteringPS;

	bool m_bUseComputeShader = true;

	FDirectionalLight m_DirectionLight;
};
class mEngine : public FGame
{
public:
	mEngine(const GameDesc& Desc) : FGame(Desc)
	{
		m_Atmospheric.m_Camera = &m_Camera;
		m_Atmospheric.m_GameDesc = &m_GameDesc;
	}

	virtual void OnStartup() override;

	virtual void OnUpdate() override;

	virtual void OnRender() override;
private:
	void OnGUI(FCommandContext& CommandContext);
	void OnTest(FCommandContext& CommandContext);
	std::chrono::high_resolution_clock::time_point tStart, tEnd;
	int m_SizeX = 0;
	int m_SizeY = 0;


	CEditorEngine* GetEditorEngine()const;
	CDirectXRenderingEngine* GetDirectRenderingEngine()const;
	FRenderingPipeline* GetFRenderingPipeline()const;

	Atmospheric m_Atmospheric;
	FCamera m_Camera;
};