#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FContainerDetailsMapping :public FPropertyDetailsMapping
{
public:
	FContainerDetailsMapping();

public:
	virtual bool UpdateDetailsWidget(CPropertyObject* InProperty);
	virtual	void UpdateWidget(CPropertyObject* InProperty);

	virtual	void UpdateEditorContainerProperty(CPropertyObject* InProperty);
protected:
	std::string ContainerName;
};