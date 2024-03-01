#include "BlueprintNodeMenuEditor.h"
#include "../BlueprintGraphic/GraphicBlueprintEditor.h"

void FBlueprintNodeMenuEditor::BuildEditor()
{
}

void FBlueprintNodeMenuEditor::DrawEditor(float DeltaTime)
{
}

void FBlueprintNodeMenuEditor::ExitEditor()
{

}

void FBlueprintNodeMenuEditor::SetGraphicBlueprintEditor(FGraphicBlueprintEditor* InNewGraphicBlueprintEditor)
{
	auto InWeak = InNewGraphicBlueprintEditor->weak_from_this();
	if (!InWeak.expired())
	{
		GraphicBlueprintEditor = *(weak_ptr<FGraphicBlueprintEditor>*)&InWeak;
	}
}
