#pragma once
#include "ContainerPropertyObject.h"

class RENZHAICOREOBJECT_API CArrayPropertyObject :public CContainerPropertyObject
{
	typedef CContainerPropertyObject Super;

public:
	CArrayPropertyObject(int InParamA) :Super(InParamA) {};

	virtual void* AddItem();
	virtual bool RemoveTopItem();
	virtual bool RemoveAllItem();
public:
	CArrayPropertyObject();
};