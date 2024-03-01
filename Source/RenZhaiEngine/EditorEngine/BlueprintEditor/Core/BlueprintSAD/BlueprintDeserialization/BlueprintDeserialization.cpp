#include "BlueprintDeserialization.h"
#include "../../BlueprintEditorType.h"
#include "../../BlueprintGraphic/GraphicBlueprintEditor.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../../BlueprintNode/BlueprintNode.h"
#include "../../BlueprintNode/BlueprintPin.h"
#include "CoreObject/PropertyObject.h"
#include "CoreObject/FunctionObject.h"
#include "../../../BlueprintFunctionLibrary.h"

namespace BlueprintDeserialization
{
	struct FGraphicBlueprintDataMappings
	{
		FGraphicBlueprintDataMappings(
			const FBlueprintNodeData& InNodeData,
			std::shared_ptr<FBlueprintNode> InBlueprintNode)
			:NodeData(InNodeData)
			,BlueprintNode(InBlueprintNode)
		{}

		std::shared_ptr<FBlueprintPin> FindPin(const simple_c_guid& InPinGuid);

		const FBlueprintNodeData& NodeData;
		std::shared_ptr<FBlueprintNode> BlueprintNode;
	};
	typedef vector<FGraphicBlueprintDataMappings> FBlueprintDataMappings;

	std::shared_ptr<FBlueprintPin> FGraphicBlueprintDataMappings::FindPin(const simple_c_guid& InPinGuid)
	{
		for (auto& Tmp : *BlueprintNode->GetPins())
		{
			simple_c_guid NodeGuid = Tmp->GetGuid();
			if (guid_equal(&NodeGuid, &InPinGuid))
			{
				return Tmp;
			}
		}

		return nullptr;
	}

	const FCustomProperties* FindPinDataByName(const vector<FCustomProperties>& InPinsDataArray, const string& InPinName)
	{
		for (auto& Tmp : InPinsDataArray)
		{
			if (Tmp.PinName == InPinName)
			{
				return &Tmp;
			}
		}

		return nullptr;
	}

	std::shared_ptr<FBlueprintPin> FindPins(FBlueprintDataMappings& InMappings, const simple_c_guid& InGuid)
	{
		for (auto& Tmp : InMappings)
		{
			if (std::shared_ptr<FBlueprintPin> Pin = Tmp.FindPin(InGuid))
			{
				return Pin;
			}
		}

		return nullptr;
	}

	void ParseIntoVectorArray(
		const char* string_content,
		std::vector<std::string>& in_array,
		const char* parse_string)
	{
		vector<string> Strings;
		simple_cpp_string_algorithm::parse_into_vector_array(string_content, Strings, parse_string);

		string TmpString;//为了组装xxx=(1,2,3,4,5) 
		for (auto& Tmp : Strings)
		{
			if (!TmpString.empty())
			{
				TmpString += parse_string + Tmp;
			}

			if (simple_cpp_string_algorithm::string_contain(Tmp, "()"))
			{
				in_array.push_back(Tmp);
			}
			else if (simple_cpp_string_algorithm::string_contain(Tmp, "("))
			{
				TmpString += parse_string + Tmp;;
			}
			else if (simple_cpp_string_algorithm::string_contain(Tmp, ")"))
			{
				in_array.push_back(TmpString);

				TmpString.clear();
			}
			else
			{
				in_array.push_back(Tmp);
			}
		}
	}

	void ProcessLink(std::shared_ptr<FLinked>& InValue, const char* InSplitName, const char* InBuffer)
	{
		InValue = std::make_shared<FLinked>();

		//LinkedTo=(K2Node_Event_0 D7DF41AD407F283EC6D323A8F861C6F8,)
		char Buffer[1024] = { 0 };
		{
			char R[1024] = { 0 };
			char L[1024] = { 0 };
			split(InBuffer, InSplitName, L, R, false);

			remove_char_start(R, '(');
			remove_char_end(R, ')');
			remove_char_end(R, ',');

			strcpy(Buffer, R);//K2Node_Event_0 D7DF41AD407F283EC6D323A8F861C6F8
		}

		{
			char R[1024] = { 0 };
			char L[1024] = { 0 };
			split(Buffer, " ", L, R, false);

			InValue->LinkName = L;//K2Node_Event_0
			string_to_guid(R, &InValue->GUID);//D7DF41AD407F283EC6D323A8F861C6F8
		}
	}

	void ProcessString(string& InValue, const char* InSplitName, const char* InBuffer)
	{
		//PinName="execute"
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(InBuffer, InSplitName, L, R, false);

		remove_char_end(R, '"');
		remove_char_start(R, '"');

		InValue = R;
	}

	void ProcessReference(FEventReference& InValue, const char* InBuffer)
	{
		//   EventReference=(MemberParent=Class'"/Script/Engine.Actor"',MemberName="ReceiveBeginPlay")
		char Buffer[1024] = { 0 };
		{
			char R[1024] = { 0 };
			char L[1024] = { 0 };
			split(InBuffer, "MemberParent=", L, R, false);

			strcpy(Buffer, R);//Class'"/Script/Engine.Actor"',MemberName="ReceiveBeginPlay")
		}

		///Script/CoreUObject.Class'"ActorObject"',MemberName="Begin_Blueprint")
		{
			char R[1024] = { 0 };
			char L[1024] = { 0 };
			split(Buffer, ",MemberName=\"", L, R, false);

			remove_char_end(R, ')');
			remove_char_end(R, '"');

			InValue.MemberParent = L;
			InValue.MemberName = R;
		}
	}

	void ProcessDirection(EBlueprintPinType& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(InBuffer, InSplitName, L, R, false);

		remove_char_end(R, '"');
		remove_char_start(R, '"');

		if (string_equal(R, "EGPD_Output"))
		{
			InValue = EBlueprintPinType::Pin_Output;
		}
		else if (string_equal(R, "EGPD_Input"))
		{
			InValue = EBlueprintPinType::Pin_Input;
		}
	}

	void ProcessBool(bool& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };

		split(InBuffer, InSplitName, L, R, false);

		if (string_equal(R, "True"))
		{
			InValue = true;
		}
		else
		{
			InValue = false;
		}
	}

	void ProcessContainerType(FPinContainerType& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(InBuffer, InSplitName, L, R, false);

		if (string_equal(R, "None"))
		{
			InValue = FPinContainerType::None;
		}
	}

	void ProcessFloat(float& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };

		split(InBuffer, InSplitName, L, R, false);

		InValue = atof(R);
	}

	void ProcessGUID(simple_c_guid& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(InBuffer, InSplitName, L, R, false);

		string_to_guid(R, &InValue);
	}

	void ProcessEnabledState(EEnabledState& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(InBuffer, InSplitName, L, R, false);

		if (string_equal(R, "DevelopmentOnly"))
		{
			InValue = EEnabledState::DevelopmentOnly;
		}
	}

	void ProcessAdvancedPinDisplay(EAdvancedPinDisplay& InValue, const char* InSplitName, const char* InBuffer)
	{
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(InBuffer, InSplitName, L, R, false);

		if (string_equal(R, "Shown"))
		{
			InValue = EAdvancedPinDisplay::Shown;
		}
		else if (string_equal(R, "Hidden"))
		{
			InValue = EAdvancedPinDisplay::Hidden;
		}
	}

	bool Deserialization(const string& InBlueprintString, FBlueprintDatas& OutData)
	{
		vector<string> BlueprintStrings;
		simple_cpp_string_algorithm::parse_into_vector_array(InBlueprintString.c_str(), BlueprintStrings, "\r\n");

		return Deserialization(BlueprintStrings, OutData);
	}

	bool Deserialization(const vector<string>& BlueprintStrings, FBlueprintDatas& OutData)
	{
		auto GetTopNode = [&]()->FBlueprintNodeData&
		{
			return OutData[OutData.size() - 1];;
		};

		for (auto& Tmp : BlueprintStrings)
		{
			//开头
			if (simple_cpp_string_algorithm::string_contain(Tmp, "Begin Object Class="))
			{
				OutData.push_back(FBlueprintNodeData());
				FBlueprintNodeData& Node = GetTopNode();

				//tmp = Begin Object Class=/Script/BlueprintGraph.K2Node_Event Name="K2Node_Event_0"
				char Buffer[1024] = { 0 };
				{
					char R[1024] = { 0 };
					char L[1024] = { 0 };
					split(Tmp.c_str(), "=", L, R, false);

					strcpy(Buffer, R);
				}

				///Script/BlueprintGraph.K2Node_Event Name="K2Node_Event_0"
				{
					char R[1024] = { 0 };
					char L[1024] = { 0 };
					split(Buffer, " ", L, R, false);

					Node.ObjectClass.ClassScript = L;//Script/BlueprintGraph.K2Node_Event
					memset(Buffer, 0, 1024);

					strcpy(Buffer, R);
				}

				//Name="K2Node_Event_0"
				{
					char R[1024] = { 0 };
					char L[1024] = { 0 };
					split(Buffer, "\"", L, R, false);

					remove_char_end(R, '\"');//K2Node_Event_0

					Node.ObjectClass.Name = R;
				}
			}
			else if (simple_cpp_string_algorithm::string_contain(Tmp, "End Object"))//一个对象的结束
			{
				continue;
			}
			else
			{
				FBlueprintNodeData& Node = GetTopNode();

				if (simple_cpp_string_algorithm::string_contain(Tmp, "EventReference="))
				{
					Node.NodeType = EBlueprintNodeType::Event_BlueprintNode;

					ProcessReference(Node.Reference, Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "FunctionReference="))
				{
					Node.NodeType = EBlueprintNodeType::Function_BlueprintNode;

					ProcessReference(Node.Reference, Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "bOverrideFunction="))
				{
					ProcessBool(Node.bOverrideFunction, "bOverrideFunction=", Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "bIsPureFunc="))
				{
					ProcessBool(Node.bIsPureFunc, "bIsPureFunc=", Tmp.c_str());

					if (Node.bIsPureFunc)
					{
						Node.NodeType = EBlueprintNodeType::Pure_BlueprintNode;
					}
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "bCommentBubblePinned="))
				{
					ProcessBool(Node.bCommentBubblePinned, "bCommentBubblePinned=", Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "NodePosX="))
				{
					ProcessFloat(Node.NodePos.x, "NodePosX=", Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "NodePosY="))
				{
					ProcessFloat(Node.NodePos.y, "NodePosY=", Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "NodeGuid="))
				{
					ProcessGUID(Node.NodeGuid, "NodeGuid=", Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "EnabledState="))
				{
					ProcessEnabledState(Node.EnabledState, "EnabledState=", Tmp.c_str());
				}
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "AdvancedPinDisplay="))
				{
					ProcessAdvancedPinDisplay(Node.AdvancedPinDisplay, "AdvancedPinDisplay=", Tmp.c_str());
				}
				//Pin的处理
				else if (simple_cpp_string_algorithm::string_contain(Tmp, "CustomProperties Pin"))
				{
					vector<string> CustomPropertiesPins;
					{
						char R[1024] = { 0 };
						char L[1024] = { 0 };
						split(Tmp.c_str(), "(", L, R, false);

						ParseIntoVectorArray(R, CustomPropertiesPins, ",");
					}

					Node.CustomProperties.push_back(FCustomProperties());
					FCustomProperties& PinNode = Node.CustomProperties[Node.CustomProperties.size() - 1];
				
					for (auto& PinTmp : CustomPropertiesPins)
					{
						if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinId="))
						{
							ProcessGUID(PinNode.PinId, "PinId=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinName="))
						{
							ProcessString(PinNode.PinName, "PinName=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "Direction="))
						{
							ProcessDirection(PinNode.Direction, "Direction=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.PinCategory="))
						{
							ProcessString(PinNode.PinType.PinCategory, "PinType.PinCategory=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.PinSubCategory="))
						{
							ProcessString(PinNode.PinType.PinSubCategory, "PinType.PinSubCategory=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.PinSubCategoryObject="))
						{
							ProcessString(PinNode.PinType.PinSubCategoryObject, "PinType.PinSubCategoryObject=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.PinSubCategoryMemberReference="))
						{
							ProcessReference(PinNode.PinType.PinSubCategoryMemberReference, PinTmp.c_str());
						}
						//else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.PinValueType="))
						//{
						//	ProcessString(PinNode.PinType.PinValueType, "PinType.PinValueType=", PinTmp.c_str());
						//}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.ContainerType="))
						{
							ProcessContainerType(PinNode.PinType.PinContainerType, "PinType.ContainerType=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.bIsReference="))
						{
							ProcessBool(PinNode.PinType.bIsReference, "PinType.bIsReference=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.bIsConst="))
						{
							ProcessBool(PinNode.PinType.bIsConst, "PinType.bIsConst=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.bIsWeakPointer="))
						{
							ProcessBool(PinNode.PinType.bIsWeakPointer, "PinType.bIsWeakPointer=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.bIsUObjectWrapper="))
						{
							ProcessBool(PinNode.PinType.bIsUObjectWrapper, "PinType.bIsUObjectWrapper=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinType.bSerializeAsSinglePrecisionFloat="))
						{
							ProcessBool(PinNode.PinType.bSerializeAsSinglePrecisionFloat, "PinType.bSerializeAsSinglePrecisionFloat=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "bHidden="))
						{
							ProcessBool(PinNode.bHidden, "bHidden=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "bNotConnectable="))
						{
							ProcessBool(PinNode.bNotConnectable, "bNotConnectable=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "bDefaultValueIsReadOnly="))
						{
							ProcessBool(PinNode.bDefaultValueIsReadOnly, "bDefaultValueIsReadOnly=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "bDefaultValueIsIgnored="))
						{
							ProcessBool(PinNode.bDefaultValueIsIgnored, "bDefaultValueIsIgnored=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "bAdvancedView="))
						{
							ProcessBool(PinNode.bAdvancedView, "bAdvancedView=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "bOrphanedPin="))
						{
							ProcessBool(PinNode.bOrphanedPin, "bOrphanedPin=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PersistentGuid="))
						{
							ProcessGUID(PinNode.PersistentGuid, "PersistentGuid=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "LinkedTo="))
						{
							ProcessLink(PinNode.LinkedTo, "LinkedTo=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "DefaultValue="))
						{
							ProcessString(PinNode.DefaultValue, "DefaultValue=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "AutogeneratedDefaultValue="))
						{
							ProcessString(PinNode.AutogeneratedDefaultValue, "AutogeneratedDefaultValue=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinToolTip="))
						{
							ProcessString(PinNode.PinToolTip, "PinToolTip=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "PinFriendlyName="))
						{
							ProcessString(PinNode.PinFriendlyName, "PinFriendlyName=", PinTmp.c_str());
						}
						else if (simple_cpp_string_algorithm::string_contain(PinTmp, "DefaultObject="))
						{
							ProcessString(PinNode.DefaultObject, "DefaultObject=", PinTmp.c_str());
						}
					}
				}
			}
		}

		return OutData.size() > 0;
	}

	bool Deserialization(FGraphicBlueprintEditor* GraphicBlueprintEditor, const FBlueprintDatas& InDatas)
	{
		FBlueprintDataMappings BlueprintDataMappings;

		////先生成对应蓝图主干 保证蓝图有足够的pin guid 和 node guid 方面后面链接
		for (auto& Tmp : InDatas)
		{
			if (CCoreMinimalObject* InObject = GraphicBlueprintEditor->GetGraphicBlueprintObject())
			{
				CFunctionObject* InFunc = InObject->GetNativeClass().FindFunction(Tmp.Reference.MemberName, Tmp.Reference.MemberParent);
				if (!InFunc)
				{
					InFunc = InObject->GetNativeClass().FindStaticFunction(Tmp.Reference.MemberName, Tmp.Reference.MemberParent);
				}

				if (InFunc)
				{
					InObject->ResetGuid(Tmp.NodeGuid);

					if (std::shared_ptr<FBlueprintNode> BlueprintNode = GBlueprintFunctionLibrary::CreateBulueprintNode(InFunc, GraphicBlueprintEditor, Tmp.NodePos))
					{
						for (auto& TmpPin : *BlueprintNode->GetPins())
						{
							if (const FCustomProperties* InCustomProper = FindPinDataByName(Tmp.CustomProperties, TmpPin->GetName()))
							{
								TmpPin->ResetGuid(InCustomProper->PinId);
								if (TmpPin->GetProperty())
								{
									TmpPin->GetProperty()->SetDefaultValue(InCustomProper->DefaultValue);
								}
							}
						}

						//存储到映射关系结构
						BlueprintDataMappings.push_back(FGraphicBlueprintDataMappings(Tmp, BlueprintNode));
					}
				}
				else
				{
					assert(0);
				}
			}
		}

		//链接各个蓝图节点
		for (auto& Tmp : BlueprintDataMappings)
		{
			for (auto& TmpCustomProper : Tmp.NodeData.CustomProperties)
			{
				if (TmpCustomProper.LinkedTo)
				{
					std::shared_ptr<FBlueprintPin> StartLinkPin = nullptr;
					for (auto& PinTmp : *Tmp.BlueprintNode->GetPins())
					{
						if (PinTmp->GetName() == TmpCustomProper.PinName)
						{
							StartLinkPin = PinTmp;
							break;
						}
					}

					if (std::shared_ptr<FBlueprintPin> EndLinkPin = FindPins(BlueprintDataMappings, TmpCustomProper.LinkedTo->GUID))
					{
						GraphicBlueprintEditor->LinkPin(StartLinkPin, EndLinkPin);
					}
				}
			}
		}

		return true;
	}
}
