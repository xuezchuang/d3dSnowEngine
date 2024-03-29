#include "AssetMenuEditor.h"
#include "CodeReflection/ClassManage.h"
#include "../../Core/AssetEditorCommon.h"
#include "Core/AssetAssistLibrary.h"

char CreateAssetNameText[128] = { 0 };

void FAssetMenuEditor::BuildEditor()
{

}

void FAssetMenuEditor::DrawEditor(float DeltaTime)
{
	ImGui::InvisibleButton("AssetMenu", ImGui::GetContentRegionAvail(), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	ImGui::OpenPopupOnItemClick("AssetMenu", ImGuiPopupFlags_MouseButtonRight);

	if (ImGui::BeginPopup("AssetMenu"))
	{
		if (ImGui::Button("Create Blueprint"))
		{
			//ImGui::CloseCurrentPopup();
			ImGui::OpenPopup("Create Blueprint");
		}

		if (ImGui::Button("Create Level"))
		{
			//ImGui::CloseCurrentPopup();
			ImGui::OpenPopup("Create Level");
		}

		//Modal
		if (ImGui::BeginPopupModal("Create Blueprint", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Hello ~~~~~");

			static ImGuiTextFilter ClassFilter;
			ClassFilter.Draw();

			ImGui::Text("All-Class:");
			if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				const char* AllClassID = FClassManage::GetData();

				for (int i = 0; i < FClassManage::GetNum(); i++)
				{
					FClassID* CurrClassID = (FClassID*)&AllClassID[i * sizeof(FClassID)];
					if (ClassFilter.PassFilter(CurrClassID->ClassName))
					{
						const bool bSelected = (ClassSelectedIndex == i);
						if (ImGui::Selectable(CurrClassID->ClassName, bSelected))
						{
							ClassSelectedIndex = i;
							SelectedClassName = CurrClassID->ClassName;
						}

						if (bSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}

				ImGui::EndListBox();
			}

			if (ImGui::InputText("Input new blueprint asset name", CreateAssetNameText,128))
			{

			}

			ImVec2 ButtonSize(ImGui::GetFontSize() * 7.f,0.f);
			if (ImGui::Button("Create", ButtonSize))
			{
				if (strlen(CreateAssetNameText) > 4)
				{
					if (!CurrentSelectAssetPath.expired())
					{
						auto AssetPackage = AssetAssistLibrary::CreateAssetPackage(
							CurrentSelectAssetPath.lock()->FileDirectory, 
							CreateAssetNameText, 
							ClassSelectedIndex);

						AssetPackage->Save();

						ClassSelectedIndex = 0;
						SelectedClassName = "";

						memset(CreateAssetNameText, 0, 128);
						ImGui::CloseCurrentPopup();
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ButtonSize))
			{
				memset(CreateAssetNameText, 0, 128);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Create Level", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::InputText("Input new blueprint asset name", CreateAssetNameText, 128))
			{

			}

			ImVec2 ButtonSize(ImGui::GetFontSize() * 7.f, 0.f);
			if (ImGui::Button("Create", ButtonSize))
			{
				if (strlen(CreateAssetNameText) > 4)
				{
					if (!CurrentSelectAssetPath.expired())
					{
						auto AssetPackage = AssetAssistLibrary::CreateAssetPackage(
							CurrentSelectAssetPath.lock()->FileDirectory,
							CreateAssetNameText,
							ClassSelectedIndex);

						AssetPackage->Save();

						ClassSelectedIndex = 0;
						SelectedClassName = "";

						memset(CreateAssetNameText, 0, 128);
						ImGui::CloseCurrentPopup();
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ButtonSize))
			{
				memset(CreateAssetNameText, 0, 128);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("Create Blueprint2"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void FAssetMenuEditor::ExitEditor()
{

}