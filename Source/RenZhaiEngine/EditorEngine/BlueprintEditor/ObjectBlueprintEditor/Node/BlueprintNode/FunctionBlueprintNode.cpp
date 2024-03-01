#include "FunctionBlueprintNode.h"
#include "../BlueprintPin/ObjectBlueprintPin.h"
#include "../../../BlueprintConfigInfo.h"
#include "CoreObject/FunctionObject.h"
#include "CoreObject/PropertyObject.h"

FFunctionBlueprintNode::FFunctionBlueprintNode()
	:Super()
{
	SetType(EBlueprintNodeType::Function_BlueprintNode);
}

void FFunctionBlueprintNode::BuildEditor(const FCanvasGeometry& InGeometry)
{
	Super::BuildEditor(InGeometry);

	AddPin(EBlueprintPinType::Pin_Input, InGeometry,nullptr);
	AddPin(EBlueprintPinType::Pin_Output, InGeometry, nullptr);

	if (Function)
	{
		ConstructionPin(Function->GetNativeClass().Property, InGeometry);
	}

	BlueprintNodeSelfAdaption(InGeometry.ZoomRatio);
}

void FFunctionBlueprintNode::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FFunctionBlueprintNode::ExitEditor()
{
	Super::ExitEditor();
}
