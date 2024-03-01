#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"
#include "ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"
#include "CoreObject/FunctionObject.h"
#include "../CoreObject/PropertyObject.h"
#include "../CoreObject/ClassObject.h"

FNativeFuntionPtr GScriptNatives[Script_Max];

int GScriptInitNative()
{
	for (int i = 0; i < Script_Max; i++)
	{
		GScriptNatives[i] = &CCoreMinimalObject::Script_Undefined;
	}

	return 0;
}
static int ScriptInitNative = GScriptInitNative();

int GScriptRegisterNative(int NativeBytecodeIndex,const FNativeFuntionPtr &Func)
{
	if (NativeBytecodeIndex != -1)
	{
		if (NativeBytecodeIndex > Script_Undefined && NativeBytecodeIndex < Script_Max)
		{
			GScriptNatives[NativeBytecodeIndex] = Func;

			return 0;
		}
	}

	return -1;
}

FFrame::FFrame()
{
	CursorIndex = 0;
}

FFrame::FFrame(CFunctionObject* InNewFunction)
	:CursorIndex(0)
	,Code(InNewFunction->Script.data())
	,Function(InNewFunction)
	,Outer(InNewFunction->GetOuter())
{
	 
}

float FFrame::ReadFloat()
{
	return Read<float>();
}

int FFrame::ReadInt()
{
	return Read<int>();
}

void FFrame::AddParm(CPropertyObject* Addr)
{
	std::shared_ptr<FOutParm> ListParm = OutParm;

	if (!ListParm.get())
	{
		OutParm = std::make_shared<FOutParm>();
		OutParm->PropAddr = Addr;
	}
	else
	{
		while (ListParm->Nest.get())
		{
			ListParm = ListParm->Nest;
		}

		ListParm->Nest = std::make_shared<FOutParm>();
		ListParm->Nest->PropAddr = Addr;
	}
}

void FFrame::Step(CCoreMinimalObject* Context, void const* RefData)
{
	int B = (*this)++;
	(GScriptNatives[B])(Context, *this, RefData);
}

string FFrame::ReadString()
{
	string RefData((char*)Code);

	//到了‘/0’结束
	while (*Code != '\0')
	{
		(*this)++;
	}

	(*this)++;//跳出'/0'

	return RefData;
}

simple_c_guid FFrame::GetGuid()
{
	simple_c_guid InPtr = *(simple_c_guid*)Code;
	*this += sizeof(simple_c_guid);

	return InPtr;
}

EPropertyType FFrame::GetPropertyType()
{
	EPropertyType InPtr = *(EPropertyType*)Code;
	*this += sizeof(EPropertyType);

	return InPtr;
}

CFunctionObject* FFrame::ReadFunction()
{
	string FunName = ReadString();
	string ClassName = ReadString();

	if (Function)
	{
		if (Function->GetName() == FunName &&
			Function->GetOuter()->GetName() == ClassName)
		{
			return Function;
		}
	}

	//先找成员函数
	if (Outer->Class()->GetName() == ClassName)//保证你的静态和成员如果有相同名字，也不会出错
	{
		if (CFunctionObject* InFunction = Outer->GetNativeClass().FindFunction(FunName))
		{
			return InFunction;
		}
	}
	
	//静态函数
	return CCoreMinimalObject::FindScriptStaticFuntion(FunName, ClassName);;
}

void FFrame::Iteration(int InSize)
{
	CursorIndex += InSize;//Debug
	Code += InSize;
}

unsigned char* FFrame::GetParmAddr()
{
	CPropertyObject* Proper = OutParm->PropAddr;
	OutParm = OutParm->Nest;

	std::shared_ptr<FOutParmValue> OutParmValue;

	if (Proper->GetPropertyType() == EPropertyType::OUT_PROPERTY)
	{
		OutValues.push_back(std::make_shared<FOutParmValue>());
		OutParmValue = OutValues[OutValues.size() - 1];

		OutParmValue->Key = Proper;

		OutParmValue->MemoryAllocation(Proper->GetElementSize());
		OutParmValue->MemorySetAllocation();

		OutParmValue->FuncID = GetGuid();

		(*this)++;
	}
	else if (Proper->GetPropertyType() == EPropertyType::INPUT_PROPERTY)
	{
		//第一种是默认值 第二种是其他对象计算出来后的传入值
		simple_c_guid OwnerFuncGuid = GetGuid();

		bool bLinkValue = (*this)++;

		//链接值
		if (bLinkValue)
		{
			simple_c_guid LinkFuncGuid = GetGuid();
			string LinkFuncParamName = ReadString();

			for (auto& Tmp : OutValues)
			{
				if (guid_equal(&Tmp->FuncID, &LinkFuncGuid))
				{
					if (string_equal(Tmp->Key->GetName().c_str(), LinkFuncParamName.c_str()))
					{
						OutParmValue = Tmp;
						break;
					}
				}
			}
		}
		else //默认值
		{
			OutParmValue = std::make_shared<FOutParmValue>();

			OutParmValue->Key = Proper;
			OutParmValue->MemoryAllocation(Proper->GetElementSize());
			OutParmValue->MemorySetAllocation();
		}
	}

	Step(Outer, OutParmValue->Data);

	return OutParmValue->Data;

	//unsigned char* LocalResult = (unsigned char*)Proper->GetData();
	//if (!LocalResult)
	//{
	//	Proper->MemoryAllocation();
	//	LocalResult = (unsigned char*)Proper->GetData();
	//}
	//Proper->MemorySetAllocation();
	//
	//Step(NULL, LocalResult);

	//return LocalResult;
}

FOutParm::FOutParm()
	:PropAddr(nullptr)
{
}

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Undefined)
{

}

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Vector3D)
{
	EPropertyType PropertyType = Stack.GetPropertyType();
	if (PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = Stack++;
		if (!bLink)
		{
			string Vector3DString = Stack.ReadString();

			vector<string> Vector3DElementValue;
			simple_cpp_string_algorithm::parse_into_vector_array(Vector3DString.c_str(), Vector3DElementValue, ",");
		
			((fvector_3d*)RefData)->x = atof(Vector3DElementValue[0].c_str());
			((fvector_3d*)RefData)->y = atof(Vector3DElementValue[1].c_str());
			((fvector_3d*)RefData)->z = atof(Vector3DElementValue[2].c_str());
		}
	}
}
ADD_SCRIPT_FUNCTION(Script_Vector3D);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Int)
{
	EPropertyType PropertyType = Stack.GetPropertyType();
	if (PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = Stack++;
		if (!bLink)
		{
			*(int*)RefData = Stack.ReadInt();
		}
	}
}
ADD_SCRIPT_FUNCTION(Script_Int);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_False)
{
	EPropertyType PropertyType = Stack.GetPropertyType();
	if (PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = Stack++;
		if (!bLink)
		{
			*(bool*)RefData = false;
		}
	}
}
ADD_SCRIPT_FUNCTION(Script_False);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_True)
{
	EPropertyType PropertyType = Stack.GetPropertyType();
	if (PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = Stack++;
		if (!bLink)
		{
			*(bool*)RefData = true;
		}
	}
}
ADD_SCRIPT_FUNCTION(Script_True);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Float)
{
	EPropertyType PropertyType = Stack.GetPropertyType();
	if (PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = Stack++;
		if (!bLink)
		{
			*(float*)RefData = Stack.ReadFloat();
		}
	}
}
ADD_SCRIPT_FUNCTION(Script_Float);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Funtion)
{
	CCoreMinimalObject::CallFunction(Stack,RefData,Stack.ReadFunction());
}
ADD_SCRIPT_FUNCTION(Script_Funtion);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_NestFunction)
{
	CCoreMinimalObject::CallNestFunction(Stack, RefData, Stack.ReadFunction());
	//CCoreMinimalObject::ExecutionScript(Stack.ReadFunction(), RefData);
}
ADD_SCRIPT_FUNCTION(Script_NestFunction);

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_ConstString)
{
	EPropertyType PropertyType = Stack.GetPropertyType();
	if (PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = Stack++;
		if (!bLink)
		{
			*(string*)RefData = (char*)Stack.GetCode();

			//到了‘/0’结束
			while (*Stack.GetCode() != '\0')
			{
				Stack++;
			}

			Stack++;//跳出'/0'
		}
	}
}
ADD_SCRIPT_FUNCTION(Script_ConstString);

void CCoreMinimalObject::InitMinimalObject()
{
	NativeClass.Outer = this;

	InitReflectionContent();
}

FOutParmValue::FOutParmValue(int InDataSize, void* InData)
{
	InitSize(InDataSize, InData);
}

FOutParmValue::~FOutParmValue()
{
	free(Data);
}

void FOutParmValue::InitSize(int InDataSize, void* InData)
{
	if (InDataSize > 0)
	{
		Data = (unsigned char*)malloc(InDataSize);

		memset(Data, 0, InDataSize);

		memcpy(Data, InData, InDataSize);
	}
}

void FOutParmValue::MemoryAllocation(int InSize)
{
	DataSize = InSize;
	Data = (unsigned char*)malloc(InSize);
}

void FOutParmValue::MemorySetAllocation()
{
	memset(Data, 0, DataSize);
}
