#pragma once
#include "../../../Engine/EngineMinimal.h"
#include "BlueprintFunctionContext.h"

struct FCompilerBytecodeBackend
{
	FCompilerBytecodeBackend(FBlueprintFunctionContext& InContext);

public:
	void GenerateBytecode();

private:
	FBlueprintFunctionContext& Context;
};