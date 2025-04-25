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
	////����Ӧ��Ļ��С��
	OnResetSizeDelegate.AddFunction(this, &CWindowsEngine::OnResetSize);
	ActiveDelegate.AddFunction(this, &CWindowsEngine::OnActive);

	InitPath();

	//��־ϵͳ��ʼ��
	std::string LogPath = 
		FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineLogsPath());

	init_log_system(LogPath.c_str());
	Engine_Log("Log Init.");

	//��������

	Engine_Log("Engine pre initialization complete.");

	//��Ⱦ�����ʼ��
	RenderingEngine->PreInit(InParameters);

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	InitWindows(InParameters);

	//RenderingEngine->SetMianWindowsHandle(MianWindowsHandle);

	RenderingEngine->Init(InParameters);

	//ע���Ӧ��world
	BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(, this);
	World = CreateObject<CWorld>(Param, new CWorld());
	RenderingEngine->World = World;

	Engine_Log("Engine initialization complete.");

	////��ʾ����
	//ShowWindow(MianWindowsHandle, SW_SHOW);

	////��������ģ�ˢ��һ��
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
		//����·��
		create_file_directory(LogPath.c_str());
	};

	//��־ϵͳ��ʼ��
	CreateFileDirectory(FEnginePathHelper::GetEngineLogsPath());

	//Content
	CreateFileDirectory(FEnginePathHelper::GetEngineContentPath());

	//�������·��
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
	////ע�ᴰ��
	//WNDCLASSEX WindowsClass;
	//WindowsClass.cbSize = sizeof(WNDCLASSEX);//�ö���ʵ��ռ�ö���ڴ�
	//WindowsClass.cbClsExtra = 0;//�Ƿ���Ҫ����ռ�
	//WindowsClass.cbWndExtra = 0;//�Ƿ���Ҫ�����ڴ�
	//WindowsClass.hbrBackground = nullptr;//����������ľ���GDI����
	//WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//����һ����ͷ���
	//WindowsClass.hIcon = nullptr; //Ӧ�ó�����ڴ�������ʾ��ͼ��
	//WindowsClass.hIconSm = NULL;//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	//WindowsClass.hInstance = InParameters.HInstance; //����ʵ��
	//WindowsClass.lpszClassName = L"RenzhaiEngine";//��������
	//WindowsClass.lpszMenuName = nullptr;//
	//WindowsClass.style = CS_VREDRAW | CS_HREDRAW;//��ô���ƴ��� ��ֱ��ˮƽ�ػ�
	//WindowsClass.lpfnWndProc = EngineWindowProc;//��Ϣ������

	////ע�ᴰ��
	//ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	//if (!RegisterAtom)
	//{
	//	Engine_Log_Error("Register windows class fail.");
	//	MessageBox(NULL, L"Register windows class fail.", L"Error", MB_OK);
	//}

	//RECT Rect = {0,0,FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,FEngineRenderConfig::GetRenderConfig()->ScrrenHight};
	//
	////@rect �ʿ�
	////WS_OVERLAPPEDWINDOW �ʿڷ��
	////NULL û�в˵�
	//DWORD dwWndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	//AdjustWindowRect(&Rect, dwWndStyle, FALSE);
	////AdjustWindowRect(&Rect,WS_OVERLAPPEDWINDOW, NULL);

	//int WindowWidth = Rect.right - Rect.left;
	//int WindowHight = Rect.bottom - Rect.top;

	//MianWindowsHandle = CreateWindowEx(
	//	NULL,//���ڶ���ķ��
	//	L"RenZhaiEngine", // ��������
	//	L"RENZHAI Engine",//����ʾ�ڴ��ڵı�������ȥ
	//	dwWndStyle, //���ڷ��
	//	0,0,//���ڵ�����
	//	WindowWidth, WindowHight,//
	//	NULL, //�����ھ��
	//	nullptr, //�˵����
	//	InParameters.HInstance,//����ʵ��
	//	NULL);//
	//if (!MianWindowsHandle)
	//{
	//	Engine_Log_Error("CreateWindow Failed..");
	//	MessageBox(0, L"CreateWindow Failed.", 0, 0);
	//	return false;
	//}
	////GameCore::g_hWnd = MianWindowsHandle;
	//////��ʾ����
	////ShowWindow(MianWindowsHandle, SW_SHOW);

	//////��������ģ�ˢ��һ��
	////UpdateWindow(MianWindowsHandle);

	//Engine_Log("InitWindows complete.");

	return true;
}

#endif