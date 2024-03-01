#include "BlueprintSADType.h"
#include "../BlueprintEditorType.h"

EBlueprintPinType BlueprintSerializeType::UnrealEnginePinTypeToOwnerEnginePinType(EUnrealEnginePinType Pin)
{
	return EBlueprintPinType();
}

EUnrealEnginePinType BlueprintSerializeType::OwnerEnginePinTypeToUnrealEnginePinType(EBlueprintPinType Pin)
{
	return EUnrealEnginePinType();
}

FBlueprintNodeData::FBlueprintNodeData()
	: NodeType(EBlueprintNodeType::Function_BlueprintNode)
	, AdvancedPinDisplay(EAdvancedPinDisplay::Hidden)
	, EnabledState(EEnabledState::DevelopmentOnly)
	, bOverrideFunction(false)
	, bCommentBubblePinned(false)
	, bIsPureFunc(false)
{
	normalization_guid(&NodeGuid);
}

FCustomProperties::FCustomProperties()
	:Direction(EBlueprintPinType::Pin_Input)
	, bHidden(false)
	, bNotConnectable(false)
	, bDefaultValueIsReadOnly(false)
	, bDefaultValueIsIgnored(false)
	, bAdvancedView(false)
	, bOrphanedPin(false)
{
	normalization_guid(&PinId);
	normalization_guid(&PersistentGuid);
}

FPinType::FPinType()
	:PinContainerType(FPinContainerType::None)
	, bIsReference(false)
	, bIsConst(false)
	, bIsWeakPointer(false)
	, bIsUObjectWrapper(false)
	, bSerializeAsSinglePrecisionFloat(false)
{
}
