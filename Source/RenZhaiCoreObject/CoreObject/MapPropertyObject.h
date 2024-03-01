#pragma once
#include "ContainerPropertyObject.h"

class RENZHAICOREOBJECT_API CMapPropertyObject :public CContainerPropertyObject
{
	typedef CContainerPropertyObject Super;

public:
	CMapPropertyObject(int InParamA) :Super(InParamA) {};

	virtual void* AddItem();
	virtual bool RemoveTopItem();
	virtual bool RemoveAllItem();

	bool IsExitKey(const char* InKey);
public:
	FORCEINLINE void SetKeyType(const std::string& InType) { KeyType = InType; }
	FORCEINLINE void SetKeySize(int InSize) { KeySize = InSize; }

	FORCEINLINE const std::string& GetKeyType() const { return KeyType; }
	FORCEINLINE int GetKeySize() const { return KeySize; }

public:
	CMapPropertyObject();

private:
	int KeySize;
	std::string KeyType;
};