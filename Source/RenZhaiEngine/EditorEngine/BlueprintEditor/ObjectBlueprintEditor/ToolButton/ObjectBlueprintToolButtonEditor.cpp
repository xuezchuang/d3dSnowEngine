#include "ObjectBlueprintToolButtonEditor.h"
#include "../../BlueprintFunctionLibrary.h"
#include "../Graphic/ObjectGraphicBlueprintEditor.h"

void FObjectBlueprintToolButtonEditor::BuildEditor()
{
	Super::BuildEditor();

}

void FObjectBlueprintToolButtonEditor::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);

	ImGui::SameLine();

	if (ImGui::Button("Compile"))
	{
		if (!Graphic.expired())
		{
			GBlueprintFunctionLibrary::Compile(Graphic.lock());
		}

		Engine_Log("Compile");
	}

	ImGui::SameLine();
	if (ImGui::Button("Simulation Play"))
	{
		if (!Graphic.expired())
		{
			GBlueprintFunctionLibrary::SimulationPlay(Graphic.lock()->GetBlueprintAssetName(),"Begin");
		
			float XX = 0.3f;
			GBlueprintFunctionLibrary::SimulationPlay(Graphic.lock()->GetBlueprintAssetName(), "Tick", (float*)&XX);
		}
		
		Engine_Log("Simulation Play");
	}

	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		Engine_Log("Play");
	}
}

void FObjectBlueprintToolButtonEditor::ExitEditor()
{
	Super::ExitEditor();

}