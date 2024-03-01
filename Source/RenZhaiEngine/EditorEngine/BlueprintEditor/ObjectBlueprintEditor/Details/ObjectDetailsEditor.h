#pragma once
#include "../../../DetailsEditor/DetailsEditor.h"

class GActorObject;
class FObjectDetailsEditor :public FDetailsEditor
{
	typedef FDetailsEditor Super;
public:
	FObjectDetailsEditor();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};