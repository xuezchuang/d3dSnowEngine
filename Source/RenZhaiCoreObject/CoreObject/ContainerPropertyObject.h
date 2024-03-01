#pragma once
#include "PropertyObject.h"

class RENZHAICOREOBJECT_API CContainerPropertyObject :public CPropertyObject
{
	typedef CPropertyObject Super;

public:
	CContainerPropertyObject();
	CContainerPropertyObject(int InParamA) :Super(InParamA) {};
public:
	virtual void* AddItem() { return NULL; }
	virtual bool RemoveTopItem() { return false; }
	virtual bool RemoveAllItem() { return false; }

protected:
	virtual void* Add();//添加数据
	virtual bool AddProterty(void* InData, const std::string& InType, int InValueSize);//添加变量

	virtual bool RemoveTop();
	virtual bool RemoveTopProterty();

	virtual bool RemoveAll();
	virtual bool RemoveAllProterty();

	//更新变量 更新地址
	virtual void UpdateProterty();
public:
#if EDITOR_ENGINE
	virtual bool UpdateEditorPropertyDetails(CPropertyObject* InProperty);
#endif // EDI
public:
	FORCEINLINE void SetValueType(const std::string& InType) { ValueType = InType; }
	FORCEINLINE void SetValueSize(int InSize) { ValueSize = InSize; }

	FORCEINLINE int GetCount() const { return Count; }
	FORCEINLINE void SetCountValue(int InCount) { Count = InCount; }

	FORCEINLINE const std::string& GetValueType() const { return ValueType; }
	FORCEINLINE int GetValueSize() const { return ValueSize; }

protected:
	std::string ValueType;
	int ValueSize;

	int Count;
};