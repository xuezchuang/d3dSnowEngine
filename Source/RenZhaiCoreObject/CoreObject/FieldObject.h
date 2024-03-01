#pragma once
#include "CoreMinimalObject.h"

//提供核心对象
class RENZHAICOREOBJECT_API CFieldObject :public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;
public:
	CFieldObject();
	CFieldObject(int)
		:Super(0){}

	CFieldObject* Nest;
};