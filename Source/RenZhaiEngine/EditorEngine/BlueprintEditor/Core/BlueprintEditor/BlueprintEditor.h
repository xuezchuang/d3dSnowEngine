#pragma once
#include "../../../Core/EditorBase.h"

class FBlueprintEditor :public FEditorBase
{
public:
	FBlueprintEditor();
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	bool bOpenBlueprint;
	std::string BlueprintName;
};