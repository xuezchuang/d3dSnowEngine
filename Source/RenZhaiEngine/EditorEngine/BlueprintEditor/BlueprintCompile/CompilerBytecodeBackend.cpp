#include "CompilerBytecodeBackend.h"
#include "../Core/BlueprintNode/BlueprintNode.h"
#include "../Core/BlueprintNode/BlueprintPin.h"
#include "CoreObject/FunctionObject.h"
#include "CoreObject/PropertyObject.h"
#include "BlueprintCompiledStatement.h"
#include "Terminal.h"
#include "ScriptBuilder.h"

FCompilerBytecodeBackend::FCompilerBytecodeBackend(FBlueprintFunctionContext& InContext)
	:Context(InContext)
{
}

void FCompilerBytecodeBackend::GenerateBytecode()
{
	//���ֽ���洢���¼�����
	FScriptBuilder Builder(Context.LinearExecutionList[0]->GetFunction()->Script);

	for (auto& Key : Context.LinearExecutionList)
	{
		auto& Tmp = Context.ListStatement[Key];
		shared_ptr<FBlueprintNode> Node = Key;

		if (Node->GetFunction())
		{	
			for (EBlueprintStatementType& InStatement : *Tmp)
			{
				switch (InStatement)
				{
					case BS_CallFunc:
					{
						Builder.Writer << EScriptCommand::Script_Funtion;
						Builder.Writer << Node->GetFunction();
						Builder.Writer << Node->GetGuid();
						break;
					}
					case BS_Nest:
					{
						if (Node->GetType() != EBlueprintNodeType::Event_BlueprintNode)//�ų�Event Node
						{
							//���л�����
							CPropertyObject* StartProperty = Node->GetFunction()->GetNativeClass().Property;
							while (StartProperty)
							{
								if (StartProperty->GetPropertyType() != EPropertyType::RETURN_PROPERTY)
								{
									FFunctionTerminal Terminal;
									if (Context.FindTerminal(Node->GetGuid(), StartProperty, Terminal))
									{
										Builder.SerializeTerminal(StartProperty, Terminal);
									}
								}

								StartProperty = dynamic_cast<CPropertyObject*>(StartProperty->Nest);
							}
						}

						//�ƶ�����һ�����ӵĺ��� 
						int Index = 0;//��Ϊ��һ������ �����ҵ���ǰ����ͼ����λ�������б���ĸ�λ��
						for (int i = 0; i < Context.LinearExecutionList.size(); i++)
						{
							if (Node == Context.LinearExecutionList[i])
							{
								Index = i;
								break;
							}
						}

						Index++;
						if (Index < Context.LinearExecutionList.size())
						{
							auto NextNode = Context.LinearExecutionList[Index];
							Builder.Writer << EScriptCommand::Script_NestFunction;
							Builder.Writer << NextNode->GetFunction();
							Builder.Writer << NextNode->GetGuid();
						}

						break;
					}
					case BS_Return:
					{
						break;
					}
				}
			}
		}
	}
}
