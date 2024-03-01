#include "AssetEditor.h"
#include "../Core/AssetEditorCommon.h"
#include "../../../Engine/Math/EngineMath.h"

void FAssetEditor::BuildEditor()
{

}

void FAssetEditor::DrawEditor(float DeltaTime)
{
	Icons.clear();

	static ImGuiTextFilter Filter;

	Filter.Draw();

	if (!CurrentSelectAssetPath.expired())
	{
		auto SelectAssetPath = CurrentSelectAssetPath.lock();

		Geometry.CanvasSize = ImGui::GetContentRegionAvail();
		float IconSize = 64.f + 12.f;

		float StandardSize = (Geometry.CanvasSize.x / IconSize);
		
		//被%不能为零
		if (StandardSize < 1.f)
		{
			StandardSize = 1.f;
		}

		int StandardNum = (int)ceil(StandardSize);
		int AssetNum = SelectAssetPath->AssetPaths.size();

		for (size_t i = 0; i < AssetNum; i++)
		{
			if (Filter.PassFilter(SelectAssetPath->AssetPaths[i].c_str()))
			{
				char Buff[512] = { 0 };
				get_path_clean_filename(Buff, SelectAssetPath->AssetPaths[i].c_str());
				
				Icons.push_back(FBrowseAssetIcon());
				Icons[Icons.size() - 1].Draw(i, Buff, SelectAssetPath->AssetPaths[i].c_str(), DeltaTime);

				int Remainder = (i + 1) % (StandardNum + 1);
				float OccupancySize = (float)(Remainder * IconSize);

				if (OccupancySize > Geometry.CanvasSize.x)
				{
					continue;
				}

				ImGui::SameLine();
			}
		}

		if (StandardNum > 6)
		{
			ImGui::Spacing();
		}

		ImGuiIO& IO = ImGui::GetIO();
		Geometry.MousePos.x = IO.MousePos.x;
		Geometry.MousePos.y = IO.MousePos.y;

		OnMouseDragging();
	}
}

void FAssetEditor::ExitEditor()
{

}

bool FAssetEditor::IsInTheScope(const fvector_2d& InCurrentPosition) const
{
	for (auto& Tmp : Icons)
	{
		if (Tmp.IsInTheScope(InCurrentPosition))
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor::OnMouseDragging()
{
	if (ImGui::IsMouseDragging(0))
	{
		if (Geometry.OperationCommand == EAssetOperationCommand::ASSETOPERATION_NONE)
		{
			if (IsInTheScope(Geometry.MousePos))
			{
				Geometry.OperationCommand = EAssetOperationCommand::ASSETOPERATION_DRAG;
			}
		}
		else if(Geometry.OperationCommand == EAssetOperationCommand::ASSETOPERATION_DRAG)
		{
			ImDrawList* DrawList = ImGui::GetWindowDrawList();

			ImGuiIO& IO = ImGui::GetIO();

			//
			Geometry.DeltaValue.x += IO.MouseDelta.x;
			Geometry.DeltaValue.y += IO.MouseDelta.y;

			fvector_2d EndPosition = Geometry.MousePos + fvector_2d(72.f);

			ImVec2 P0 = ImVec2(Geometry.MousePos.x, Geometry.MousePos.y);
			ImVec2 P1 = ImVec2(EndPosition.x, EndPosition.y);

			ImGui::PushClipRect(P0, P1, false);
			DrawList->AddRectFilled(
				P0,
				P1, IM_COL32(36, 70, 110, 255));
			ImGui::PopClipRect();
		}
	}
	else
	{
		if (Geometry.OperationCommand == EAssetOperationCommand::ASSETOPERATION_DRAG)
		{
			Geometry.OperationCommand = EAssetOperationCommand::ASSETOPERATION_NONE;
			Geometry.DeltaValue = fvector_2d();
		}
	}
}
