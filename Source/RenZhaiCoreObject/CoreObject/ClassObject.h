#pragma once
#include "FieldObject.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FRegisterClassObjectDelegate, CCoreMinimalObject*)

//提供核心对象
class RENZHAICOREOBJECT_API CClassObject :public CFieldObject
{
	typedef CFieldObject Super;

public:
	FRegisterClassObjectDelegate RegisterClassObjectDelegate;
public:
	CClassObject(int);

public:
	CCoreMinimalObject* GetDefaultObject();

public:
	CCoreMinimalObject* DefaultObject;
};