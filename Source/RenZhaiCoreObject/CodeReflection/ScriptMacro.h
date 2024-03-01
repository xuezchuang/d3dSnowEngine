#pragma once

#define FUNCTION_DEFINITION(Func) static void Func(CCoreMinimalObject* Context, FFrame& Stack,void const *RefData)
#define FUNCTION_IMPLEMENTATION(Func) void Func(CCoreMinimalObject* Context, FFrame& Stack,void const *RefData)

#define ADD_SCRIPT_FUNCTION(BytecodeIndex)\
static int BytecodeIndex##_Native = GScriptRegisterNative((int)EScriptCommand::##BytecodeIndex,&CCoreMinimalObject::##BytecodeIndex);