#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FXMFLOAT3DetailsMapping :public FPropertyDetailsMapping
{
public:
	virtual bool UpdateDetailsWidget(CPropertyObject* InProperty);


public:
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};