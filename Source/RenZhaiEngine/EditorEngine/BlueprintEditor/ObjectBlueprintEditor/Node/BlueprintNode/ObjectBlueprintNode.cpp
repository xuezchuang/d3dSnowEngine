#include "ObjectBlueprintNode.h"
#include "../../../BlueprintConfigInfo.h"
#include "CoreObject/FunctionObject.h"
#include "CoreObject/PropertyObject.h"

#include "../BlueprintPin/BoolBlueprintPin.h"
#include "../BlueprintPin/IntBlueprintPin.h"
#include "../BlueprintPin/FloatBlueprintPin.h"
#include "../BlueprintPin/VectorBlueprintPin.h"
#include "../BlueprintPin/StringBlueprintPin.h"

FObjectBlueprintNode::FObjectBlueprintNode()
	:Super()
{
}

void FObjectBlueprintNode::BuildEditor(const FCanvasGeometry& InGeometry)
{
	Super::BuildEditor(InGeometry);
}

void FObjectBlueprintNode::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);
}

void FObjectBlueprintNode::ExitEditor()
{
	Super::ExitEditor();
}

std::shared_ptr<FBlueprintPin> FObjectBlueprintNode::MakePin(CPropertyObject* InPinProperty)
{
	if (InPinProperty)
	{
		string InType = InPinProperty->GetType();
		if (InType == "int")
		{
			return make_shared<FIntBlueprintPin>();
		}
		else if (InType == "float")
		{
			return make_shared<FFloatBlueprintPin>();
		}
		else if (InType == "string")
		{
			return make_shared<FStringBlueprintPin>();
		}
		else if (InType == "bool")
		{
			return make_shared<FBoolBlueprintPin>();
		}
		else if (InType == "fvector_3d" || InType == "XMFLOAT3")
		{
			return make_shared<FVectorBlueprintPin>();
		}
	}

	return make_shared<FObjectBlueprintPin>();
}
