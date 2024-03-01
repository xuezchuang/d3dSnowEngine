#include "FunctionManage.h"
#include "../CoreObject/FunctionObject.h"
#include "ClassManage.h"
#include "CoreObject/ClassObject.h"

map<string, CClassObject*> FAssetClassManage::AssetClass;
int FClassManage::Count = 0;

//补充代码(课程中未讲解)
bool NameEqual(const char* InStringA, const char* InStringB)
{
	int len1 = strlen(InStringA);
	int len2 = strlen(InStringB);

	if (len1 == len2 && (len2 != 0))
	{
		for (int i = 0; i < len1; i++)
		{
			if (InStringA[i] != InStringB[i])
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

CClassObject* FClassManage::GetClassByIndex(int Index)
{
	if (FClassID* CurrClassID = Get(Index))
	{
		return CurrClassID->ClassObject;
	}

	return nullptr;
}

CClassObject* FClassManage::GetClass(const std::string& InClassName)
{
	char* Ptr = (char*)Data;
	for (size_t i = 0; i < DataNumber; i++)
	{
		FClassID* CurrClassID = (FClassID*)&Ptr[i * GetTypeSize()];
		if(NameEqual(CurrClassID->ClassName, InClassName.c_str()))
		//if (string_equal(CurrClassID->ClassName, InClassName.c_str()))//报错 静态不准访问
		//if(CurrClassID->ClassName == InClassName)//字符串是相等 但是判定确实错误
		{
			return CurrClassID->ClassObject;
		}
	}

	return nullptr;
}

std::pair<string, CClassObject*> FAssetClassManage::AddClass(FClassID* InClassID)
{
	std::pair<string, CClassObject*> InPair;
	if (InClassID)
	{
		InPair.first = simple_cpp_string_algorithm::printf("%s_%i",InClassID->ClassName,FClassManage::GetCount());
	
		FCreateObjectParam ClassParam;
		InPair.second = CreateObject<CClassObject>(ClassParam, new CClassObject(0));
		InPair.second->Rename(InPair.first);

		InPair.second->RegisterClassObjectDelegate = InClassID->ClassObject->RegisterClassObjectDelegate;
		
		auto Tmp = AssetClass.insert(InPair);

		FClassManage::SetCount(FClassManage::GetCount() + 1);
	}

	return InPair;
}

//Class_01
std::pair<string, CClassObject*> FAssetClassManage::SetClass(const string& InKey, const string& InName)
{
	std::pair<string, CClassObject*> InPair;
	if (CClassObject* InClass = FClassManage::GetClass(InKey))
	{
		InPair.first = InName;

		FCreateObjectParam ClassParam;
		InPair.second = CreateObject<CClassObject>(ClassParam, new CClassObject(0));
		InPair.second->Rename(InPair.first);

		InPair.second->RegisterClassObjectDelegate = InClass->RegisterClassObjectDelegate;

		auto Tmp = AssetClass.insert(InPair);

		//计数
		{
			char R[1024] = { 0 };
			char L[1024] = { 0 };
			split(InName.c_str(), "_", L, R, false);

			int LastCount = atoi(R);
			if (FClassManage::GetCount() < LastCount)
			{
				FClassManage::SetCount(LastCount);
			}
			else if (FClassManage::GetCount() == LastCount)
			{
				FClassManage::SetCount(LastCount + 1);
			}
		}
	}

	return InPair;
}
