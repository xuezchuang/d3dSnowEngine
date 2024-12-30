#pragma once

#include "../RenZhaiCoreObjectMacro.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "ConstructionComponents.h"
#include "../CoreObject/CoreObjectType.h"

class CCoreMinimalObject;
class CComponent;
class CClassObject;

struct RENZHAICOREOBJECT_API FCreateObjectParam
{
	FCreateObjectParam()
		:Outer(nullptr)
		,Class(NULL)
		,ParentComponent(NULL)
	{}

	CCoreMinimalObject* Outer;
	CClassObject* Class;
	CComponent* ParentComponent;
};

void RENZHAICOREOBJECT_API RZ_UpdateClass(CClassObject* InClass, CCoreMinimalObject* InObject);

template<class T>
T* CreateObject(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject)
{
	//检测是不是组件 是组件按照组件规则注册
	ConstructionComponent::ConstructionComponents(InObjectParam, NewObject);

	T* Obj = dynamic_cast<T*>(NewObject);

	Obj->SetFlag(Initialization);

	Obj->SetOuter(InObjectParam.Outer);

	Obj->InitMinimalObject();

	Obj->SetFlag(PostInitialization);

	//更新Class
	RZ_UpdateClass(InObjectParam.Class, Obj);

	return Obj;
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(const FCreateObjectParam& InObjectParam, ParamTypes &&...Params)
{
	return CreateObject<T>(InObjectParam,new T(Params...));
}
