#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FBoolDetailsMapping :public FPropertyDetailsMapping
{
public:
	virtual bool UpdateDetailsWidget(CPropertyObject* InProperty);

public:
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};