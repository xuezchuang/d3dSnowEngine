#pragma once
#include "ObjectBlueprintNode.h"

class FEventBlueprintNode :public FObjectBlueprintNode
{
	typedef FObjectBlueprintNode Super;
public:
	FEventBlueprintNode();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	virtual ImColor GetTopFrontColor() const { return IM_COL32(255, 100, 100, 255); }
	virtual ImColor GetTopBackColor() const { return IM_COL32(255, 100, 200, 100); }
};