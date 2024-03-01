#pragma once
#include "../../../Engine/EngineMinimal.h"

class FBlueprintNode;
enum EBlueprintStatementType;
struct FFunctionTerminal;
class CFunctionObject;
class CPropertyObject;
struct FBlueprintFunctionContext
{
	//����ִ���б�
	vector<shared_ptr<FBlueprintNode>> LinearExecutionList;

	//�б��Ӧ�����ִ��
	map<shared_ptr<FBlueprintNode>, std::shared_ptr<vector<EBlueprintStatementType>>> ListStatement;

	//����ӳ��
	map<shared_ptr<simple_c_guid>,std::shared_ptr<vector<FFunctionTerminal>>> TerminalMap;

public:
	bool FindTerminal(const simple_c_guid& InFunctionObjectKey, CPropertyObject* InProperty, FFunctionTerminal& OutTerminal);
	std::shared_ptr<vector<FFunctionTerminal>> *FindTerminals(const simple_c_guid& InFunctionObjectKey);
};