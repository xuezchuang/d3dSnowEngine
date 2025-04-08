#include "ProfilingEditor.h"

FProfilingEditor::FProfilingEditor()
{
}

void FProfilingEditor::BuildEditor()
{
	//AssetEditor->BuildEditor();
	//FileEditor->BuildEditor();
	//AssetMenuEditor->BuildEditor();
}

void FProfilingEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	//static bool bOpenAsset = false;
	ImGui::Begin("Profiling Editor");

	float cpuTime = GetTotalCpuTime();
	float gpuTime = GetTotalGpuTime();
	uint32_t frameRate = (uint32_t)(1.0f / GetFrameDelta()+0.5f);


	bool bNodeOpen = ImGui::TreeNode("Brief Info");
	if (bNodeOpen)
	{
		ImGui::Text("CPU %7.3f ms", cpuTime);
		ImGui::Text("CPU %7.3f ms", gpuTime);
		ImGui::Text("frame %3u Hz", frameRate);
		ImGui::TreePop();
	}

	bNodeOpen = ImGui::TreeNode("Render Info");
	if (bNodeOpen)
	{
		int nTriangleCount = GetRenderTriangleCount();
		ImGui::Text("RenderTriangleCount : %d", nTriangleCount);
		ImGui::TreePop();
	}

	bNodeOpen = ImGui::TreeNode("Detail Info");
	if (bNodeOpen)
	{
		aProfilingTime.clear();
		getAllProfilingTime(aProfilingTime);

		std::function<void(const T_PROFILING&)> ShowTiming = [&](const T_PROFILING& tProfiling)
			{
				ImGui::Text("%-20s CPU:%6.3f		GPU:%6.3f", tProfiling.m_Name.c_str(), tProfiling.CPUTime, tProfiling.GPUTime);
				if (!tProfiling.child.empty())
				{
					bool bChildNodeOpen = ImGui::TreeNode(tProfiling.m_Name.c_str());
					if (bChildNodeOpen)
					{
						for (size_t i = 0; i < tProfiling.child.size(); i++)
						{
							ShowTiming(tProfiling.child[i]);
						}
						ImGui::TreePop();
					}
				}
			};
		for (size_t i = 0; i < aProfilingTime.size(); i++)
		{
			ShowTiming(aProfilingTime[i]);
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void FProfilingEditor::ExitEditor()
{
	//AssetEditor->ExitEditor();
	//FileEditor->ExitEditor();
	//AssetMenuEditor->ExitEditor();
}