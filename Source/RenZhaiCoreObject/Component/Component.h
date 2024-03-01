#pragma once

#include "../CoreObject/CoreMinimalObject.h"

class RENZHAICOREOBJECT_API CComponent :public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;
public:
	CComponent();
	CComponent(int InParamA) :Super(InParamA) {};
	void SetParentComponent(CComponent* InParent) { Parent = InParent; }
	void AddChildren(CComponent* InChildren);

	template<class T>
	void CallChildren(std::function<void(T*)> Func)
	{
		for (auto& Tmp : Children)
		{
			Func(dynamic_cast<T*>(Tmp));
		}
	}

public:
	FORCEINLINE std::vector<CComponent*>& GetChildrens(){ return Children; }

protected:
	CComponent* Parent;
	std::vector<CComponent*> Children;
};