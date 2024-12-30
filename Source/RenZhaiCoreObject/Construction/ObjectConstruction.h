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
	//����ǲ������ ����������������ע��
	ConstructionComponent::ConstructionComponents(InObjectParam, NewObject);

	T* Obj = dynamic_cast<T*>(NewObject);

	Obj->SetFlag(Initialization);

	Obj->SetOuter(InObjectParam.Outer);

	Obj->InitMinimalObject();

	Obj->SetFlag(PostInitialization);

	//����Class
	RZ_UpdateClass(InObjectParam.Class, Obj);

	return Obj;
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(const FCreateObjectParam& InObjectParam, ParamTypes &&...Params)
{
	return CreateObject<T>(InObjectParam,new T(Params...));
}
