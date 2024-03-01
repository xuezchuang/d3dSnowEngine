#pragma once
#include "../../../Engine/EngineMinimal.h"

struct FBlueprintFunctionContext;
struct FBlueprintCompilerContext
{
	std::vector<FBlueprintFunctionContext> FunctionList;

	void CompileFunction(FBlueprintFunctionContext& Context);
};