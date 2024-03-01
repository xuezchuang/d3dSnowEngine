#include "ObjectGraphicBlueprintEditor.h"
#include "../Node/BlueprintNode/ObjectBlueprintNode.h"
#include "../Node/BlueprintNode/EventBlueprintNode.h"
#include "../Node/BlueprintNode/FunctionBlueprintNode.h"
#include "../Node/BlueprintNode/PureFunctionBlueprintNode.h"
#include "../../BlueprintConfigInfo.h"
#include "../../Core/BlueprintConnection/BlueprintDrawConnection.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionManage.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionType.h"
#include "../Menu/ObjectBlueprintNodeMenuEditor.h"
#include "CoreObject/FunctionObject.h"

FObjectGraphicBlueprintEditor::FObjectGraphicBlueprintEditor()
	:Super()
{
	Menu = std::make_shared<FObjectBlueprintNodeMenuEditor>();
}

void FObjectGraphicBlueprintEditor::BuildEditor()
{
	Super::BuildEditor();

	Menu->SetGraphicBlueprintEditor(this);

	Menu->BuildEditor();
}

void FObjectGraphicBlueprintEditor::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);

	Menu->DrawEditor(DeltaTime);
}

void FObjectGraphicBlueprintEditor::ExitEditor()
{
	Super::ExitEditor();

	Menu->ExitEditor();
}

std::shared_ptr<FBlueprintNode> FObjectGraphicBlueprintEditor::CreateBlueprintNode(
	const fvector_2d& InNewOffsetPosition,
	const fvector_2d& InNewOriginPosition,
	CFunctionObject* InFunction)
{
	string CodeType =InFunction->GetNativeClass().Metas["CodeType"];
	if (CodeType == "Event")
	{
		Nodes.push_back(std::make_shared<FEventBlueprintNode>());
	}
	else if (CodeType == "Function")
	{
		Nodes.push_back(std::make_shared<FFunctionBlueprintNode>());
	}
	else if (CodeType == "PureFunction")
	{
		Nodes.push_back(std::make_shared<FPureFunctionBlueprintNode>());
	}
	else
	{
		Nodes.push_back(std::make_shared<FObjectBlueprintNode>());
	}
	
	auto& Node = Nodes[Nodes.size() - 1];
	
	Node->SetStandardOffsetPosition(InNewOffsetPosition / GetZoomRatio());
	Node->SetOffsetPosition(InNewOffsetPosition);
	Node->SetOriginPosition(InNewOriginPosition);
	Node->SetSize(FBlueprintConfigInfo::Get()->BlueprintNodeSize * GetZoomRatio());

	Node->SetFunction(InFunction);

	return Node;
}

std::shared_ptr<FBlueprintDrawConnection> FObjectGraphicBlueprintEditor::MakePinConnection()
{
	auto BlueprintDrawConnection = make_shared<FBlueprintDrawConnection>();
	BlueprintDrawConnection->SetConnectionType(OBJECT_BLUEPRINT);
	
	return BlueprintDrawConnection;
}
