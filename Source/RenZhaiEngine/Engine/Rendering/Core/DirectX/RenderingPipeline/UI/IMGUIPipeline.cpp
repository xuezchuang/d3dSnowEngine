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
//	//�汾���
//	IMGUI_CHECKVERSION();
//
//	//��Ҫ����ImGUI���ã�Ini����صĲ���
//	ImGui::CreateContext();
//
//	//Ԫ������ɫ��ʼ��
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
//	//win32��ʼ�� ���ض�Ӧ��DLL�ͳ�ʼ��ʱ���
//	ImGui_ImplWin32_Init(GetMianWindowsHandle());
//
//	//��ʼ����̨���� ע���Ӧ��������֡��
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
	////����PSO(��ǩ�� shader ���벼��) ��������
	//ImGui_ImplDX12_NewFrame();

	////��ʼ�� �ӿ� �ֱ� ����ƶ� ֡ʱ��
	//ImGui_ImplWin32_NewFrame();

	//ImGui::NewFrame();

	////����
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
