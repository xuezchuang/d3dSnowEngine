#pragma once
#include "../../Core/BlueprintToolButton/BlueprintToolButtonEditor.h"

class FObjectBlueprintToolButtonEditor :public FBlueprintToolButtonEditor
{
	typedef FBlueprintToolButtonEditor Super;
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};