#include "FolderMenuEditor.h"
#include "CodeReflection/ClassManage.h"
#include "../../Core/AssetEditorCommon.h"
#include "Core/AssetAssistLibrary.h"

FFolderMenuEditor::FFolderMenuEditor()
{
	memset(NewFolderName, 0, 128);
}

void FFolderMenuEditor::BuildEditor()
{

}

void FFolderMenuEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::Button("Create Folder"))
		{
			//ImGui::CloseCurrentPopup();
			ImGui::OpenPopup("Create Folder");
		}

		if (ImGui::Button("Delete Folder"))
		{
			//ImGui::CloseCurrentPopup();
			ImGui::OpenPopup("Delete Folder");
		}

		if (ImGui::BeginPopupModal("Create Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Create a new folder!");

			ImGui::Text("New folder name");
			ImGui::InputText("folder name", NewFolderName, 128);

			//显示路径+名字
			ImGui::Text("%s%s",Path.c_str(),NewFolderName);

			ImVec2 ButtonSize(ImGui::GetFontSize() * 7.f, 0.f);
			if (ImGui::Button("Sure", ButtonSize))
			{
				if (NewFolderName[0] != '\0')
				{
					Path += NewFolderName;
					create_file_directory(Path.c_str());

					memset(NewFolderName, 0, 128);

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ButtonSize))
			{
				memset(NewFolderName, 0, 128);

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Delete Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure to delete all files in the folder?");
			ImGui::Text("Delete all files In [%s]", Path.c_str());

			ImVec2 ButtonSize(ImGui::GetFontSize() * 7.f, 0.f);
			if (ImGui::Button("Sure", ButtonSize))
			{
				if (!Path.empty())
				{
					//移除文件
					remove_dir_all_files(Path.c_str());

					//移除文件夹
					remove_directory_all(Path.c_str());
				}
				
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ButtonSize))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::EndPopup();
	}
}

void FFolderMenuEditor::ExitEditor()
{

}