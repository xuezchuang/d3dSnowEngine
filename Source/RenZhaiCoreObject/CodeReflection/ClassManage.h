#pragma once

#include "../RenZhaiCoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"
#include "Template.h"

class CClassObject;
struct RENZHAICOREOBJECT_API FClassID
{
	char ClassName[32];
	CClassObject* ClassObject;

	FClassID(const std::string& InNewClassName, CClassObject* InClassObject)
		:ClassObject(InClassObject)
	{
		memset(ClassName, 0, 32);
		memcpy(ClassName, InNewClassName.c_str(), InNewClassName.length());
	}
};

//默认反射模板Calss
struct RENZHAICOREOBJECT_API FClassManage :public FStaticDataBase<FClassID>
{
	static CClassObject* GetClassByIndex(int Index);
	static CClassObject* GetClass(const std::string& InClassName);

	static int GetCount() { return Count; }
	static void SetCount(int InNewCount) { Count = InNewCount; }

protected:
	static int Count;//计数 class重复
};

//实例化的
struct RENZHAICOREOBJECT_API FAssetClassManage
{
	static std::pair<string, CClassObject*> AddClass(FClassID* InClassID);
	static std::pair<string, CClassObject*> SetClass(const string& InKey, const string& InName);
protected:
	static map<string, CClassObject*> AssetClass;
};