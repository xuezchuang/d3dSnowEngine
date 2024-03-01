#pragma once
#include <map>
#include <string>
#include "CoreMacro.h"
#include <map>
#include <vector>

class CCoreMinimalObject;
class CFunctionObject;
class CPropertyObject;
class CFieldObject;
class FArchive;

enum EPropertyType;

using namespace std;

struct FNativeClass
{
	friend class CCoreMinimalObject;
public:
	FNativeClass();

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
		const string& InDefaultValue);

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
		const string& InDefaultValue);

	template<class T>
	T* AddProperty(const std::string& PropertyName,
		const std::string& InType,
		int InElementSize,
		void* InData,
		EPropertyType InPropertyType,
		const string& InDefaultValue);

	CPropertyObject* AddProperty(
		const std::string& PropertyName,
		const std::string& InType,
		int InElementSize,
		void* InData,
		EPropertyType InPropertyType,
		const string& InDefaultValue);

	void AddClassType(const std::string& InType);

	CPropertyObject* FindProperty(const std::string& InPropertyString);

	int GetBytecodeFunctionNum();

public:
	void Serialize(FArchive& OutArchive);
	void Deserialization(FArchive& InArchive);

public:
	CFunctionObject* AddStaticFunction(const string& InFunctionName);
	CFunctionObject* AddFunction(const string& InFunctionName);

	CFunctionObject* FindStaticFunction(const string& InFunctionName, const string& InClassName);
	CFunctionObject* FindFunction(const string& InFunctionName);
	CFunctionObject* FindFunction(const string& InFunctionName, const string& InClassName);

	static string ResolvePackageName(const string& InClassName);
protected:
	static void AddList(CFieldObject*& InList, CFieldObject* InAddObject);

protected:
	static CFunctionObject* FindFunction_Interior(CFunctionObject*& InList, const string& InClassName, const string& InFunctionName);
	static CFunctionObject* AddFunction_Interior(CFieldObject*& InList, CCoreMinimalObject* InOuter, const string& InFunctionName);
#if EDITOR_ENGINE
public:
	bool IsExitFields(const std::string& InField);
	int FindFields(const std::string &InField);
	std::string *FindMetas(const std::string& InKey);
public:
	void AddMetas(const std::string& InKeyString, const std::string& InValue);
	void AddMetas(const string& InPropertyName, const string& InKeyString, const string& InValue);

	void AddFields(const string& InPropertyName, const string& InFields);
	void AddFields(const string& InFields);
#endif // 0
public:
	CFunctionObject* FunctionList;
	static CFunctionObject* StaticFunctionList;

	CPropertyObject *Property;//
	CCoreMinimalObject* Outer;

	std::vector<std::string> InheritClassTypes;//¼Ì³ÐÀàµÄType

#if EDITOR_ENGINE
	std::map<std::string, std::string> Metas;
	std::vector<std::string> Fields;
#endif // 0
};
