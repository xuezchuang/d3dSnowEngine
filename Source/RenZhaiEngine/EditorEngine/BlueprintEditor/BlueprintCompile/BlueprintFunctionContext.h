#pragma once
#include "../../../Engine/EngineMinimal.h"

class FBlueprintNode;
enum EBlueprintStatementType;
struct FFunctionTerminal;
class CFunctionObject;
class CPropertyObject;
struct FBlueprintFunctionContext
{
	//线性执行列表
	vector<shared_ptr<FBlueprintNode>> LinearExecutionList;

	//列表对应的语句执行
	map<shared_ptr<FBlueprintNode>, std::shared_ptr<vector<EBlueprintStatementType>>> ListStatement;

	//端子映射
	map<shared_ptr<simple_c_guid>,std::shared_ptr<vector<FFunctionTerminal>>> TerminalMap;

public:
	bool FindTerminal(const simple_c_guid& InFunctionObjectKey, CPropertyObject* InProperty, FFunctionTerminal& OutTerminal);
	std::shared_ptr<vector<FFunctionTerminal>> *FindTerminals(const simple_c_guid& InFunctionObjectKey);
};