#include "BlueprintPin.h"
#include "../../BlueprintConfigInfo.h"

FBlueprintPin::FBlueprintPin()
{
	NodeName = "New Param";
	Property = nullptr;
}

void FBlueprintPin::BuildEditor(const FCanvasGeometry& InGeometry)
{
	switch (Type)
	{
	case Pin_Input:
		NodeName = "Input";
		break;
	case Pin_Output:
		NodeName = "Output";
		break;
	}
}

void FBlueprintPin::DrawEditor(float DeltaTime)
{
	DrawPin(DeltaTime);

	DrawText(DeltaTime);
}

void FBlueprintPin::ExitEditor()
{
	
}

void FBlueprintPin::CheckLinkPins(float DeltaTime)
{
	bool bRemove = false;
	for (auto& Tmp : LinkPins)
	{
		if (Tmp.expired())
		{
			bRemove = true;
		}
	}

	if (bRemove)
	{
		vector<std::weak_ptr<FNode>> TmpPins;
		for (auto& Tmp : LinkPins)
		{
			if (!Tmp.expired())
			{
				TmpPins.push_back(Tmp);
			}
		}

		LinkPins = TmpPins;

		//ÊÖ¶¯¶Ï¿ª...
	}

}

void FBlueprintPin::AddLinkPin(std::shared_ptr<FNode> InPin)
{
	LinkPins.push_back(InPin);
}

void FBlueprintPin::DrawText(float DeltaTime)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	fvector_2d CurrentPosition = GetPosition();

	int TextSize = FBlueprintConfigInfo::Get()->BlueprintPinFontSize * GetZoomRatio();

	int TextLength = NodeName.length();
	float NodeSizeX = NodeSize.x + NodeSize.x * 0.3f;

	float ZoomSizeCoefficient = XM_PI * 17.f * 0.01f;

	ImVec2 DrawPosition;
	switch (Type)
	{
	case Pin_Input:
	case Pin_ParamInput:
		DrawPosition = ImVec2(
			CurrentPosition.x + NodeSizeX,
			CurrentPosition.y - NodeSize.y);
		break;
	case Pin_Output:
	case Pin_ParamOutput:
		DrawPosition = ImVec2(
			CurrentPosition.x - NodeSizeX - TextLength * TextSize * ZoomSizeCoefficient,
			CurrentPosition.y - NodeSize.y);
		break;
	}

	DrawList->AddText(
		NULL,
		TextSize,
		DrawPosition,
		GetPinTextColor(),
		NodeName.c_str());
}

void FBlueprintPin::DrawPin(float DeltaTime)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	fvector_2d CurrentPosition = GetPosition();

	DrawList->AddCircleFilled(
		ImVec2(CurrentPosition.x, CurrentPosition.y),
		NodeSize.x,
		GetPinColor(), 12);
}

void FBlueprintPin::OnMouseHovered(const FCanvasGeometry& InGeometry)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	fvector_2d CurrentPosition = GetPosition();

	DrawList->AddCircleFilled(ImVec2(CurrentPosition.x, CurrentPosition.y), NodeSize.x, IM_COL32(255, 255, 50, 255), 12);
}

void FBlueprintPin::OnMouseClicked(const FCanvasGeometry& InGeometry)
{

}

void FBlueprintPin::OnMouseDrag(const FCanvasGeometry& InGeometry)
{
	ImGuiIO& IO = ImGui::GetIO();
	OffsetPosition += fvector_2d(IO.MouseDelta.x, IO.MouseDelta.y);

	StandardOffsetPosition = OffsetPosition;
	StandardOffsetPosition /= InGeometry.ZoomRatio;
}

void FBlueprintPin::ZoomSize(float InRatio)
{
	NodeSize = FBlueprintConfigInfo::Get()->BlueprintPinSize * InRatio;
}

void FBlueprintPin::ZoomOffset(float InRatio)
{
	Super::ZoomOffset(InRatio);
}

float FBlueprintPin::GetZoomRatio() const
{
	return NodeSize.x / FBlueprintConfigInfo::Get()->BlueprintPinSize.x;
}

void FBlueprintPin::SetProperty(CPropertyObject* InProperty)
{
	Property = InProperty;
}
