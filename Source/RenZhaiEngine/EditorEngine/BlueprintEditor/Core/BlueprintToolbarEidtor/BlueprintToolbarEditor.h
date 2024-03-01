#pragma once
#include "../../../Core/EditorBase.h"

class FBlueprintToolbarEditor :public FEditorBase
{
	typedef FEditorBase Super;

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};