#pragma once
#include "../../Core/BlueprintMenu/BlueprintNodeMenuEditor.h"

class CFunctionObject;
class FObjectBlueprintNodeMenuEditor :public FBlueprintNodeMenuEditor
{
	typedef FBlueprintNodeMenuEditor Super;
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};