#include "WindowsEngine.h"
#include "CoreObject/CoreMinimalObject.h"

#include "../../Core/Construction/MacroConstruction.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Mesh/BoxMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Mesh/CylinderMesh.h"
#include "../../Mesh/ConeMesh.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/CustomMesh.h"
#include "../../Core/World.h"
#include "../../Core/Camera.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Rendering/Enigne/DirectX/DirectX12RenderingEngine.h"
#include "../../Component/Input/Input.h"

#if EDITOR_ENGINE
#include "../../../EditorEngine/EditorEngine.h"
#endif

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"
#include <../../packages/directxtex_desktop_win10.2024.6.5.1/include/DirectXTex.h>
#include "Display.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CWindowsEngine::CWindowsEngine()
	:RenderingEngine(new CDirectX12RenderingEngine())
#if EDITOR_ENGINE
	,EditorEngine(new CEditorEngine())
#endif // 0

{
	
}

CWindowsEngine::~CWindowsEngine()
{
	delete RenderingEngine;
}

int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	////自适应屏幕大小绑定
	OnResetSizeDelegate.AddFunction(this, &CWindowsEngine::OnResetSize);
	ActiveDelegate.AddFunction(this, &CWindowsEngine::OnActive);

	InitPath();

	//日志系统初始化
	std::string LogPath = 
		FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineLogsPath());

	init_log_system(LogPath.c_str());
	Engine_Log("Log Init.");

	//处理命令

	Engine_Log("Engine pre initialization complete.");

	//渲染引擎初始化
	RenderingEngine->PreInit(InParameters);

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	InitWindows(InParameters);

	//RenderingEngine->SetMianWindowsHandle(MianWindowsHandle);

	RenderingEngine->Init(InParameters);

	//注册对应的world
	BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(, this);
	World = CreateObject<CWorld>(Param, new CWorld());
	RenderingEngine->World = World;

	Engine_Log("Engine initialization complete.");

	////显示窗口
	//ShowWindow(MianWindowsHandle, SW_SHOW);

	////窗口是脏的，刷新一下
	//UpdateWindow(MianWindowsHandle);

	return 0;
}

int CWindowsEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	RenderingEngine->PostInit();

	for (auto& Tmp : GObjects)
	{
		Tmp->BeginInit();
	}

	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	if (m_bPaused)
	{
		Sleep(30);
		return;
	}
	for (auto& Tmp : GObjects)
	{
		if (Tmp->IsTick())
		{
			Tmp->Tick(DeltaTime);
		}
	}

	if(World)
	{
		if (World->GetCamera())
		{
			FViewportInfo ViewportInfo;

			XMFLOAT3 ViewPosition = World->GetCamera()->GetPosition();
			ViewportInfo.ViewPosition = XMFLOAT4(ViewPosition.x, ViewPosition.y, ViewPosition.z, 1.f);

			//ViewportInfo.ViewMatrix = World->GetCamera()->ViewMatrix;
			//ViewportInfo.ProjectMatrix = World->GetCamera()->ProjectMatrix;

			ViewportInfo.ViewMatrix = World->GetCamera()->ViewMatrixRH;
			ViewportInfo.ProjectMatrix = World->GetCamera()->ProjectMatrixRHZ;

			RenderingEngine->UpdateCalculations(DeltaTime, ViewportInfo);

			RenderingEngine->Tick(DeltaTime);
		}
	}
}

void CWindowsEngine::OnResetSize(int InWidth, int InHeight, int wParam)
{
	if (wParam == SIZE_MINIMIZED)
	{
		m_bPaused = true;
		return;
	}
	m_bPaused = false;

	Display::Resize(InWidth, InHeight);
	RenderingEngine->OnResetSize(InWidth, InHeight, wParam);

#if EDITOR_ENGINE
	EditorEngine->OnResetSize(InWidth, InHeight, wParam);
#endif
}

void CWindowsEngine::OnActive(int wParam, int lParam)
{
	if(wParam == WA_INACTIVE)
	{
		m_bPaused = true;
	}
	else
	{
		m_bPaused = false;
	}
}

void CWindowsEngine::RenderScene(float DeltaTime)
{
	RenderingEngine->RenderScene(DeltaTime);
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	Engine_Log("Engine post exit complete.");
	return 0;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	Engine_Log("Engine exit complete.");
	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	RenderingEngine->PostExit();

	Engine_Log("Engine post exit complete.");
	return 0;
}

void CWindowsEngine::InitPath()
{
	auto CreateFileDirectory = [](const std::string& InPath)
	{
		std::string LogPath =
			FEnginePathHelper::RelativeToAbsolutePath(
				InPath);
		//创建路径
		create_file_directory(LogPath.c_str());
	};

	//日志系统初始化
	CreateFileDirectory(FEnginePathHelper::GetEngineLogsPath());

	//Content
	CreateFileDirectory(FEnginePathHelper::GetEngineContentPath());

	//反射代码路径
	CreateFileDirectory(FEnginePathHelper::GetEngineCodeReflectionPath());

	wstring ShadersPathW = FEnginePathHelper::GetEngineShadersPath();
	char ShaderPaht[1024] = { 0 };
	wchar_t_to_char(ShaderPaht,1024, ShadersPathW.c_str());
	CreateFileDirectory(ShaderPaht);

	CreateFileDirectory(FEnginePathHelper::GetEngineBinariesPath());
}

CMeshManage* CWindowsEngine::GetMeshManage() const
{
	return RenderingEngine->GetMeshManage();
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	////注册窗口
	//WNDCLASSEX WindowsClass;
	//WindowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
	//WindowsClass.cbClsExtra = 0;//是否需要额外空间
	//WindowsClass.cbWndExtra = 0;//是否需要额外内存
	//WindowsClass.hbrBackground = nullptr;//如果有设置哪就是GDI擦除
	//WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//设置一个箭头光标
	//WindowsClass.hIcon = nullptr; //应用程序放在磁盘上显示的图标
	//WindowsClass.hIconSm = NULL;//应用程序显示在左上角的图标
	//WindowsClass.hInstance = InParameters.HInstance; //窗口实例
	//WindowsClass.lpszClassName = L"RenzhaiEngine";//窗口名字
	//WindowsClass.lpszMenuName = nullptr;//
	//WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//怎么绘制窗口 垂直和水平重绘
	//WindowsClass.lpfnWndProc = EngineWindowProc;//消息处理函数

	////注册窗口
	//ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	//if (!RegisterAtom)
	//{
	//	Engine_Log_Error("Register windows class fail.");
	//	MessageBox(NULL, L"Register windows class fail.", L"Error", MB_OK);
	//}

	//RECT Rect = {0,0,FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,FEngineRenderConfig::GetRenderConfig()->ScrrenHight};
	//
	////@rect 适口
	////WS_OVERLAPPEDWINDOW 适口风格
	////NULL 没有菜单
	//DWORD dwWndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	//AdjustWindowRect(&Rect, dwWndStyle, FALSE);
	////AdjustWindowRect(&Rect,WS_OVERLAPPEDWINDOW, NULL);

	//int WindowWidth = Rect.right - Rect.left;
	//int WindowHight = Rect.bottom - Rect.top;

	//MianWindowsHandle = CreateWindowEx(
	//	NULL,//窗口额外的风格
	//	L"RenZhaiEngine", // 窗口名称
	//	L"RENZHAI Engine",//会显示在窗口的标题栏上去
	//	dwWndStyle, //窗口风格
	//	0,0,//窗口的坐标
	//	WindowWidth, WindowHight,//
	//	NULL, //副窗口句柄
	//	nullptr, //菜单句柄
	//	InParameters.HInstance,//窗口实例
	//	NULL);//
	//if (!MianWindowsHandle)
	//{
	//	Engine_Log_Error("CreateWindow Failed..");
	//	MessageBox(0, L"CreateWindow Failed.", 0, 0);
	//	return false;
	//}
	////GameCore::g_hWnd = MianWindowsHandle;
	//////显示窗口
	////ShowWindow(MianWindowsHandle, SW_SHOW);

	//////窗口是脏的，刷新一下
	////UpdateWindow(MianWindowsHandle);

	//Engine_Log("InitWindows complete.");

	return true;
}

#endif