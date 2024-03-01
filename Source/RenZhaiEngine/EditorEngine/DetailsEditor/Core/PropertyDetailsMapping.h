#pragma once
#include "DetailsMapping.h"
#include "CoreObject/PropertyObject.h"

class CPropertyObject;

class FPropertyDetailsMapping :public FDetailsMapping
{
public:
	virtual bool UpdateDetailsWidget(CPropertyObject* InProperty);

public:
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping() { return nullptr; }
};