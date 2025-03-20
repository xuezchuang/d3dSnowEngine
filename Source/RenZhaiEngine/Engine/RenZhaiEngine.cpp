#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"
#include "Core/AssetPackageManage.h"
#include "CoreObject/ClassObject.h"
#include "CodeReflection/ClassManage.h"
#include "Timer.h"
#include "GameCore.h"
#include "Platform/Windows/WindowsEngine.h"
#include "Rendering/Core/DirectX/RenderingPipeline/UI/IMGUIPipeline.h"
#include "Camera.h"
#include "CameraController.h"
#include "../Model/Model.h"
#include "ShadowCamera.h"
#include "MotionBlur.h"
#include "TemporalEffects.h"
#include "FXAA.h"
#include "PostEffects.h"
#include "SSAO.h"
#include "../Model/Renderer.h"
#include "../Model/ModelLoader.h"
#include "BufferManager.h"
#include "ParticleEffectManager.h"
#include "Display.h"
#include "../Model/SponzaRenderer.h"
//#include "include/Game.h"
//#include "Core/mEngine.h"
//#include "include/ApplicationWin32.h"

int Init(CEngine* InEngine,HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	FWinMainCommandParameters WinMainParameters(hInstance, prevInstance, cmdLine, showCmd);

	//��������Class Ϊ����ͼ����
	for (int i = 0; i < FClassManage::GetNum(); i++)
	{
		if (CClassObject* InObjectClass = FClassManage::GetClassByIndex(i))
		{
			InObjectClass->GetDefaultObject();
		}
	}

	int ReturnValue = InEngine->PreInit(WinMainParameters);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Init(WinMainParameters);
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error, please check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	//��ʼ�����ǵ��ʲ�
	FAssetPackageManage::Get()->Init();

	ReturnValue = InEngine->PostInit();
	if(ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error, please check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

void Tick(CEngine* InEngine, float DeltaTime)
{
	FAssetPackageManage::Tick(DeltaTime);

	InEngine->Tick(DeltaTime);
}

int Exit(CEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

CEngine* Engine = NULL;
std::unique_ptr<FTimer> Timer = std::make_unique<FTimer>();

using namespace GameCore;
using namespace Math;
using namespace Graphics;
using namespace std;
using Renderer::MeshSorter;
ExpVar g_SunLightIntensity("Viewer/Lighting/Sun Light Intensity", 4.0f, 0.0f, 16.0f, 0.1f);
NumVar g_SunOrientation("Viewer/Lighting/Sun Orientation", -0.5f, -100.0f, 100.0f, 0.1f);
NumVar g_SunInclination("Viewer/Lighting/Sun Inclination", 0.75f, 0.0f, 1.0f, 0.01f);

void ChangeIBLSet(EngineVar::ActionType);
void ChangeIBLBias(EngineVar::ActionType);

DynamicEnumVar g_IBLSet("Viewer/Lighting/Environment", ChangeIBLSet);
std::vector<std::pair<TextureRef, TextureRef>> g_IBLTextures;
NumVar g_IBLBias("Viewer/Lighting/Gloss Reduction", 2.0f, 0.0f, 10.0f, 1.0f, ChangeIBLBias);

void ChangeIBLSet(EngineVar::ActionType)
{
	int setIdx = g_IBLSet - 1;
	if (setIdx < 0)
	{
		Renderer::SetIBLTextures(nullptr, nullptr);
	}
	else
	{
		auto texturePair = g_IBLTextures[setIdx];
		Renderer::SetIBLTextures(texturePair.first, texturePair.second);
	}
}

void ChangeIBLBias(EngineVar::ActionType)
{
	Renderer::SetIBLBias(g_IBLBias);
}

#include <direct.h> // for _getcwd() to check data root path

void LoadIBLTextures()
{
	char CWD[256];
	_getcwd(CWD, 256);

	Utility::Printf("Loading IBL environment maps\n");

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(L"Textures/*_diffuseIBL.dds", &ffd);

	g_IBLSet.AddEnum(L"None");

	if (hFind != INVALID_HANDLE_VALUE) do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		std::wstring diffuseFile = ffd.cFileName;
		std::wstring baseFile = diffuseFile;
		baseFile.resize(baseFile.rfind(L"_diffuseIBL.dds"));
		std::wstring specularFile = baseFile + L"_specularIBL.dds";

		TextureRef diffuseTex = TextureManager::LoadDDSFromFile(L"Textures/" + diffuseFile);
		if (diffuseTex.IsValid())
		{
			TextureRef specularTex = TextureManager::LoadDDSFromFile(L"Textures/" + specularFile);
			/*CommandContext::InitializeTexture�б���ʹ��finish(true)��ԭ�����,����Ҳ���specularFile�����Դ,�����diffuseTex������.��ʱ,�ϴ��ѵ���Դ������diffuseTex���gpu��Դ����,��Ҫ�ȴ���ɲſ�������*/
			if (specularTex.IsValid())
			{
				g_IBLSet.AddEnum(baseFile);
				g_IBLTextures.push_back(std::make_pair(diffuseTex, specularTex));
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);

	Utility::Printf("Found %u IBL environment map sets\n", g_IBLTextures.size());

	if (g_IBLTextures.size() > 0)
		g_IBLSet.Increment();
}

class ModelViewer : public GameCore::IGameApp
{
public:

	ModelViewer(void) {}

	virtual void Startup(void) override
	{
		if (1)
		{
			Engine = FEngineFactory::Instance();
			if (m_WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine))
			{
				m_WindowsEngine->MianWindowsHandle = GetHwnd();
			}
			Init(Engine, WinMainParameters.HInstance, WinMainParameters.PrevInstance, WinMainParameters.CmdLine, WinMainParameters.ShowCmd);
			UIPipeline.Init();
			return;
		}


		MotionBlur::Enable = false;// true;
		TemporalEffects::EnableTAA = true;
		FXAA::Enable = false;
		PostEffects::EnableHDR = true;
		PostEffects::EnableAdaptation = true;
		SSAO::Enable = true;

		Renderer::Initialize();

		LoadIBLTextures();

		std::wstring gltfFileName;

		bool forceRebuild = false;
		uint32_t rebuildValue;
		if (CommandLineArgs::GetInteger(L"rebuild", rebuildValue))
			forceRebuild = rebuildValue != 0;

		if (CommandLineArgs::GetString(L"model", gltfFileName) == false)
		{
#ifdef LEGACY_RENDERER
			Sponza::Startup(m_Camera);
#else
			m_ModelInst = Renderer::LoadModel(L"Sponza/PBR/sponza2.gltf", forceRebuild);
			m_ModelInst.Resize(100.0f * m_ModelInst.GetRadius());
			OrientedBox obb = m_ModelInst.GetBoundingBox();
			float modelRadius = Length(obb.GetDimensions()) * 0.5f;
			const Vector3 eye = obb.GetCenter() + Vector3(modelRadius * 0.5f, 0.0f, 0.0f);
			m_Camera.SetEyeAtUp(eye, Vector3(kZero), Vector3(kYUnitVector));
#endif
		}
		else
		{
			m_ModelInst = Renderer::LoadModel(gltfFileName, forceRebuild);
			m_ModelInst.LoopAllAnimations();
			m_ModelInst.Resize(10.0f);

			MotionBlur::Enable = false;
		}

		m_Camera.SetZRange(1.0f, 10000.0f);
		if (gltfFileName.size() == 0)
			m_CameraController.reset(new FlyingFPSCamera(m_Camera, Vector3(kYUnitVector)));
		else
			m_CameraController.reset(new OrbitCamera(m_Camera, m_ModelInst.GetBoundingSphere(), Vector3(kYUnitVector)));
	}
	virtual void Cleanup(void) override
	{
		UIPipeline.Exit();
		Exit(m_WindowsEngine);
	}

	virtual void Update(float deltaT) override
	{
		//ScopedTimer _prof(L"Update State");

		////if (GameInput::IsFirstPressed(GameInput::kLShoulder))
		////	DebugZoom.Decrement();
		////else if (GameInput::IsFirstPressed(GameInput::kRShoulder))
		////	DebugZoom.Increment();

		//m_CameraController->Update(deltaT);

		//GraphicsContext& gfxContext = GraphicsContext::Begin(L"Scene Update");

		//m_ModelInst.Update(gfxContext, deltaT);

		//gfxContext.Finish();

		//// We use viewport offsets to jitter sample positions from frame to frame (for TAA.)
		//// D3D has a design quirk with fractional offsets such that the implicit scissor
		//// region of a viewport is floor(TopLeftXY) and floor(TopLeftXY + WidthHeight), so
		//// having a negative fractional top left, e.g. (-0.25, -0.25) would also shift the
		//// BottomRight corner up by a whole integer.  One solution is to pad your viewport
		//// dimensions with an extra pixel.  My solution is to only use positive fractional offsets,
		//// but that means that the average sample position is +0.5, which I use when I disable
		//// temporal AA.
		//TemporalEffects::GetJitterOffset(m_MainViewport.TopLeftX, m_MainViewport.TopLeftY);

		//m_MainViewport.Width = (float)g_SceneColorBuffer.GetWidth();
		//m_MainViewport.Height = (float)g_SceneColorBuffer.GetHeight();
		//m_MainViewport.MinDepth = 0.0f;
		//m_MainViewport.MaxDepth = 1.0f;

		//m_MainScissor.left = 0;
		//m_MainScissor.top = 0;
		//m_MainScissor.right = (LONG)g_SceneColorBuffer.GetWidth();
		//m_MainScissor.bottom = (LONG)g_SceneColorBuffer.GetHeight();

		m_DeltaTime = deltaT;
		Tick(m_WindowsEngine, deltaT);
	}
	virtual void RenderScene(void) override
	{
		m_WindowsEngine->RenderScene(m_DeltaTime);

		//{
		//	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Render UI");
		//	m_WindowsEngine->RenderScene(gfxContext, m_DeltaTime);
		//	gfxContext.Finish();
		//}
		 
		//GraphicsContext& gfxContext = GraphicsContext::Begin(L"Scene Render");

		//uint32_t FrameIndex = TemporalEffects::GetFrameIndexMod2();
		//const D3D12_VIEWPORT& viewport = m_MainViewport;
		//const D3D12_RECT& scissor = m_MainScissor;

		//ParticleEffectManager::Update(gfxContext.GetComputeContext(), Graphics::GetFrameTime());

		//if (m_ModelInst.IsNull())
		//{
		//	Sponza::RenderScene(gfxContext, m_Camera, viewport, scissor, false, true);
		//}
		//else
		//{
		//	// Update global constants
		//	float costheta = cosf(g_SunOrientation);
		//	float sintheta = sinf(g_SunOrientation);
		//	float cosphi = cosf(g_SunInclination * 3.14159f * 0.5f);
		//	float sinphi = sinf(g_SunInclination * 3.14159f * 0.5f);

		//	Vector3 SunDirection = Normalize(Vector3(costheta * cosphi, sinphi, sintheta * cosphi));
		//	Vector3 ShadowBounds = Vector3(m_ModelInst.GetRadius());
		//	//m_SunShadowCamera.UpdateMatrix(-SunDirection, m_ModelInst.GetCenter(), ShadowBounds,
		//	m_SunShadowCamera.UpdateMatrix(-SunDirection, Vector3(0, -500.0f, 0), Vector3(5000, 3000, 3000),
		//		(uint32_t)g_ShadowBuffer.GetWidth(), (uint32_t)g_ShadowBuffer.GetHeight(), 16);

		//	GlobalConstants globals;
		//	globals.ViewProjMatrix = m_Camera.GetViewProjMatrix();
		//	globals.SunShadowMatrix = m_SunShadowCamera.GetShadowMatrix();
		//	globals.CameraPos = m_Camera.GetPosition();
		//	globals.SunDirection = SunDirection;
		//	globals.SunIntensity = Vector3(Scalar(g_SunLightIntensity));

		//	// Begin rendering depth
		//	gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
		//	gfxContext.ClearDepth(g_SceneDepthBuffer);

		//	MeshSorter sorter(MeshSorter::kDefault);
		//	sorter.SetCamera(m_Camera);
		//	sorter.SetViewport(viewport);
		//	sorter.SetScissor(scissor);
		//	sorter.SetDepthStencilTarget(g_SceneDepthBuffer);
		//	sorter.AddRenderTarget(g_SceneColorBuffer);

		//	m_ModelInst.Render(sorter);

		//	sorter.Sort();

		//	{
		//		ScopedTimer _prof(L"Depth Pre-Pass", gfxContext);
		//		sorter.RenderMeshes(MeshSorter::kZPass, gfxContext, globals);
		//	}

		//	SSAO::Render(gfxContext, m_Camera);

		//	if (!SSAO::DebugDraw)
		//	{
		//		ScopedTimer _outerprof(L"Main Render", gfxContext);

		//		{
		//			ScopedTimer _prof(L"Sun Shadow Map", gfxContext);

		//			MeshSorter shadowSorter(MeshSorter::kShadows);
		//			shadowSorter.SetCamera(m_SunShadowCamera);
		//			shadowSorter.SetDepthStencilTarget(g_ShadowBuffer);

		//			m_ModelInst.Render(shadowSorter);

		//			shadowSorter.Sort();
		//			shadowSorter.RenderMeshes(MeshSorter::kZPass, gfxContext, globals);
		//		}

		//		gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
		//		gfxContext.ClearColor(g_SceneColorBuffer);

		//		{
		//			ScopedTimer _prof(L"Render Color", gfxContext);

		//			gfxContext.TransitionResource(g_SSAOFullScreen, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		//			gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ);
		//			gfxContext.SetRenderTarget(g_SceneColorBuffer.GetRTV(), g_SceneDepthBuffer.GetDSV_DepthReadOnly());
		//			gfxContext.SetViewportAndScissor(viewport, scissor);

		//			sorter.RenderMeshes(MeshSorter::kOpaque, gfxContext, globals);
		//		}

		//		Renderer::DrawSkybox(gfxContext, m_Camera, viewport, scissor);

		//		sorter.RenderMeshes(MeshSorter::kTransparent, gfxContext, globals);
		//	}
		//}

		//// Some systems generate a per-pixel velocity buffer to better track dynamic and skinned meshes.  Everything
		//// is static in our scene, so we generate velocity from camera motion and the depth buffer.  A velocity buffer
		//// is necessary for all temporal effects (and motion blur).
		//MotionBlur::GenerateCameraVelocityBuffer(gfxContext, m_Camera, true);

		//TemporalEffects::ResolveImage(gfxContext);

		//ParticleEffectManager::Render(gfxContext, m_Camera, g_SceneColorBuffer, g_SceneDepthBuffer, g_LinearDepth[FrameIndex]);

		//// Until I work out how to couple these two, it's "either-or".
		////if (DepthOfField::Enable)
		////    DepthOfField::Render(gfxContext, m_Camera.GetNearClip(), m_Camera.GetFarClip());
		////else
		////    MotionBlur::RenderObjectBlur(gfxContext, g_VelocityBuffer);

		//gfxContext.Finish();
	}

	FWinMainCommandParameters WinMainParameters;

	virtual void RenderUI(class GraphicsContext& context) override
	{
		UIPipeline.Draw(context, m_DeltaTime);
	}

private:
	CWindowsEngine* m_WindowsEngine;
	FIMGUIPipeline UIPipeline;
	float m_DeltaTime;

	Camera m_Camera;
	unique_ptr<CameraController> m_CameraController;

	D3D12_VIEWPORT m_MainViewport;
	D3D12_RECT m_MainScissor;

	ModelInstance m_ModelInst;
	ShadowCamera m_SunShadowCamera;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)
{
	ModelViewer model;
	model.WinMainParameters = FWinMainCommandParameters(hInstance, prevInstance, cmdLine, showCmd);
	return GameCore::RunApplication(model, L"ModelViewer", hInstance, showCmd);
	//int ReturnValue = 0;

	//Engine = FEngineFactory::Instance();
	//if (Engine)
	//{
	//	//��ʼ��
	//	Init(Engine, hInstance, prevInstance, cmdLine, showCmd);

	//	MSG EngineMsg = { 0 };

	//	Timer->Reset();

	//	//��Ⱦ��ͼ
	//	while (EngineMsg.message != WM_QUIT)
	//	{
	//		//PM_NOREMOVE ��Ϣ���Ӷ����������
	//		//PM_REMOVE   ��Ϣ�Ӷ����������
	//		//PM_NOYIELD  �˱�־ʹϵͳ���ͷŵȴ����ó�����е��߳�
	//		// 
	//		//PM_QS_INPUT �������ͼ�����Ϣ��
	//		//PM_QS_PAINT ����ͼ��Ϣ��
	//		//PM_QS_POSTMESSAGE �������б����͵���Ϣ��������ʱ�����ȼ���
	//		//PM_QS_SENDMESSAGE �������з�����Ϣ��
	//		if (PeekMessage(&EngineMsg,0,0,0, PM_REMOVE))
	//		{
	//			TranslateMessage(&EngineMsg);
	//			DispatchMessage(&EngineMsg);
	//		}
	//		else
	//		{
	//			Timer->Tick();

	//			Tick(Engine,Timer->GetDeltaTime());

	//			//ģ����Թ̶�����
	//			//Sleep(1);
	//		}
	//	}

	//	//�˳�
	//	ReturnValue = Exit(Engine);
	//	//delete Engine;
	//}
	//else
	//{
	//	ReturnValue = 1;
	//}

	//Engine_Log("[%i]The engine has exited.", ReturnValue);
	//return ReturnValue;

	////GameDesc Desc;
	////Desc.Caption = L"Test";
	////mEngine tutorial(Desc);
	////ApplicationWin32::Get().Run(&tutorial);
	//CoUninitialize();
}