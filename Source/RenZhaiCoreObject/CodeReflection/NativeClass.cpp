#include "NativeClass.h"
#include "../CoreObject/FunctionObject.h"
#include "../CoreObject/PropertyObject.h"
#include "../CoreObject/ArrayPropertyObject.h"
#include "../CoreObject/MapPropertyObject.h"
#include "../Archive/Archive.h"

CFunctionObject* FNativeClass::StaticFunctionList = nullptr;

FNativeClass::FNativeClass()
	:Outer(NULL)
{
	FunctionList = nullptr;
	Property = nullptr;
}

bool FNativeClass::RemoveTopProperty()
{
	if (Property)
	{
		CPropertyObject* StartPtr = Property;
		if (!StartPtr->Nest)
		{
			StartPtr->Destroy();
			Property = nullptr;
		}
		else
		{
			while (StartPtr->Nest)
			{
				if (!StartPtr->Nest->Nest)
				{
					//交给GC处理
					StartPtr->Nest->Destroy();
					StartPtr->Nest = nullptr;
				}
				else
				{
					StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Nest);
				}
			}
		}

		return true;
	}

	return false;
}

bool FNativeClass::RemoveAllProperty()
{
	if (Property)
	{
		CPropertyObject* StartPtr = Property;
		while (StartPtr)
		{
			StartPtr->Destroy();

			StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Nest);
		}

		Property = nullptr;
	}

	return false;
}

int FNativeClass::GetPropertyNumber() const
{
	int Num = 0;

	CPropertyObject* StartPtr = Property;
	while (StartPtr)
	{
		if (!StartPtr->Nest)
		{
			StartPtr = nullptr;
		}
		else
		{
			StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Nest);
		}

		Num++;
	}

	return Num;
}

CPropertyObject* FNativeClass::AddArrayProperty(
	const std::string& PropertyName,
	const std::string& InType, 
	const std::string& InValueType, 
	int InValueElementSize, 
	int InCount,
	int InElementSize,
	void* InData,
	EPropertyType InPropertyType,
	const string& InDefaultValue)
{
	if (CArrayPropertyObject* ArrayProperty = AddProperty<CArrayPropertyObject>(
		PropertyName,
		InType,
		InElementSize, 
		InData, InPropertyType, InDefaultValue))
	{
		ArrayProperty->SetValueType(InValueType);
		ArrayProperty->SetValueSize(InValueElementSize);
		ArrayProperty->SetCountValue(InCount);

		return ArrayProperty;
	}

	return nullptr;
}

CPropertyObject* FNativeClass::AddMapProperty(
	const std::string& PropertyName,
	const std::string& InType, 
	const std::string& InKeyType,
	int InKeyElementSize, 
	const std::string& InValueType,
	int InValueElementSize, 
	int InCount, 
	int InElementSize,
	void* InData,
	EPropertyType InPropertyType,
	const string& InDefaultValue)
{
	if (CMapPropertyObject* MapProperty = AddProperty<CMapPropertyObject>(
		PropertyName,
		InType, 
		InElementSize,
		InData, InPropertyType, InDefaultValue))
	{
		MapProperty->SetValueType(InValueType);
		MapProperty->SetValueSize(InValueElementSize);

		MapProperty->SetKeyType(InKeyType);
		MapProperty->SetKeySize(InKeyElementSize);

		MapProperty->SetCountValue(InCount);

		return MapProperty;
	}

	return nullptr;
}

template<class T>
inline T* FNativeClass::AddProperty(
	const std::string& PropertyName,
	const std::string& InType, 
	int InElementSize,
	void* InData,
	EPropertyType InPropertyType,
	const string& InDefaultValue)
{
	FCreateObjectParam Param;
	Param.Outer = Outer;
	T* Ptr = CreateObject<T>(Param, new T(0));

	//Ptr->SetCountValue(InCount);
	Ptr->SetElementSize(InElementSize);
	Ptr->SetTypeValue(InType);
	Ptr->InitializeValue(InData);
	Ptr->SetPropertyType(InPropertyType);
	Ptr->SetDefaultValue(InDefaultValue);

	Ptr->Rename(PropertyName);

	CFieldObject* Field = Property;
	AddList(Field, Ptr);
	if (!Property)
	{
		Property = dynamic_cast<CPropertyObject*>(Field);
	}
	
	return Ptr;
}

CPropertyObject* FNativeClass::AddProperty(
	const std::string& PropertyName, 
	const std::string& InType, 
	int InElementSize,
	void* InData,
	EPropertyType InPropertyType,
	const string& InDefaultValue)
{
	return AddProperty<CPropertyObject>(
		PropertyName,
		InType,
		InElementSize,
		InData, 
		InPropertyType,
		InDefaultValue);
}

void FNativeClass::AddClassType(const std::string& InType)
{
	InheritClassTypes.push_back(InType);
}

CPropertyObject* FNativeClass::FindProperty(const std::string& InPropertyString)
{
	CPropertyObject* StartPtr = Property;
	while (StartPtr)
	{
		if (StartPtr->GetName() == InPropertyString)
		{
			return StartPtr;
		}

		StartPtr = dynamic_cast<CPropertyObject*>(StartPtr->Nest);
	}

	return nullptr;
}

int FNativeClass::GetBytecodeFunctionNum()
{
	int BytecodeNum = 0;

	CFunctionObject* StartPtr = FunctionList;
	while (StartPtr)
	{
		//如此存在字节码 就序列化它
		if (StartPtr->Script.size() > 0)
		{
			BytecodeNum++;
		}

		if (!StartPtr->Nest)
		{
			StartPtr = nullptr;
		}
		else
		{
			StartPtr = dynamic_cast<CFunctionObject*>(StartPtr->Nest);
		}
	}

	return BytecodeNum;
}

void FNativeClass::Serialize(FArchive& OutArchive)
{
	int BytecodeFunctionNum = GetBytecodeFunctionNum();
	OutArchive << BytecodeFunctionNum;

	//序列化字节码
	CFunctionObject* StartPtr = FunctionList;
	while (StartPtr)
	{
		//如此存在字节码 就序列化它
		if (StartPtr->Script.size() > 0)
		{
			OutArchive << StartPtr->GetName();
			OutArchive << StartPtr->Script;

			BytecodeFunctionNum--;
			if (BytecodeFunctionNum <= 0)
			{
				break;
			}
		}

		if (!StartPtr->Nest)
		{
			StartPtr = nullptr;
		}
		else
		{
			StartPtr = dynamic_cast<CFunctionObject*>(StartPtr->Nest);
		}
	}

	//如果以后有自定义变量或者其他结构 按照需求序列化
	//...
}

void FNativeClass::Deserialization(FArchive& InArchive)
{
	int BytecodeFunctionNum = 0;
	InArchive >> BytecodeFunctionNum;

	int BytecodeFunctionNumCount = BytecodeFunctionNum;

	//暂时不考虑自定义函数
	for (int i = 0; i < BytecodeFunctionNum; i++)
	{
		string FunName;
		InArchive >> FunName;

		CFunctionObject* StartPtr = FunctionList;
		while (StartPtr)
		{
			if (string_equal(FunName.c_str(), StartPtr->GetName().c_str()))
			{
				InArchive >> StartPtr->Script;

				BytecodeFunctionNumCount--;
				if (BytecodeFunctionNumCount <= 0)
				{
					break;
				}
			}

			if (!StartPtr->Nest)
			{
				StartPtr = nullptr;
			}
			else
			{
				StartPtr = dynamic_cast<CFunctionObject*>(StartPtr->Nest);
			}
		}
	}

	//如果以后有自定义变量或者其他结构 按照需求反序列化
	//...
}

CFunctionObject* FNativeClass::AddStaticFunction(const string& InFunctionName)
{
	CFieldObject* Field = StaticFunctionList;
	CFunctionObject* InFunction = AddFunction_Interior(Field, Outer, InFunctionName);
	if (!StaticFunctionList)
	{
		StaticFunctionList = dynamic_cast<CFunctionObject*>(Field);
	}

	return InFunction;
}

CFunctionObject* FNativeClass::AddFunction(const string& InFunctionName)
{
	CFieldObject* Field = FunctionList;
	CFunctionObject* InFunction = AddFunction_Interior(Field, Outer, InFunctionName);
	if (!FunctionList)
	{
		FunctionList = dynamic_cast<CFunctionObject*>(Field);
	}

	return InFunction;
}

CFunctionObject* FNativeClass::FindStaticFunction(const string& InFunctionName, const string& InClassName)
{
	return FindFunction_Interior(StaticFunctionList, ResolvePackageName(InClassName),InFunctionName);
}

CFunctionObject* FNativeClass::FindFunction(const string& InFunctionName)
{
	return FindFunction_Interior(FunctionList, Outer->GetName(), InFunctionName);
}

CFunctionObject* FNativeClass::FindFunction(const string& InFunctionName, const string& InClassName)
{
	return FindFunction_Interior(FunctionList, ResolvePackageName(InClassName), InFunctionName);
}

string FNativeClass::ResolvePackageName(const string& InClassName)
{
	string ClassName = InClassName;

	//获取ClassName
	if (simple_cpp_string_algorithm::string_contain(InClassName, "'") ||
		simple_cpp_string_algorithm::string_contain(InClassName, "\""))
	{
		///Script/Engine.Actor"xxxx"
		char R[1024] = { 0 };
		char L[1024] = { 0 };
		split(ClassName.c_str(), "\"", L, R, false);

		remove_char_end(R, '\'');
		remove_char_end(R, '"');

		ClassName = R;
	}

	return ClassName;
}

void FNativeClass::AddList(CFieldObject*& InList, CFieldObject* InAddObject)
{
	if (!InList)
	{
		InList = InAddObject;
	}
	else
	{
		CFieldObject* StartPtr = InList;
		while (StartPtr)
		{
			if (!StartPtr->Nest)
			{
				StartPtr->Nest = InAddObject;

				return;
			}
			else
			{
				StartPtr = dynamic_cast<CFieldObject*>(StartPtr->Nest);
			}
		}
	}
}

CFunctionObject* FNativeClass::FindFunction_Interior(CFunctionObject*& InList, const string& InClassName, const string& InFunctionName)
{
	CFunctionObject* StartPtr = InList;
	while (StartPtr)
	{
		if (StartPtr->GetName() == InFunctionName && //函数名字是否相等
			StartPtr->GetOuter()->GetName() == InClassName)//对象名字
		{
			return StartPtr;
		}

		StartPtr = dynamic_cast<CFunctionObject*>(StartPtr->Nest);
	}

	return nullptr;
}

CFunctionObject* FNativeClass::AddFunction_Interior(
	CFieldObject*& InList, 
	CCoreMinimalObject* InOuter, 
	const string& InFunctionName)
{
	FCreateObjectParam Param;
	Param.Outer = InOuter;
	CFunctionObject* Ptr = CreateObject<CFunctionObject>(Param, new CFunctionObject(0));

	Ptr->Rename(InFunctionName);

	AddList(InList, Ptr);

	return Ptr;
}

#if EDITOR_ENGINE
bool FNativeClass::IsExitFields(const std::string& InField)
{
	return FindFields(InField) != -1;
}

int FNativeClass::FindFields(const std::string& InField)
{
	for (int i = 0; i < Fields.size(); i++)
	{
		if (Fields[i] == InField)
		{
			return i;
		}
	}

	return -1;
}

std::string* FNativeClass::FindMetas(const std::string& InKey)
{
	auto It = Metas.find(InKey);
	if (It != Metas.end())
	{
		return &It->second;
	}

	return nullptr;
}

void FNativeClass::AddMetas(const std::string& InKeyString, const std::string& InValue)
{
	if (InKeyString.length() != 0 && InValue.length() != 0)
	{
		Metas.insert({InKeyString, InValue });
	}
}

void FNativeClass::AddMetas(const string& InPropertyName, const string& InKeyString, const string& InValue)
{
	if (InKeyString.length() != 0 && InValue.length() != 0)
	{
		if (CPropertyObject* InPropertyPtr = FindProperty(InPropertyName))
		{
			InPropertyPtr->GetNativeClass().AddMetas(InKeyString, InValue);
		}
		else if (CFunctionObject* InFunctionObjectPtr = FindFunction(InPropertyName))
		{
			InFunctionObjectPtr->GetNativeClass().AddMetas(InKeyString, InValue);
		}
		else
		{
			AddMetas(InKeyString, InValue);
		}
	}
}

void FNativeClass::AddFields(const string& InPropertyName, const string& InFields)
{
	if (InFields.length() != 0)
	{
		if (CPropertyObject* InPropertyPtr = FindProperty(InPropertyName))
		{
			InPropertyPtr->GetNativeClass().AddFields(InFields);
		}
		else if (CFunctionObject* InFunctionObjectPtr = FindFunction(InPropertyName))
		{
			InFunctionObjectPtr->GetNativeClass().AddFields(InFields);
		}
		else
		{
			AddFields(InFields);
		}
	}
}

void FNativeClass::AddFields(const string& InFields)
{
	if (InFields.length() != 0)
	{
		Fields.push_back(InFields);
	}
}
#endif