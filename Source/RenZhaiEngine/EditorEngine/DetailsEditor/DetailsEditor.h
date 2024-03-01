#pragma once
#include "../Core/EditorBase.h"

class GActorObject;
class FDetailsEditor :public FEditorBase
{
public:
	FDetailsEditor();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void UpdateActorDetailsEditor(GActorObject* InUpdateActor);
};