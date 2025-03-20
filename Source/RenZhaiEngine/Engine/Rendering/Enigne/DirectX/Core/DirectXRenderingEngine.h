#pragma once
#include "../../Core/RenderingEngine.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "CommandContext.h"

class CLightManage;
class CMeshManage;
class CWorld;
class CDirectXRenderingEngine :public CRenderingEngine
{
	friend class IDirectXDeviceInterfece;
	friend class CWindowsEngine;
public:
	CDirectXRenderingEngine();
	~CDirectXRenderingEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);

	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void UpdateCalculations(float DeltaTime,const FViewportInfo &ViewportInfo);
	virtual void Tick(float DeltaTime);
	virtual void OnResetSize(int InWidth, int InHeight, int wParam);

	void RenderScene(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();
public:
	void StartSetMainViewportRenderTarget();
	void EndSetMainViewportRenderTarget();
	void ClearMainSwapChainCanvas();

public:
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }
	UINT GetDXGISampleCount()const;
	UINT GetDXGISampleQuality()const;

	CMeshManage* GetMeshManage() const{ return MeshManage; }
	CLightManage* GetLightManage() const { return LightManage; }
protected:

	bool InitDirect3D();
	void PostInitDirect3D();

protected:
	CLightManage* LightManage;
	CMeshManage* MeshManage;
	CWorld* World;

protected:
	UINT M4XQualityLevels;
	bool bMSAA4XEnabled;
	DXGI_FORMAT BackBufferFormat;
	DXGI_FORMAT DepthStencilFormat;
};