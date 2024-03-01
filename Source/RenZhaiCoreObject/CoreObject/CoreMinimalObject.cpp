#include "CoreMinimalObject.h"
#include "FunctionObject.h"
#include "PropertyObject.h"
#include "../CodeReflection/Frame.h"
#include "CoreObjectType.h"
#include "../CodeReflection/FunctionManage.h"
#include "ClassObject.h"

vector<CCoreMinimalObject*> GObjects;

CCoreMinimalObject::CCoreMinimalObject()
{
	bTick = true;
	GObjects.push_back(this);
	Outer = NULL;

	Flag = ECoreObjectFlag::PreInitialization;
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator Iter = GObjects.begin();
		Iter != GObjects.end();
		++Iter)
	{
		if (*Iter == this)
		{
			GObjects.erase(Iter);
			break;
		}
	}
}

CClassObject* CCoreMinimalObject::Class()
{
	static CClassObject* ClassObject = nullptr;
	if (!ClassObject)
	{
		FCreateObjectParam ClassParam;
		ClassObject = CreateObject<CClassObject>(ClassParam, new CClassObject(0));
		ClassObject->Rename("CoreMinimalObject");

		ClassObject->RegisterClassObjectDelegate.BindLambda([&]()->CCoreMinimalObject*
		{
			return CreateObject<CCoreMinimalObject>(FCreateObjectParam(), new CCoreMinimalObject(0));
		});
	}

	return ClassObject;
}

bool CCoreMinimalObject::IsPeriodOfDestruction() const
{
	return Flag == ECoreObjectFlag::DeletedPeriod;
}

void CCoreMinimalObject::Destroy()
{
	//Flag = 准备删除
	Flag = ECoreObjectFlag::DeletedPeriod;
}

std::string CCoreMinimalObject::GetDefaultObjectName()
{
	if (CClassObject *InClass = Class())
	{
		if (CCoreMinimalObject *InObject = InClass->GetDefaultObject())
		{
			return InObject->GetName();
		}
	}

	return GetName();
}

void CCoreMinimalObject::CallFunction(FFrame& Stack, void const* Data, CFunctionObject* Function)
{
	std::shared_ptr<FOutParmValue> ParamValue;
	simple_c_guid FuncGUID = Stack.GetGuid();

	//收集函数的参数
	CPropertyObject* PropertyPtr = Function->GetNativeClass().Property;

	int IndexPos = 0;
	unsigned char* InDataPtr = (unsigned char*)Data;

	while (PropertyPtr)
	{
		if (PropertyPtr->GetPropertyType() == EPropertyType::INPUT_PROPERTY)
		{
			if (/*PropertyPtr->GetType() == "mapp"*/0)
			{

			}
			else
			{
				Stack.OutValues.push_back(std::make_shared<FOutParmValue>());
				ParamValue = Stack.OutValues[Stack.OutValues.size() - 1];

				ParamValue->InitSize(PropertyPtr->GetElementSize(),&InDataPtr[IndexPos]);

				ParamValue->FuncID = FuncGUID;
				ParamValue->Key = PropertyPtr;

				IndexPos += PropertyPtr->GetElementSize();

				//还要加偏移 比如超过8个字节的大小 统统按照8个字节对齐
				//...
			}	
		}
		//else
		//{
		//	Stack.AddParm(PropertyPtr);
		//}

		PropertyPtr = dynamic_cast<CPropertyObject*>(PropertyPtr->Nest);
	}

	//执行字节码对应的命令
	Stack.Step(Stack.GetOuter(), nullptr);
}

void CCoreMinimalObject::CallNestFunction(FFrame& Stack, void const* Data, CFunctionObject* Function)
{
	std::shared_ptr<FOutParmValue> ReturnValue;
	simple_c_guid FuncGUID = Stack.GetGuid();

	//收集函数的参数
	CPropertyObject* PropertyPtr = Function->GetNativeClass().Property;
	while (PropertyPtr)
	{
		if (PropertyPtr->GetPropertyType() == EPropertyType::RETURN_PROPERTY)
		{
			Stack.OutValues.push_back(std::make_shared<FOutParmValue>());
			ReturnValue = Stack.OutValues[Stack.OutValues.size() - 1];

			ReturnValue->MemoryAllocation(PropertyPtr->GetElementSize());
			ReturnValue->MemorySetAllocation();

			ReturnValue->FuncID = FuncGUID;
			ReturnValue->Key = PropertyPtr;
		}
		else
		{
			Stack.AddParm(PropertyPtr);
		}

		PropertyPtr = dynamic_cast<CPropertyObject*>(PropertyPtr->Nest);
	}

	//执行本地函数
	if (FNativeFuntionPtr FuntionPtr = FFuntionManage::GetNativeFuncPtr(Function))
	{
		if (ReturnValue)
		{
			FuntionPtr(Stack.GetOuter(), Stack, ReturnValue->Data);
		}
		else
		{
			FuntionPtr(Stack.GetOuter(), Stack, nullptr);
		}
	}

	//执行字节码对应的命令
	Stack.Step(Stack.GetOuter(), Data);
}

void CCoreMinimalObject::ExecutionScript(CFunctionObject* Function, void const* Data)
{
	if (Function)
	{
		assert(Function);

		if (Function->Script.size() == 0)
		{
			return;
		}

		FFrame Stack(Function);

		//执行字节码对应的命令
		Stack.Step(this, Data);
	}
}

CFunctionObject* CCoreMinimalObject::FindScriptFuntion(const std::string& FunName)
{
	return NativeClass.FindFunction(FunName);
}

CFunctionObject* CCoreMinimalObject::FindScriptStaticFuntion(const std::string& FunName, const std::string& InClassName)
{
	return FNativeClass::FindFunction_Interior(FNativeClass::StaticFunctionList, InClassName, FunName);
}

CFunctionObject* CCoreMinimalObject::FindScriptStaticFuntion(const std::string& FunName)
{
	return nullptr;
}
