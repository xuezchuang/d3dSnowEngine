#pragma once
#include "DetailsEditor.h"

class GActorObject;
class FOutsideDetailsEditor :public FDetailsEditor
{
	typedef FDetailsEditor Super;
public:
	FOutsideDetailsEditor();
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

private:
	GActorObject* LastSelectedObject;
};