#pragma once
#include "../Core/ClassDetailsMapping.h"

class FActorObjectDetailsMapping :public FClassDetailsMapping
{
public:
	virtual void UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject);

	void ShowPlaceholderObject(CComponent* InComponent, int ID);
public:
	static shared_ptr<FClassDetailsMapping> MakeDetailsMapping();
};