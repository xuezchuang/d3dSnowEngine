#pragma once
#include "FieldObject.h"

class CPropertyObject;

class RENZHAICOREOBJECT_API CFunctionObject :public CFieldObject
{
	typedef CFieldObject Super;
public:
	CFunctionObject(){}
	CFunctionObject(int)
		:Super(0) {}

	vector<unsigned char> Script;
};