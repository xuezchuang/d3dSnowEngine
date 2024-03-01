#include "PureFunctionBlueprintNode.h"
#include "../BlueprintPin/ObjectBlueprintPin.h"
#include "../../../BlueprintConfigInfo.h"
#include "CoreObject/FunctionObject.h"
#include "CoreObject/PropertyObject.h"

FPureFunctionBlueprintNode::FPureFunctionBlueprintNode()
	:Super()
{
	SetType(EBlueprintNodeType::Pure_BlueprintNode);
}

void FPureFunctionBlueprintNode::BuildEditor(const FCanvasGeometry& InGeometry)
{
	Super::BuildEditor(InGeometry);

	if (Function)
	{
		ConstructionPin(Function->GetNativeClass().Property, InGeometry);
	}

	BlueprintNodeSelfAdaption(InGeometry.ZoomRatio);
}

void FPureFunctionBlueprintNode::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FPureFunctionBlueprintNode::ExitEditor()
{
	Super::ExitEditor();
}
