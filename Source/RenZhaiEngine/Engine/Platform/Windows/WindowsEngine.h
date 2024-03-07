#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"
#include "../../Rendering/Enigne/Core/RenderingEngine.h"

class CDirectXRenderingEngine;
class CWorld;
class CMeshManage;

class CWindowsEngine :public CEngine
{
	friend class IDirectXDeviceInterfece;
public:
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);

	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void Tick(float DeltaTime);
	virtual void OnResetSize(int InWidth, int InHeight,int wParam);
	virtual void OnActive(int wParam,int lParam);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

	CDirectXRenderingEngine* GetRenderingEngine() const { return RenderingEngine; }
public:
	void InitPath();
public:
	CMeshManage* GetMeshManage() const;
	CWorld* GetWorld()const { return World; }
public:

	bool InitWindows(FWinMainCommandParameters InParameters);

protected:
	HWND MianWindowsHandle;//Ö÷windows¾ä±ú

protected:
	CDirectXRenderingEngine* RenderingEngine;
#if EDITOR_ENGINE
	class CEditorEngine* EditorEngine;
#endif
	CWorld* World;

private:
	bool m_bPaused = false;
};
#endif