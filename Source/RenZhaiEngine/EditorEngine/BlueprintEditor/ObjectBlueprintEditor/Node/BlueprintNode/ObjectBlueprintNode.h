#pragma once
#include "../../../Core/BlueprintNode/BlueprintNode.h"

class FObjectBlueprintNode :public FBlueprintNode
{
	typedef FBlueprintNode Super;
public:
	FObjectBlueprintNode();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	virtual std::shared_ptr<FBlueprintPin> MakePin(CPropertyObject* InPinProperty);
};