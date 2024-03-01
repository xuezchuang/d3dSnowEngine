#include "FunctionManage.h"
#include "../CoreObject/FunctionObject.h"

namespace NativeFuntionMess
{
	void Error(CCoreMinimalObject* Context, FFrame& TheStack, void const* RefData)
	{
		assert(0);
	}

	void Empty(CCoreMinimalObject* Context, FFrame& TheStack, void const* RefData)
	{
	}
}

FNativeFuntionPtr ErrorFunction = NativeFuntionMess::Error;
FNativeFuntionPtr EmptyFunction = NativeFuntionMess::Empty;

FNativeFuntionPtr FFuntionManage::GetNativeFuncPtr(CFunctionObject* Function)
{
	char* Ptr = (char*)Data;
	for (size_t i = 0; i < DataNumber; i++)
	{
		FFuntionID* CurrFuntionID = (FFuntionID*)&Ptr[i * GetTypeSize()];
		if (string_equal(CurrFuntionID->FuncName, Function->GetName().c_str()) &&
			string_equal(CurrFuntionID->ClassName, Function->GetOuter()->GetDefaultObjectName().c_str()))
		{
			return CurrFuntionID->NativeFuncPtr;
		}
	}

	return NULL;
}