#pragma once
#include "FieldObject.h"

class CPropertyObject;

class RENZHAICOREOBJECT_API CFunctionObject :public CFieldObject
{
	typedef CFieldObject Super;
	//friend class FNativeClass;
	//friend class CCoreMinimalObject;
	//friend struct FFrame;
public:
	CFunctionObject(){}
	CFunctionObject(int)
		:Super(0) {}


	vector<unsigned char> Script;
};