#include "IMGUIPipeline.h"

#if EDITOR_ENGINE
#include "../../../../../../EditorEngine/EditorEngine.h"
#endif // EDITOR_ENGINE
//#include "include/imgui/imgui_impl_win32.h"

FIMGUIPipeline::FIMGUIPipeline()
{

}

void FIMGUIPipeline::Init()
{
//	//版本检测
//	IMGUI_CHECKVERSION();
//
//	//主要加载ImGUI配置（Ini）相关的操作
//	ImGui::CreateContext();
//
//	//元件的颜色初始化
//	ImGui::StyleColorsDark();
//
//	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
//	ImGuiStyle& style = ImGui::GetStyle();
//	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//	{
//		style.WindowRounding = 0.0f;
//		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//	}
//
//	//win32初始化 加载对应的DLL和初始化时间等
//	ImGui_ImplWin32_Init(GetMianWindowsHandle());
//
//	//初始化后台数据 注册对应的驱动和帧数
//	ImGui_ImplDX12_Init(
//		GetD3dDevice().Get(),1,
//		DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap,
//		g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
//		g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
//
//#if EDITOR_ENGINE
//	GetEditorEngine()->BuildEditor();
//#endif
}

void FIMGUIPipeline::Draw(float DeltaTime)
{
	////构建PSO(根签名 shader 输入布局) 构建字体
	//ImGui_ImplDX12_NewFrame();

	////初始化 视口 手柄 鼠标移动 帧时间
	//ImGui_ImplWin32_NewFrame();

	//ImGui::NewFrame();

	////绘制
	//Tick(DeltaTime);

	//ImGui::Render();

	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetGraphicsCommandList().Get());
}

void FIMGUIPipeline::Exit()
{
//#if EDITOR_ENGINE
//	GetEditorEngine()->ExitEditor();
//#endif
//
//	ImGui_ImplDX12_Shutdown();
//	ImGui_ImplWin32_Shutdown();
}

void FIMGUIPipeline::BuildDesciptorHeap()
{
	//D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	//HeapDesc.NumDescriptors = 1;
	//HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//HeapDesc.NodeMask = 0;

	//GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&g_pd3dSrvDescHeap));
}

void FIMGUIPipeline::Tick(float DeltaTime)
{
//	GetGraphicsCommandList()->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
//#if EDITOR_ENGINE
//	GetEditorEngine()->DrawEditor(DeltaTime);
//#endif
}
