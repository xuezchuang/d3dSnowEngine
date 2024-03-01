#include "BlueprintFunctionLibrary.h"
#include "CodeReflection/ClassManage.h"
#include "CoreObject/ClassObject.h"
#include "Core/AssetPackageManage.h"
#include "Core/BlueprintGraphic/GraphicBlueprintEditor.h"
#include "Core/BlueprintNode/BlueprintNode.h"
#include "Core/BlueprintNode/BlueprintPin.h"
#include "CoreObject/FunctionObject.h"
#include "BlueprintCompile/BlueprintCompilerContext.h"
#include "BlueprintCompile/BlueprintFunctionContext.h"

void GBlueprintFunctionLibrary::SimulationPlay(const string& InClassName, const string& EventName, void* InParam)
{
	if (CAssetPackage* InPackage = FAssetPackageManage::Get()->FindPackageSafe(InClassName))
	{
		if (CClassObject* InClass = InPackage->GetClassObject())
		{
			if (CCoreMinimalObject* InMinimalObject = InClass->GetDefaultObject())
			{
				InMinimalObject->ExecutionEventScript(EventName, InParam);
			}
		}
	}
}

vector<shared_ptr<FBlueprintNode>> GBlueprintFunctionLibrary::GetEventBlueprintRoot(std::shared_ptr<FGraphicBlueprintEditor> InGraphic)
{
	vector<shared_ptr<FBlueprintNode>> EventNodes;
	for (auto& Tmp : InGraphic->Nodes)
	{
		if (Tmp->GetType() == Event_BlueprintNode)
		{
			if (Tmp->GetFunction())
			{
				EventNodes.push_back(Tmp);
			}
		}
	}

	return EventNodes;
}

void GBlueprintFunctionLibrary::PruneSolatedNodes(
	const vector<shared_ptr<FBlueprintNode>>& RootEvent, 
	vector<vector<shared_ptr<FBlueprintNode>>>& OutVisitedNodes)
{
	for (auto& Tmp : RootEvent)
	{
		OutVisitedNodes.push_back(vector<shared_ptr<FBlueprintNode>>());
		TraverseNodes(Tmp, OutVisitedNodes[OutVisitedNodes.size() - 1]);
	}
}

void GBlueprintFunctionLibrary::TraverseNodes(shared_ptr<FBlueprintNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes)
{
	//���õ��������ӵ����Ӵ������ӿ�
	TraverseParamPureNodes(Node, OutVisitedNodes);

	OutVisitedNodes.push_back(Node);

	for (int i = 0; i < Node->Pins.size(); ++i)
	{
		if (std::shared_ptr<FBlueprintPin>& BPPin = Node->Pins[i])
		{
			//���뱣֤�����ӵ�ִ���� �������������
			if (BPPin->GetType() == EBlueprintPinType::Pin_Output)
			{
				//����ԵȵĲ���
				for (int j = 0; j < BPPin->LinkPins.size(); j++)
				{
					if (auto LinkNodePin = BPPin->LinkPins[j].lock())
					{
						std::shared_ptr<FNode> BlueprintNode = LinkNodePin->GetOuter();
						//�������в������ظ���
						if (!Contains(BlueprintNode, OutVisitedNodes))
						{
							auto TmpBP = BlueprintNode.get()->shared_from_this();
							shared_ptr<FBlueprintNode> NewBPNode = *(shared_ptr<FBlueprintNode>*)&TmpBP;

							TraverseNodes(NewBPNode, OutVisitedNodes);
						}
					}		
				}
			}
		}
	}
}

void GBlueprintFunctionLibrary::TraverseParamPureNodes(shared_ptr<FBlueprintNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes)
{
	for (int i = 0; i < Node->Pins.size(); i++)
	{
		if (FBlueprintPin* InParamPin = dynamic_cast<FBlueprintPin*>(Node->Pins[i].get()))
		{
			if (InParamPin->GetType() == EBlueprintPinType::Pin_ParamInput)
			{
				for (int j = 0; j < InParamPin->LinkPins.size(); j++)
				{
					std::shared_ptr<FNode> BlueprintNode = InParamPin->LinkPins[j].lock()->GetOuter();
					auto TmpBP = BlueprintNode.get()->shared_from_this();

					shared_ptr<FBlueprintNode> NewBPNode = *(shared_ptr<FBlueprintNode>*)&TmpBP;
					if (NewBPNode->GetFunction()->GetNativeClass().Metas["CodeType"] == "PureFunction")
					{
						TraverseParamPureNodes(NewBPNode, OutVisitedNodes);
						OutVisitedNodes.push_back(NewBPNode);
					}
				}
			}
		}
	}
}

bool GBlueprintFunctionLibrary::Contains(shared_ptr<FNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes)
{
	for (auto& Tmp : OutVisitedNodes)
	{
		if (Tmp == Node)
		{
			return true;
		}
	}

	return false;
}

void GBlueprintFunctionLibrary::ClearBytecode(vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes)
{
	//��������ֽ��롣�ϴε��ֽ���
	for (auto& Tmp : OutVisitedNodes)
	{
		if (Tmp->GetFunction())
		{
			Tmp->GetFunction()->Script.clear();

			//�������ʵ���ڴ�
			Tmp->GetFunction()->Script.shrink_to_fit();
		}
	}
}

void GBlueprintFunctionLibrary::Compile(std::shared_ptr<FGraphicBlueprintEditor> InGraphic)
{
	if (InGraphic)
	{
		vector<shared_ptr<FBlueprintNode>> EventRootNodes = GetEventBlueprintRoot(InGraphic);
		if (EventRootNodes.size() >= 1)
		{
			//��һ�׶Σ��޼����ǵ���ͼ��ȥ�����������ӵ���ͼ
			vector<vector<shared_ptr<FBlueprintNode>>> VisitedNodes;
			PruneSolatedNodes(EventRootNodes, VisitedNodes);

			FBlueprintCompilerContext CompilerContext;
			for (auto& Tmp : VisitedNodes)
			{	
				CompilerContext.FunctionList.push_back(FBlueprintFunctionContext());
				FBlueprintFunctionContext& FunctionContext = CompilerContext.FunctionList[CompilerContext.FunctionList.size() - 1];

				//�ڶ��׶���������ֽ��롣�ϴε��ֽ���
				ClearBytecode(Tmp);

				//�����׶� �������ǵĺ������뻷���Լ����
				FunctionContext.LinearExecutionList = Tmp;
				CompilerContext.CompileFunction(FunctionContext);
			}
		}		
	}
}

std::shared_ptr<FBlueprintNode> GBlueprintFunctionLibrary::CreateBulueprintNode(
	CFunctionObject* InFunction,
	FGraphicBlueprintEditor* InGraphicBlueprint,
	const fvector_2d& InOffsetPos)
{
	string Name = GBlueprintFunctionLibrary::GetFunction(InFunction);

	return CreateBulueprintNodeByName(InFunction, InGraphicBlueprint, Name.c_str(), InOffsetPos);
}

string GBlueprintFunctionLibrary::GetFunction(CFunctionObject* InFunction)
{
	char Name[32] = { 0 };
	if (InFunction->GetNativeClass().Metas.find("DisplayName") != InFunction->GetNativeClass().Metas.end())
	{
		strcpy(Name, InFunction->GetNativeClass().Metas["DisplayName"].c_str());
	}
	else
	{
		strcpy(Name, InFunction->GetName().c_str());
	}

	return Name;
}

std::shared_ptr<FBlueprintNode> GBlueprintFunctionLibrary::CreateBulueprintNodeByName(
	CFunctionObject* InFunction,
	FGraphicBlueprintEditor* InGraphicBlueprint,
	const char* InBlueprintName,
	const fvector_2d& InOffsetPos)
{
	if (InGraphicBlueprint)
	{
		FCanvasGeometry CanvasGeometry;
		InGraphicBlueprint->StructureGeometryParam(CanvasGeometry);

		fvector_2d OffsetPos = fvector_2d();
		if (InOffsetPos.x == FLT_MIN || InOffsetPos.y == FLT_MIN)
		{
			OffsetPos.x = InGraphicBlueprint->GetGeometry()->MousePositionInCanvas.x;
			OffsetPos.y = InGraphicBlueprint->GetGeometry()->MousePositionInCanvas.y;
		}
		else
		{
			OffsetPos = InOffsetPos;
		}

		if (auto Node = InGraphicBlueprint->CreateBlueprintNode(
			OffsetPos,
			fvector_2d(
				InGraphicBlueprint->GetGeometry()->Origin.x,
				InGraphicBlueprint->GetGeometry()->Origin.y)
			, InFunction))
		{
			Node->SetName(InBlueprintName);

			Node->BuildEditor(CanvasGeometry);

			return Node;
		}
	}

	return nullptr;
}