#pragma once
#include <string>
#include "CoreMacro.h"
#include <map>
#include <vector>
#include <RenZhaiCoreObjectMacro.h>
//#include <cstddef>
//std::byte b = std::byte{0x01}; // 明确使用 std::byte

class CCoreMinimalObject;
class CFunctionObject;
class CPropertyObject;
class CFieldObject;
class FArchive;

enum EPropertyType;


class RENZHAICOREOBJECT_API FNativeClass
{
	friend class CCoreMinimalObject;
public:
	FNativeClass();
	virtual ~FNativeClass();
	bool RemoveTopProperty();

	bool RemoveAllProperty();

	int GetPropertyNumber()const;

	CPropertyObject* AddArrayProperty(
		const std::string& PropertyName,
		const std::string& InType,
		const std::string& InValueType,
		int InValueElementSize,
		int InCount,
		int InElementSize,
		void* InData,
		EPropertyType InPropertyType,
		const std::string& InDefaultValue);

	CPropertyObject* AddMapProperty(
		const std::string& PropertyName,
		const std::string& InType,
		const std::string& InKeyType,
		int InKeyElementSize,
		const std::string& InValueType,
		int InValueElementSize,
		int InCount,
		int InElementSize,
		void* InData,
		EPropertyType InPropertyType,
		const std::string& InDefaultValue);

	template<class T>
	T* AddProperty(const std::string& PropertyName,
		const std::string& InType,
		int InElementSize,
		void* InData,
		EPropertyType InPropertyType,
		const std::string& InDefaultValue);

	CPropertyObject* AddProperty(
		const std::string& PropertyName,
		const std::string& InType,
		int InElementSize,
		void* InData,
		EPropertyType InPropertyType,
		const std::string& InDefaultValue);

	void AddClassType(const std::string& InType);

	CPropertyObject* FindProperty(const std::string& InPropertyString);

	int GetBytecodeFunctionNum();

public:
	void Serialize(FArchive& OutArchive);
	void Deserialization(FArchive& InArchive);

public:
	CFunctionObject* AddStaticFunction(const std::string& InFunctionName);
	CFunctionObject* AddFunction(const std::string& InFunctionName);

	CFunctionObject* FindStaticFunction(const std::string& InFunctionName, const std::string& InClassName);
	CFunctionObject* FindFunction(const std::string& InFunctionName);
	CFunctionObject* FindFunction(const std::string& InFunctionName, const std::string& InClassName);

	static std::string ResolvePackageName(const std::string& InClassName);
protected:
	static void AddList(CFieldObject*& InList, CFieldObject* InAddObject);

protected:
	static CFunctionObject* FindFunction_Interior(CFunctionObject*& InList, const std::string& InClassName, const std::string& InFunctionName);
	static CFunctionObject* AddFunction_Interior(CFieldObject*& InList, CCoreMinimalObject* InOuter, const std::string& InFunctionName);
#if EDITOR_ENGINE
public:
	bool IsExitFields(const std::string& InField);
	int FindFields(const std::string& InField);
	std::string* FindMetas(const std::string& InKey);
public:
	void AddMetas(const std::string& InKeyString, const std::string& InValue);
	void AddMetas(const std::string& InPropertyName, const std::string& InKeyString, const std::string& InValue);

	void AddFields(const std::string& InPropertyName, const std::string& InFields);
	void AddFields(const std::string& InFields);
#endif // 0
public:
	CFunctionObject* FunctionList;
	static CFunctionObject* StaticFunctionList;

	CPropertyObject *Property;//
	CCoreMinimalObject* Outer;

	std::vector<std::string> InheritClassTypes;//继承类的Type

#if EDITOR_ENGINE
	std::map<std::string, std::string> Metas;
	std::vector<std::string> Fields;
#endif // 0
};
