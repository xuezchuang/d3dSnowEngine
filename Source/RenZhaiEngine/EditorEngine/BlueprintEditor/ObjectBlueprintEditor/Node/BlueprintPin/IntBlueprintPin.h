#pragma once
#include "ObjectBlueprintPin.h"

class FIntBlueprintPin :public FObjectBlueprintPin
{
	typedef FObjectBlueprintPin Super;
public:
	FIntBlueprintPin();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	virtual ImColor GetPinColor() const { return IM_COL32(100, 100, 255, 255); }
	//virtual ImColor GetPinTextColor() const { return IM_COL32(255, 255, 255, 255); }
};