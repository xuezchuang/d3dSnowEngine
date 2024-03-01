#pragma once
#include "../Core/BlueprintEditor/BlueprintEditor.h"

class FObjectBlueprintEditor :public FBlueprintEditor
{
	typedef FBlueprintEditor Super;
public:
	FObjectBlueprintEditor();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

protected:
	std::map<std::string, std::shared_ptr<FEditorBase>> Editors;
};