#pragma once
#include "ObjectBlueprintPin.h"

class FVectorBlueprintPin :public FObjectBlueprintPin
{
	typedef FObjectBlueprintPin Super;
public:
	FVectorBlueprintPin();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	virtual ImColor GetPinColor() const { return IM_COL32(120, 120, 10, 255); }
	//virtual ImColor GetPinTextColor() const { return IM_COL32(255, 255, 255, 255); }
};