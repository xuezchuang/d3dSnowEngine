#pragma once
#include "CoreMinimalObject.h"

//�ṩ���Ķ���
class RENZHAICOREOBJECT_API CFieldObject :public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;
public:
	CFieldObject();
	CFieldObject(int)
		:Super(0){}

	CFieldObject* Nest;
};