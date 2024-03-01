#include "BlueprintCompilerContext.h"
#include "BlueprintFunctionContext.h"
#include "../Core/BlueprintNode/BlueprintNode.h"
#include "../Core/BlueprintNode/BlueprintPin.h"
#include "BlueprintCompiledStatement.h"
#include "Terminal.h"
#include "CompilerBytecodeBackend.h"
#include "CoreObject/PropertyObject.h"
#include "CoreObject/FunctionObject.h"

void GetCurrentParamTerminal(
	FFunctionTerminal& FunctionTerminal,
	std::shared_ptr<FBlueprintPin> Pin,const simple_c_guid& InGUID)
{
	if (Pin->GetType() != EBlueprintPinType::Pin_Output &&
		Pin->GetType() != EBlueprintPinType::Pin_Input)
	{
		FunctionTerminal.Name = Pin->GetName();
		FunctionTerminal.Type = Pin->GetProperty()->GetType();
		FunctionTerminal.DefaultValue = Pin->GetProperty()->GetDefaultValue();
		FunctionTerminal.PropertyType = Pin->GetProperty()->GetPropertyType();

		FunctionTerminal.FuncGUID = InGUID;

		if (FunctionTerminal.PropertyType == EPropertyType::INPUT_PROPERTY)
		{
			if (vector<std::weak_ptr<FNode>>* InLinks = Pin->GetLinkPins())
			{
				if (InLinks->size() > 0)
				{
					if (FBlueprintPin* InLinkPin = dynamic_cast<FBlueprintPin*>((*InLinks)[0].lock().get()))
					{
						if (FBlueprintNode* InBlueprintNode = dynamic_cast<FBlueprintNode*>(InLinkPin->GetOuter().get()))
						{
							if (InBlueprintNode->GetFunction())
							{
								if (InLinkPin->GetProperty())
								{
									FunctionTerminal.LinkParam = std::make_shared<FLinkParam>();
									FunctionTerminal.LinkParam->FuncGUID = InBlueprintNode->GetGuid();
									FunctionTerminal.LinkParam->ParamName = InLinkPin->GetProperty()->GetName();
								}
							}
						}
					}
				}
			}
		}
	}
}

void FBlueprintCompilerContext::CompileFunction(FBlueprintFunctionContext& Context)
{
	shared_ptr<FBlueprintNode> EndNode = nullptr;
	for (int i = 0; i < Context.LinearExecutionList.size(); i++)
	{
		shared_ptr<FBlueprintNode>& InNode = Context.LinearExecutionList[i];
		if (InNode->GetFunction())
		{
			if (!Context.ListStatement[InNode])
			{
				Context.ListStatement[InNode] = std::make_shared<vector<EBlueprintStatementType>>();
			}
			//编译语句
			if (i == 0)
			{
				Context.ListStatement[InNode]->push_back(EBlueprintStatementType::BS_CallFunc);
			}

			Context.ListStatement[InNode]->push_back(EBlueprintStatementType::BS_Nest);

			EndNode = InNode;

			//收集端子
			std::shared_ptr<vector<FFunctionTerminal>> TerminalArray = std::make_shared<vector<FFunctionTerminal>>();
			for (auto& Tmp : InNode->Pins)
			{
				TerminalArray->push_back(FFunctionTerminal());
				GetCurrentParamTerminal((*TerminalArray)[TerminalArray->size()-1],Tmp,InNode->GetGuid());
			}

			if (TerminalArray->size() > 0)
			{
				auto FuncGuid = std::make_shared<simple_c_guid>();
				
				*FuncGuid = InNode->GetGuid();

				Context.TerminalMap.insert({ FuncGuid,TerminalArray });
			}
		}
	}

	if (EndNode)
	{
		Context.ListStatement[EndNode]->push_back(EBlueprintStatementType::BS_Return);
	}

	FCompilerBytecodeBackend CompilerBytecodeBackend(Context);

	//生成字节码
	CompilerBytecodeBackend.GenerateBytecode();
}
