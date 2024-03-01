#pragma once

#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../CodeReflection/ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"
#include "../Construction/ObjectConstruction.h"
#include "../CodeReflection/NativeClass.h"
#include "CoreMacro.h"

class CFunctionObject;
class CCoreMinimalObject;
struct FFrame;
class CClassObject;

//提供核心对象
class RENZHAICOREOBJECT_API CCoreMinimalObject :public IGuidInterface
{
public:
	virtual void InitMinimalObject();//内部调用
public:
	CCoreMinimalObject();
	CCoreMinimalObject(int){}//主要用于反射 为了不自动添加到GObject
	virtual ~CCoreMinimalObject();
	virtual void BeginInit() {};
	virtual void Tick(float DeltaTime) {};
	virtual void BeginExit() {};

	virtual CClassObject* Class();

	bool IsTick()const { return bTick; }
	bool IsPeriodOfDestruction() const;

	virtual void Destroy();

	FORCEINLINE CCoreMinimalObject* GetOuter() const { return Outer; }
	FORCEINLINE std::string GetName() const { return Name; }
	std::string GetDefaultObjectName();//模板的对象的名字

	void SetOuter(CCoreMinimalObject* InNewOuter) {Outer = InNewOuter;}
	void Rename(const std::string& InName) { Name = InName; }
	void SetFlag(unsigned char InNewFlag) { Flag = InNewFlag; }

	//编辑器API
#if EDITOR_ENGINE
	virtual bool UpdateEditorPropertyDetails(CPropertyObject* InProperty) { return false; }
#endif // EDITOR_ENGINE

	//给反射使用 
	virtual bool UpdateEditorContainerPropertyDetails(CPropertyObject* InProperty) { return false; }

	//字节码表函数对应的内容
public:
	FUNCTION_DEFINITION(Script_Undefined);
	FUNCTION_DEFINITION(Script_Int);
	FUNCTION_DEFINITION(Script_Float);
	FUNCTION_DEFINITION(Script_ConstString);
	FUNCTION_DEFINITION(Script_Vector3D);
	FUNCTION_DEFINITION(Script_True);
	FUNCTION_DEFINITION(Script_False);
	FUNCTION_DEFINITION(Script_Funtion);
	FUNCTION_DEFINITION(Script_NestFunction);

	//函数操作
public:
	static void CallFunction(FFrame& Stack, void const* Data, CFunctionObject* Function);
	static void CallNestFunction(FFrame& Stack, void const* Data, CFunctionObject* Function);
	void ExecutionScript(CFunctionObject* Function, void const* Data);

	//主要用于执行事件
	virtual void ExecutionEventScript(const string& EventName, void* Param = nullptr) {};//由反射生成 不要刻意实现该函数
	//该函数可以获取编译后带字节码的函数
	CFunctionObject* FindScriptFuntion(const std::string& FunName);
	
	static CFunctionObject* FindScriptStaticFuntion(const std::string& FunName, const std::string& InClassName);
	static CFunctionObject* FindScriptStaticFuntion(const std::string& FunName);

public:
	FNativeClass &GetNativeClass() { return NativeClass; }

protected:
	virtual void InitReflectionContent() {};

protected:
	bool bTick;

	//就是外层是谁
	CCoreMinimalObject* Outer;
	std::string Name;
	unsigned char Flag;
protected:
	FNativeClass NativeClass;
};

extern RENZHAICOREOBJECT_API vector<CCoreMinimalObject*> GObjects;