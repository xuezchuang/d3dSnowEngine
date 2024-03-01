#pragma once
#include "ObjectBlueprintPin.h"

class FBoolBlueprintPin :public FObjectBlueprintPin
{
	typedef FObjectBlueprintPin Super;
public:
	FBoolBlueprintPin();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	virtual ImColor GetPinColor() const { return IM_COL32(144, 10, 10, 255); }
	//virtual ImColor GetPinTextColor() const { return IM_COL32(255, 255, 255, 255); }
};