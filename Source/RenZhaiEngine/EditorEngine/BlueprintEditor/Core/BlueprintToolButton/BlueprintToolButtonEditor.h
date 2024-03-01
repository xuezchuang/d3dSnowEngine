#pragma once
#include "../../../Core/EditorBase.h"

class FGraphicBlueprintEditor;
class FBlueprintToolButtonEditor :public FEditorBase
{
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void SetGraphicBlueprintEditor(FGraphicBlueprintEditor *InGraphic);

protected:
	std::weak_ptr<FGraphicBlueprintEditor> Graphic;
};