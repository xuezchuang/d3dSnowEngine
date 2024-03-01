#pragma once
#include "../../../Core/BlueprintNode/BlueprintPin.h"

class FObjectBlueprintPin :public FBlueprintPin
{
	typedef FBlueprintPin Super;
public:
	FObjectBlueprintPin();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
};