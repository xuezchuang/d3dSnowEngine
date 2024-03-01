#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FStringDetailsMapping :public FPropertyDetailsMapping
{
public:
	virtual bool UpdateDetailsWidget(CPropertyObject* InProperty);

	static int StringCallback(ImGuiInputTextCallbackData* InData);
public:
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};