#pragma once
#include "FieldObject.h"

enum EPropertyType
{
	INPUT_PROPERTY,
	OUT_PROPERTY,
	RETURN_PROPERTY
};

class RENZHAICOREOBJECT_API CPropertyObject :public CFieldObject
{
	typedef CFieldObject Super;

public:
	CPropertyObject();
	CPropertyObject(int InParamA) :Super(InParamA) {};

	template<class T>
	FORCEINLINE T* GetData() const
	{
		return (T*)Data;
	}

public:
	FORCEINLINE int GetElementSize() const { return ElementSize; }
	FORCEINLINE void* GetData() const { return Data; }
	FORCEINLINE const string& GetType() const { return Type; }
	FORCEINLINE const string& GetDefaultValue() const { return DefaultValue; }
	FORCEINLINE EPropertyType GetPropertyType() const { return PropertyType; }

	void MemoryAllocation();
	void MemorySetAllocation();
public:
	FORCEINLINE void InitializeValue(void* InDest) { Data = (char*)InDest; }
	FORCEINLINE void SetElementSize(int InSize) { ElementSize = InSize; }
	FORCEINLINE void SetTypeValue(const string& InType) { Type = InType; }
	FORCEINLINE void SetPropertyType(EPropertyType InPropertyType) { PropertyType = InPropertyType; }
	FORCEINLINE void SetDefaultValue(const string& InDefaultValue) { DefaultValue = InDefaultValue; }

protected:
	char* Data;
	int ElementSize;
	string Type;
	string DefaultValue;
	EPropertyType PropertyType;
};