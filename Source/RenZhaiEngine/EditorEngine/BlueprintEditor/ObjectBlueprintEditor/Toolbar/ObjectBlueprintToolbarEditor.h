#pragma once
#include "../../Core/BlueprintToolbarEidtor/BlueprintToolbarEditor.h"

class FObjectBlueprintToolbarEditor :public FBlueprintToolbarEditor
{
	typedef FBlueprintToolbarEditor Super;

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};