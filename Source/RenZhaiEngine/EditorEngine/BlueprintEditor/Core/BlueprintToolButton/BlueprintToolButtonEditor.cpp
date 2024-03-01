#include "BlueprintToolButtonEditor.h"
#include "../BlueprintGraphic/GraphicBlueprintEditor.h"

void FBlueprintToolButtonEditor::BuildEditor()
{
}

void FBlueprintToolButtonEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::Button("Save"))
	{
		Engine_Log("Save");
		if (!Graphic.expired())
		{
			Graphic.lock()->Save();
		}
	}
}

void FBlueprintToolButtonEditor::ExitEditor()
{

}

void FBlueprintToolButtonEditor::SetGraphicBlueprintEditor(FGraphicBlueprintEditor* InGraphic)
{
	auto InWeak = InGraphic->weak_from_this();
	if (!InWeak.expired())
	{
		Graphic = *(weak_ptr<FGraphicBlueprintEditor>*)&InWeak;
	}
}
