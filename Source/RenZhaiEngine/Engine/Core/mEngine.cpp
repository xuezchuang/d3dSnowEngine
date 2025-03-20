//#include "../EngineMinimal.h"
#include "mEngine.h"
////#include "../DirectX12Lib/include/ImguiManager.h"
////#include "../DirectX12Lib/include/RenderWindow.h"
//#include "../Platform/Windows/WindowsEngine.h"
//#include "../EditorEngine/EditorEngine.h"
//#include "../EngineFactory.h"
//#include "../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
//#include <CodeReflection/ClassManage.h>
//#include <CoreObject/ClassObject.h>
//#include "../Common.h"
//#include "../SamplerManager.h"
//#include "../CommandListManager.h"
//#include "../Mesh/Core/MeshManage.h"
//#include "../BufferManager.h"
//
//
//extern FCommandListManager g_CommandListManager;
//CEngine* Engine = NULL;
//
//void mEngine::OnStartup()
//{
//	tStart = std::chrono::high_resolution_clock::now();
//
//	Engine = FEngineFactory::Instance();
//	for (int i = 0; i < FClassManage::GetNum(); i++)
//	{
//		if (CClassObject* InObjectClass = FClassManage::GetClassByIndex(i))
//		{
//			InObjectClass->GetDefaultObject();
//		}
//	}
//	Engine->PreExit();
//	Engine->Init();
//	Engine->PostInit();
//
//#if EDITOR_ENGINE
//	GetEditorEngine()->BuildEditor();
//#endif
//	const static bool bViewFromGround = true;
//	if(bViewFromGround)
//	{
//		float CameraHeight = 1000.f;
//		m_Camera = FCamera(Vector3f(0.f, CameraHeight, 0.f), Vector3f(0.f, CameraHeight, 1.f), Vector3f(0.f, 1.f, 0.f));
//
//		const float FovVertical = 65.f * MATH_PI / 180;
//		m_Camera.SetPerspectiveParams(FovVertical, (float)GetDesc().Width / GetDesc().Height, 0.1f, 100.f);
//	}
//	else
//	{
//		const static float AtmospherRadius = 6460e3;
//		m_Camera = FCamera(Vector3f(0.f, 0.f, -AtmospherRadius * 3), Vector3f(0.f, 0, 0.f), Vector3f(0.f, 1.f, 0.f));
//
//		const float FovVertical = MATH_PI / 4.f;
//		m_Camera.SetPerspectiveParams(FovVertical, (float)GetDesc().Width / GetDesc().Height, 0.1f, 100.f);
//	}
//	m_Atmospheric.OnStartup();
//}
//
//void mEngine::OnUpdate()
//{
//	tEnd = std::chrono::high_resolution_clock::now();
//	//int curSizeX, curSizeY;
//	//GameInput::GetSize(curSizeX, curSizeY);
//	//if(m_SizeX != curSizeX || m_SizeY != curSizeY)
//	//{
//	//	Initialize()
//	//}
//	//m_SizeX = curSizeX;
//	//m_SizeY = curSizeY;
//
//	float DeltaTime = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
//
//	//GetDirectRenderingEngine()->UpdateCalculations(DeltaTime, ViewportInfo);
//	//GetDirectRenderingEngine()->Tick(DeltaTime);
//	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
//	{
//		InEngine->Tick(DeltaTime);
//	}
//
//	m_Atmospheric.OnUpdate();
//}
//
//void mEngine::OnRender()
//{
//	GraphicsContext& CommandContext = GraphicsContext::Begin(D3D12_COMMAND_LIST_TYPE_DIRECT, L"3D Queue");
//	
//	RenderWindow& renderWindow = RenderWindow::Get();
//	FColorBuffer& BackBuffer = renderWindow.GetBackBuffer();
//	
//
//
//	CommandContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
//	CommandContext.TransitionResource(BufferManager::g_SceneDepthZ, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
//	CommandContext.SetRenderTargets(1, &BackBuffer.GetRTV(), BufferManager::g_SceneDepthZ.GetDSV());
//	CommandContext.ClearColor(BackBuffer);
//	CommandContext.ClearDepth(BufferManager::g_SceneDepthZ);
//
//	CommandContext.SetViewportAndScissor(0, 0, m_GameDesc.Width, m_GameDesc.Height);
//
//	float delta = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
//	GetFRenderingPipeline()->PreDraw(CommandContext, delta);
	//GetFRenderingPipeline()->Draw(CommandContext, delta);
//
//	CommandContext.FlushResourceBarriers();
//
//	//OnTest(CommandContext);
//
//	//m_Atmospheric.OnRender(CommandContext);
//
//	OnGUI(CommandContext);
//	CommandContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_PRESENT);
//	CommandContext.Finish(true);
//
//	RenderWindow::Get().Present();
//}
//
//void mEngine::OnGUI(GraphicsContext& CommandContext)
//{
//	ImguiManager::Get().NewFrame();
//
//	float delta = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
//
//	//GetEngine()->Dra
//#if EDITOR_ENGINE
//	GetEditorEngine()->DrawEditor(delta);
//#endif
//	ImguiManager::Get().Render(CommandContext, RenderWindow::Get());
//}
//
//void mEngine::OnTest(GraphicsContext& CommandContext)
//{
//
//}
//
//CDirectXRenderingEngine* mEngine::GetDirectRenderingEngine()const
//{
//	if(CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
//	{
//		return InEngine->RenderingEngine;
//	}
//	return nullptr;
//}
//
//FRenderingPipeline* mEngine::GetFRenderingPipeline() const
//{
//	if(CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
//	{
//		return InEngine->GetMeshManage()->GetRenderingPipeline();
//	}
//	return nullptr;
//}
//
//#if EDITOR_ENGINE
//CEditorEngine* mEngine::GetEditorEngine() const
//{
//	if(CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
//	{
//		return InEngine->EditorEngine;
//	}
//	return nullptr;
//}
//#endif
//
//void Atmospheric::OnStartup()
//{
//	SetupCameraLight();
//	SetupMesh();
//	SetupShaders();
//	SetupPipelineState();
//}
//
//void Atmospheric::OnUpdate()
//{
//	m_Constants.ScreenParams = Vector4f(1.f * m_GameDesc->Width, 1.f * m_GameDesc->Height, 1.f / m_GameDesc->Width, 1.f / m_GameDesc->Height);
//	m_Constants.LightDirAndIntensity = Vector4f(m_DirectionLight.GetDirection(), m_DirectionLight.GetIntensity());
//	m_Constants.InvProjectionMatrix = m_Camera->GetProjectionMatrix().Inverse();
//	m_Constants.InvViewMatrix = m_Camera->GetViewMatrix().Inverse();
//}
//
//void Atmospheric::OnRender(GraphicsContext& CommandContext)
//{
//	if(m_bUseComputeShader)
//		ScatteringPassCS(CommandContext);
//	else
//		ScatteringPassPS(CommandContext);
//	PostProcess(CommandContext);
//}
//
//void Atmospheric::SetupCameraLight()
//{
//	// parameters are from "A Scalable and Production Ready Sky and Atmosphere Rendering Technique"
//	const static float EarthRadius = 6360e3;
//	const static float AtmospherRadius = 6460e3;
//
//	m_Constants.DensityScaleHeight = Vector2f(8000.0f, 1200.f);
//	m_Constants.AtmosphereRadius = AtmospherRadius;
//	m_Constants.RayleiCoef = Vector3f(5.802f, 13.558f, 33.1f) * 1e-6f;
//	m_Constants.MieG = 0.8f; //[-1, 1], from backward to forward
//	m_Constants.MieCoef = 21e-6f;
//
//	const static bool bViewFromGround = true;
//	if(bViewFromGround)
//	{
//		float theta = 30 * MATH_PI / 180.f;
//		float z = cos(theta);
//		float y = sin(theta);
//		m_DirectionLight.SetDirection(Vector3f(0.f, -y, -z));
//		m_DirectionLight.SetIntensity(10.f);
//
//		m_Constants.EarthCenterAndRadius = Vector4f(0.f, -EarthRadius, 0.f, EarthRadius);
//	}
//	else
//	{
//		float theta = 90 * MATH_PI / 180.f;
//		float z = cos(theta);
//		float y = sin(theta);
//		//m_DirectionLight.SetDirection(Vector3f(-1.f, 0.f, -0.3f));
//		m_DirectionLight.SetDirection(Vector3f(0.f, 0.f, 1.0f));
//		m_DirectionLight.SetIntensity(20.f);
//
//		m_Constants.EarthCenterAndRadius = Vector4f(0.f, 0.f, 0.f, EarthRadius);
//	}
//}
//
//void Atmospheric::SetupMesh()
//{
//
//}
//
//void Atmospheric::SetupShaders()
//{
//	m_ScatteringCS = D3D12RHI::Get().CreateShader(getpath(L"Resources/Shaders/AtmosphericScatteringCS.hlsl"), "cs_main", "cs_5_1");
//	m_ScatteringPS = D3D12RHI::Get().CreateShader(getpath(L"Resources/Shaders/AtmosphericScatteringPS.hlsl"), "ps_main", "ps_5_1");
//	m_ScreenQuadVS = D3D12RHI::Get().CreateShader(getpath(L"Resources/Shaders/PostProcess.hlsl"), "VS_ScreenQuad", "vs_5_1");
//	m_PostPS = D3D12RHI::Get().CreateShader(getpath(L"Resources/Shaders/PostProcess.hlsl"), "PS_Main", "ps_5_1");
//
//	
//}
//
//void Atmospheric::SetupPipelineState()
//{
//	m_ScatteringSignature.Reset(3, 0);
//	m_ScatteringSignature[0].InitAsBufferCBV(0, D3D12_SHADER_VISIBILITY_ALL); // compute shader need 'all'
//	m_ScatteringSignature[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1);
//	m_ScatteringSignature[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1);
//	m_ScatteringSignature.Finalize(L"Atmospheric Scattering RootSignature");
//
//	m_ScatteringCSPSO.SetRootSignature(m_ScatteringSignature);
//	m_ScatteringCSPSO.SetComputeShader(CD3DX12_SHADER_BYTECODE(m_ScatteringCS.Get()));
//	m_ScatteringCSPSO.Finalize();
//	m_ScatteringBuffer.Create(L"Scattering Buffer", m_GameDesc->Width, m_GameDesc->Height, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
//
//	FSamplerDesc DefaultSamplerDesc;
//	m_PostSignature.Reset(2, 1);
//	m_PostSignature[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1);
//	m_PostSignature[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1);
//	m_PostSignature.InitStaticSampler(0, DefaultSamplerDesc, D3D12_SHADER_VISIBILITY_PIXEL);
//	m_PostSignature.Finalize(L"Post Process RootSignature");
//
//	m_PostPSO.SetRootSignature(m_PostSignature);
//	m_PostPSO.SetRasterizerState(FPipelineState::RasterizerTwoSided);
//	m_PostPSO.SetBlendState(FPipelineState::BlendDisable);
//	m_PostPSO.SetDepthStencilState(FPipelineState::DepthStateDisabled);
//	// no need to set input layout
//	m_PostPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
//	m_PostPSO.SetRenderTargetFormats(1, &RenderWindow::Get().GetColorFormat(), DXGI_FORMAT_UNKNOWN);
//	m_PostPSO.SetVertexShader(CD3DX12_SHADER_BYTECODE(m_ScreenQuadVS.Get()));
//	m_PostPSO.SetPixelShader(CD3DX12_SHADER_BYTECODE(m_PostPS.Get()));
//	m_PostPSO.Finalize();
//
//	m_ScatteringPSPSO.SetRootSignature(m_ScatteringSignature);
//	m_ScatteringPSPSO.SetRasterizerState(FPipelineState::RasterizerTwoSided);
//	m_ScatteringPSPSO.SetBlendState(FPipelineState::BlendDisable);
//	m_ScatteringPSPSO.SetDepthStencilState(FPipelineState::DepthStateDisabled);
//	// no need to set input layout
//	m_ScatteringPSPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
//	m_ScatteringPSPSO.SetRenderTargetFormats(1, &RenderWindow::Get().GetColorFormat(), DXGI_FORMAT_UNKNOWN);
//	m_ScatteringPSPSO.SetVertexShader(CD3DX12_SHADER_BYTECODE(m_ScreenQuadVS.Get()));
//	m_ScatteringPSPSO.SetPixelShader(CD3DX12_SHADER_BYTECODE(m_ScatteringPS.Get()));
//	m_ScatteringPSPSO.Finalize();
//}
//
//void Atmospheric::ScatteringPassCS(GraphicsContext& GfxContext)
//{
//	GfxContext.TransitionResource(m_ScatteringBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, true);
//	g_CommandListManager.GetComputeQueue().WaitForFenceValue(GfxContext.Flush(true));
//
//	FComputeContext& CommandContext = FComputeContext::Begin(L"Compute Queue");
//
//	CommandContext.SetRootSignature(m_ScatteringSignature);
//	CommandContext.SetPipelineState(m_ScatteringCSPSO);
//
//
//	CommandContext.SetDynamicConstantBufferView(0, sizeof(m_Constants), &m_Constants);
//	CommandContext.SetDynamicDescriptor(2, 0, m_ScatteringBuffer.GetUAV());
//
//	uint32_t GroupCountX = (m_GameDesc->Width + 7) / 8;
//	uint32_t GroupCountY = (m_GameDesc->Height + 7) / 8;
//
//	CommandContext.Dispatch(GroupCountX, GroupCountY, 1);
//
//	CommandContext.Finish(false);
//}
//
//void Atmospheric::ScatteringPassPS(GraphicsContext& GfxContext)
//{
//	GfxContext.SetRootSignature(m_ScatteringSignature);
//	GfxContext.SetPipelineState(m_ScatteringPSPSO);
//	GfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	GfxContext.SetViewportAndScissor(0, 0, m_GameDesc->Width, m_GameDesc->Height);
//
//	GfxContext.TransitionResource(m_ScatteringBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
//
//	GfxContext.SetRenderTargets(1, &m_ScatteringBuffer.GetRTV());
//	GfxContext.ClearColor(m_ScatteringBuffer);
//
//	GfxContext.SetDynamicConstantBufferView(0, sizeof(m_Constants), &m_Constants);
//
//	GfxContext.Draw(3);
//}
//
//void Atmospheric::PostProcess(GraphicsContext& GfxContext)
//{
//	if(m_bUseComputeShader)
//	{
//		g_CommandListManager.GetGraphicsQueue().StallForProducer(g_CommandListManager.GetComputeQueue());
//	}
//	// Set necessary state.
//	GfxContext.SetRootSignature(m_PostSignature);
//	GfxContext.SetPipelineState(m_PostPSO);
//	GfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	GfxContext.SetViewportAndScissor(0, 0, m_GameDesc->Width, m_GameDesc->Height);
//
//	RenderWindow& renderWindow = RenderWindow::Get();
//	FColorBuffer& BackBuffer = renderWindow.GetBackBuffer();
//
//	// Indicate that the back buffer will be used as a render target.
//	GfxContext.TransitionResource(m_ScatteringBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
//	GfxContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
//
//	GfxContext.SetRenderTargets(1, &BackBuffer.GetRTV());
//
//	//GfxContext.ClearColor(BackBuffer);
//
//	GfxContext.SetDynamicDescriptor(0, 0, m_ScatteringBuffer.GetSRV());
//
//	// no need to set vertex buffer and index buffer
//	GfxContext.Draw(3);
//}
