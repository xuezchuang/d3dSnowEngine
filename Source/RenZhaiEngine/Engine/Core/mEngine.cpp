#include "../EngineMinimal.h"
#include "mEngine.h"
#include "../DirectX12Lib/include/ImguiManager.h"
#include "../DirectX12Lib/include/RenderWindow.h"
#include "../Platform/Windows/WindowsEngine.h"
#include "../EditorEngine/EditorEngine.h"
#include "../EngineFactory.h"
#include "../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include <CodeReflection/ClassManage.h>
#include <CoreObject/ClassObject.h>


CEngine* Engine = NULL;

void mEngine::OnStartup()
{
	tStart = std::chrono::high_resolution_clock::now();

	//m_EditorEngine = new CEditorEngine();
	//m_RenderingEngine = new CDirectXRenderingEngine();
	//m_RenderingEngine->PostInit();
	Engine = FEngineFactory::Instance();
	//Engine->PreExit();
	//Engine->Init();
	//Engine->PostInit();
	for(int i = 0; i < FClassManage::GetNum(); i++)
	{
		if(CClassObject* InObjectClass = FClassManage::GetClassByIndex(i))
		{
			InObjectClass->GetDefaultObject();
		}
	}
//
#if EDITOR_ENGINE
	GetEditorEngine()->BuildEditor();
#endif
}

void mEngine::OnUpdate()
{
	tEnd = std::chrono::high_resolution_clock::now();
	//int curSizeX, curSizeY;
	//GameInput::GetSize(curSizeX, curSizeY);
	//if(m_SizeX != curSizeX || m_SizeY != curSizeY)
	//{
	//	Initialize()
	//}
	//m_SizeX = curSizeX;
	//m_SizeY = curSizeY;
}

void mEngine::OnRender()
{
	FCommandContext& CommandContext = FCommandContext::Begin(D3D12_COMMAND_LIST_TYPE_DIRECT, L"3D Queue");
	
	RenderWindow& renderWindow = RenderWindow::Get();
	FColorBuffer& BackBuffer = renderWindow.GetBackBuffer();
	
	CommandContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	BackBuffer.SetClearColor(Vector3f(0.5f, 0.58f, 0.8f));
	CommandContext.ClearColor(BackBuffer);

	CommandContext.SetRenderTargets(1, &BackBuffer.GetRTV());

	OnTest(CommandContext);

	OnGUI(CommandContext);
	CommandContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_PRESENT);
	CommandContext.Finish(true);

	RenderWindow::Get().Present();
}

void mEngine::OnGUI(FCommandContext& CommandContext)
{
	ImguiManager::Get().NewFrame();

	float delta = std::chrono::duration<float, std::milli>(tEnd - tStart).count();

	//GetEngine()->Dra
#if EDITOR_ENGINE
	GetEditorEngine()->DrawEditor(delta);
#endif
	ImguiManager::Get().Render(CommandContext, RenderWindow::Get());
}

void mEngine::OnTest(FCommandContext& CommandContext)
{

}

CDirectXRenderingEngine* mEngine::GetDirectRenderingEngine()const
{
	if(CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		return InEngine->RenderingEngine;
	}
	return nullptr;
}

#if EDITOR_ENGINE
CEditorEngine* mEngine::GetEditorEngine() const
{
	if(CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		return InEngine->EditorEngine;
	}
	return nullptr;
}
#endif

