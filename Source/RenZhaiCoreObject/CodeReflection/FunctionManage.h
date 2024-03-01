#pragma once

#include "../RenZhaiCoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"
#include "Template.h"

class CFunctionObject;

struct RENZHAICOREOBJECT_API FFuntionID
{
	char ClassName[32];
	char FuncName[32];

	FNativeFuntionPtr NativeFuncPtr;

	FFuntionID(const char* InNewClassName, const char* InNewFuncName, FNativeFuntionPtr NewNativeFuncPtr)
		: NativeFuncPtr(NewNativeFuncPtr)
	{
		memset(ClassName, 0, 32);
		memset(FuncName, 0, 32);

		strcpy_s(ClassName, InNewClassName);
		strcpy_s(FuncName, InNewFuncName);
	}
};

struct RENZHAICOREOBJECT_API FFuntionManage :public FStaticDataBase<FFuntionID>
{
	static FNativeFuntionPtr GetNativeFuncPtr(CFunctionObject* Function);
};