#pragma once
#include "../../../Core/BlueprintNode/BlueprintNode.h"
#include "ObjectBlueprintNode.h"

class FFunctionBlueprintNode :public FObjectBlueprintNode
{
	typedef FObjectBlueprintNode Super;
public:
	FFunctionBlueprintNode();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	virtual ImColor GetTopFrontColor() const { return IM_COL32(100, 100, 255, 255); }
	virtual ImColor GetTopBackColor() const { return IM_COL32(100, 100, 255, 100); }
};