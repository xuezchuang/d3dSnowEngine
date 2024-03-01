#pragma once
#include "ContainerDetailsMapping.h"

class FArrayDetailsMapping :public FContainerDetailsMapping
{
	typedef FContainerDetailsMapping Super;

public:
	virtual bool UpdateDetailsWidget(CPropertyObject* InProperty);
	virtual	void UpdateWidget(CPropertyObject* InProperty);
public:
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};