#include "BlueprintSerialize.h"
#include "../../BlueprintGraphic/GraphicBlueprintEditor.h"
#include "../../BlueprintNode/BlueprintNode.h"
#include "../../BlueprintNode/BlueprintPin.h"
#include "CoreObject/PropertyObject.h"
#include "CoreObject/FunctionObject.h"

namespace BlueprintSerialize
{
	string GetAdvancedPinDisplayString(EAdvancedPinDisplay InType)
	{
		switch (InType)
		{
		case Hidden:
			return "Hidden";
		case Shown:
			return "Shown";
		}

		return "";
	}

	string GetPinContainerType(FPinContainerType InType)
	{
		switch (InType)
		{
		case None:
			return "None";
		}

		return "";
	}

	string GetBool(bool InValue)
	{
		return InValue == true ? "True" : "False";
	}

	string GetEnabledState(EEnabledState InEnableState)
	{
		switch (InEnableState)
		{
		case EEnabledState::DevelopmentOnly:
			return "DevelopmentOnly";
		}

		return "";
	}

	bool Serialize(FBlueprintDatas& Out, FGraphicBlueprintEditor* GraphicBlueprintEditor)
	{
		//先归一到统一尺寸度量缩放
		float ZoomRatio = GraphicBlueprintEditor->GetZoomRatio();
		GraphicBlueprintEditor->ZoomBlueprintNodeOffset(1.f);

		//序列化
		{
			for (auto& Tmp : *GraphicBlueprintEditor->GetBlueprintNodes())
			{
				Out.push_back(FBlueprintNodeData());
				FBlueprintNodeData& InNodeData = Out[Out.size() - 1];

				InNodeData.NodeType = Tmp->GetType();
				InNodeData.bIsPureFunc = ((InNodeData.NodeType == EBlueprintNodeType::Pure_BlueprintNode) ? true : false);
			
				InNodeData.NodePos = Tmp->GetOffsetPosition();
				InNodeData.NodeGuid = Tmp->GetGuid();

				InNodeData.Reference.MemberName = Tmp->GetFunction()->GetName();
				InNodeData.Reference.MemberParent = 
					simple_cpp_string_algorithm::printf("/Script/CObject.Class\'\"%s\"\'",//
					Tmp->GetFunction()->GetOuter()->GetName().c_str());

				for (auto& PinTmp : *Tmp->GetPins())
				{
					InNodeData.CustomProperties.push_back(FCustomProperties());
					FCustomProperties& InProperties = InNodeData.CustomProperties[InNodeData.CustomProperties.size() - 1];

					InProperties.PinId = PinTmp->GetGuid();
					InProperties.PinName = PinTmp->GetName();
					InProperties.Direction = PinTmp->GetType();
					if (auto InProper = PinTmp->GetProperty())
					{
						InProperties.DefaultValue = InProper->GetDefaultValue();
					}

					if (InProperties.Direction == EBlueprintPinType::Pin_Input ||
						InProperties.Direction == EBlueprintPinType::Pin_ParamInput)
					{
						for (auto& LinkTmp : *PinTmp->GetLinkPins())
						{
							InProperties.LinkedTo = std::make_shared<FLinked>();

							InProperties.LinkedTo->LinkName = LinkTmp.lock()->GetName();
							InProperties.LinkedTo->GUID = LinkTmp.lock()->GetGuid();
						}
					}
				}
			}
		}

		//还原缩放值
		GraphicBlueprintEditor->ZoomBlueprintNodeOffset(ZoomRatio);

		return Out.size() > 0;
	}

	bool Serialize(const FBlueprintDatas& InData, vector<string>& InBlueprintString)
	{
		for (auto& Tmp : InData)
		{
			if (Serialize(Tmp, InBlueprintString))
			{

			}
		}

		return InBlueprintString.size() > 0;
	}

	bool Serialize(const FBlueprintNodeData& InData, vector<string>& InBlueprintString)
	{
		//Begin Object Class=/Script/BlueprintGraph.K2Node_Event Name="K2Node_Event_0"
		InBlueprintString.push_back(simple_cpp_string_algorithm::printf(
			"Begin Object Class=%s Name=\"%s\"",
			InData.ObjectClass.ClassScript.c_str(),
			InData.ObjectClass.Name.c_str()));
		{
			// EventReference=(MemberParent=/Script/CoreUObject.Class'"/Script/Engine.Actor"',MemberName="ReceiveBeginPlay")
			InBlueprintString.push_back(simple_cpp_string_algorithm::printf(
				"   EventReference=(MemberParent=%s,MemberName=\"%s\")",
				InData.Reference.MemberParent.c_str(),
				InData.Reference.MemberName.c_str()));

			if (InData.bOverrideFunction)
			{
				InBlueprintString.push_back("   bOverrideFunction=True");
			}

			if (InData.NodePos.x != 0.f)
			{
				InBlueprintString.push_back(
					simple_cpp_string_algorithm::printf("   NodePosX=%i", (int)InData.NodePos.x));
			}

			if (InData.NodePos.y != 0.f)
			{
				InBlueprintString.push_back(
					simple_cpp_string_algorithm::printf("   NodePosY=%i", (int)InData.NodePos.y));
			}

			if (InData.bCommentBubblePinned)
			{
				InBlueprintString.push_back("   bCommentBubblePinned=True");
			}

			char GUIDString[512] = { 0 };
			guid_to_string(GUIDString, &InData.NodeGuid);
			InBlueprintString.push_back(
				simple_cpp_string_algorithm::printf("   NodeGuid=%s", GUIDString));

			InBlueprintString.push_back(
				simple_cpp_string_algorithm::printf("   AdvancedPinDisplay=%s", GetAdvancedPinDisplayString(InData.AdvancedPinDisplay).c_str()));

			InBlueprintString.push_back(
				simple_cpp_string_algorithm::printf("   EnabledState=%s", GetEnabledState(InData.EnabledState).c_str()));

			//序列化自定义变量
			for (auto& Tmp : InData.CustomProperties)
			{
				InBlueprintString.push_back(string());
				Serialize(Tmp, InBlueprintString[InBlueprintString.size() - 1]);
			}
		}
		InBlueprintString.push_back("End Object");

		return InBlueprintString.size() > 0;
	}

	bool Serialize(const FCustomProperties& InData, string& InBlueprintString)
	{
		char PinId[512] = { 0 };
		guid_to_string(PinId, &InData.PinId);

		char PersistentGuid[512] = { 0 };
		guid_to_string(PersistentGuid, &InData.PersistentGuid);

		string PinSubCategoryMemberReference;
		if (!InData.PinType.PinSubCategoryMemberReference.MemberParent.empty())
		{
			PinSubCategoryMemberReference = simple_cpp_string_algorithm::printf(
				"(MemberParent=/Script/CoreUObject.Class'\"%s\"',MemberName=\"%s\")",
				InData.PinType.PinSubCategoryMemberReference.MemberParent.c_str(),
				InData.PinType.PinSubCategoryMemberReference.MemberName.c_str());
		}

		//LinkedTo=(K2Node_CallFunction_1 00EE4AA44E91814979F5548120AC1E97,)
		string LinkTo;
		if (InData.LinkedTo)
		{
			char LinkGuid[512] = { 0 };
			guid_to_string(LinkGuid, &InData.LinkedTo->GUID);

			LinkTo = simple_cpp_string_algorithm::printf("LinkedTo=(%s %s,),", InData.LinkedTo->LinkName.c_str(), LinkGuid);
		}

		InBlueprintString = simple_cpp_string_algorithm::printf(
			"   \
CustomProperties Pin (PinId=%s,\
PinName=\"%s\",\
PinType.PinCategory=\"%s\",\
PinType.PinSubCategory=\"%s\",\
PinType.PinSubCategoryObject=%s,\
PinType.PinSubCategoryMemberReference=(%s),\
PinType.PinValueType=(),\
PinType.ContainerType=%s,\
PinType.bIsReference=%s,\
PinType.bIsConst=%s,\
PinType.bIsWeakPointer=%s,\
PinType.bIsUObjectWrapper=%s,\
PinType.bSerializeAsSinglePrecisionFloat=%s,\
%s\
PersistentGuid=%s,\
bHidden=%s,\
bNotConnectable=%s,\
bDefaultValueIsIgnored=%s,\
bAdvancedView=%s,\
bOrphanedPin=%s,\
)",
PinId,
InData.PinName.c_str(),
InData.PinType.PinCategory.c_str(),
InData.PinType.PinSubCategory.c_str(),
InData.PinType.PinSubCategoryObject.c_str(),
PinSubCategoryMemberReference.c_str(),

GetPinContainerType(InData.PinType.PinContainerType).c_str(),
GetBool(InData.PinType.bIsReference).c_str(),
GetBool(InData.PinType.bIsConst).c_str(),
GetBool(InData.PinType.bIsWeakPointer).c_str(),
GetBool(InData.PinType.bIsUObjectWrapper).c_str(),
GetBool(InData.PinType.bSerializeAsSinglePrecisionFloat).c_str(),
LinkTo.c_str(),
PersistentGuid,
GetBool(InData.bHidden).c_str(),
GetBool(InData.bNotConnectable).c_str(),
GetBool(InData.bDefaultValueIsReadOnly).c_str(),
GetBool(InData.bAdvancedView).c_str(),
GetBool(InData.bOrphanedPin).c_str()
);
		return InBlueprintString.length() > 0;
	}
}