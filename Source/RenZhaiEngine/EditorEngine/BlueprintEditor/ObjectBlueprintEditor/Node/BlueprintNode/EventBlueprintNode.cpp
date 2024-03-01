#include "EventBlueprintNode.h"
#include "../BlueprintPin/ObjectBlueprintPin.h"
#include "../../../BlueprintConfigInfo.h"
#include "CoreObject/FunctionObject.h"
#include "CoreObject/PropertyObject.h"

FEventBlueprintNode::FEventBlueprintNode()
	:Super()
{
	SetType(EBlueprintNodeType::Event_BlueprintNode);
}

void FEventBlueprintNode::BuildEditor(const FCanvasGeometry& InGeometry)
{
	Super::BuildEditor(InGeometry);

	AddPin(EBlueprintPinType::Pin_Output, InGeometry, nullptr);

	if (Function)
	{
		ConstructionPin(Function->GetNativeClass().Property,InGeometry,EConstructionPinType::ALL_OUTPUT_PARAM);
	}

	BlueprintNodeSelfAdaption(InGeometry.ZoomRatio);
}

void FEventBlueprintNode::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FEventBlueprintNode::ExitEditor()
{
	Super::ExitEditor();
}
