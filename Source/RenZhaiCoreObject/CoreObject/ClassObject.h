#pragma once
#include "FieldObject.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FRegisterClassObjectDelegate, CCoreMinimalObject*)

//�ṩ���Ķ���
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