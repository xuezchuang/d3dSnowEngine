#pragma once
#include "../../../Core/EditorBase.h"

class FGraphicBlueprintEditor;
class FBlueprintNodeMenuEditor :public FEditorBase
{
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void SetGraphicBlueprintEditor(FGraphicBlueprintEditor* InNewGraphicBlueprintEditor);

protected:
	std::weak_ptr<FGraphicBlueprintEditor> GraphicBlueprintEditor;
};