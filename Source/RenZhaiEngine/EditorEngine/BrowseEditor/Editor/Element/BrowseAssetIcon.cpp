#include "BrowseAssetIcon.h"
#include "../Menu/ObjectAssetMenuEditor.h"
#include "../../../Core/EditorCommon.h"
#include "../../../../Engine/Math/EngineMath.h"

FBrowseAssetIcon::FBrowseAssetIcon()
{
	IconSize = fvector_2d(64.f);
}

void FBrowseAssetIcon::Draw(int UID, const char* AssetName, const char* AssetFileName, float DeltaTime)
{
	FObjectAssetMenuEditor ObjectAssetMenuEditor;

	ImGui::BeginGroup();
	{
		//获取开始的位置
		ImVec2 CursorPos = ImGui::GetCursorScreenPos();
		IconPosition.x = CursorPos.x;
		IconPosition.y = CursorPos.y;

		ImGuiIO& IO = ImGui::GetIO();
		ImTextureID AssetTexture = IO.Fonts->TexID;

		float TexWidth = IO.Fonts->TexWidth;
		float TexHeight = IO.Fonts->TexHeight;

		ImVec2 ImIconSize = ImVec2(IconSize.x, IconSize.y);
		ImVec2 UV0 = ImVec2(0.f, 0.f);
		ImVec2 UV1 = ImVec2(32.f / TexWidth, 32.f / TexHeight);
		int SelectFramePadding = 3.f;

		ImGui::PushID(UID);

		if (ImGui::ImageButton(
			AssetTexture,
			ImIconSize,
			UV0, UV1,
			SelectFramePadding))
		{
			char SimpleAssetName[128] = { 0 };
			strcpy(SimpleAssetName, AssetName);

			replace_string_inline(SimpleAssetName, ".rz", "\0");

			BlueprintEditorSelected::Add(
				EBlueprintSelectedType::BlueprintSelected_Object,
				SimpleAssetName);
		}

		ObjectAssetMenuEditor.SetAssetFullPath(AssetFileName);
		ObjectAssetMenuEditor.DrawEditor(DeltaTime);

		//char Buff[512] = { 0 };
		//memcpy(Buff, AssetName, 7);
		//strcat(Buff, "...");

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 8.f * 9.14f);
		{
			ImGui::TextWrapped(AssetName);
		}
		ImGui::PopTextWrapPos();

		ImGui::PopID();
	}
	ImGui::EndGroup();
}

bool FBrowseAssetIcon::IsInTheScope(const fvector_2d& InCurrentPosition) const
{
	return EngineMath::IsInTheScope(GetIconPosition(), GetIconEndPosition(), InCurrentPosition);
}
