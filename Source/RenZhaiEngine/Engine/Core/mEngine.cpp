#include "mEngine.h"
#include "../DirectX12Lib/include/ImguiManager.h"
#include "../DirectX12Lib/include/RenderWindow.h"


void mEngine::OnStartup()
{

}

void mEngine::OnUpdate()
{

}

void mEngine::OnRender()
{
	FCommandContext& CommandContext = FCommandContext::Begin(D3D12_COMMAND_LIST_TYPE_DIRECT, L"3D Queue");
}

void mEngine::OnGUI(FCommandContext& CommandContext)
{

	ImguiManager::Get().NewFrame();

	ImGui::Begin("config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::BeginGroup();
	//ImGui::Checkbox("Use Compute Shader", &m_bUseComputeShader);
	ImGui::EndGroup();

	ImGui::End();

	ImguiManager::Get().Render(CommandContext, RenderWindow::Get());
}

