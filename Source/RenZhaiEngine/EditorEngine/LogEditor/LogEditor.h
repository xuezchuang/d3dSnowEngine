#pragma once
#include "../Core/EditorBase.h"

class FLogEditor :public FEditorBase
{
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};