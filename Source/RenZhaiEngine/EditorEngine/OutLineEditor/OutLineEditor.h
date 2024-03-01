#pragma once
#include "../Core/EditorBase.h"
#include "../../Engine/Interface/DirectXDeviceInterfece.h"

class GActorObject;
class FOutLineEditor :public FEditorBase, public IDirectXDeviceInterfece
{
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

	void HighlightDisplayObject(GActorObject *InObject);
};