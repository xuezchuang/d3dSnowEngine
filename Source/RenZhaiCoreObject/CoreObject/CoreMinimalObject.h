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

//�ṩ���Ķ���
class  RENZHAICOREOBJECT_API CCoreMinimalObject :public IGuidInterface
{
public:
	virtual void InitMinimalObject();//�ڲ�����
public:
	CCoreMinimalObject();
	CCoreMinimalObject(int){}//��Ҫ���ڷ��� Ϊ�˲��Զ���ӵ�GObject
	virtual ~CCoreMinimalObject();
	virtual void BeginInit() {};
	virtual void Tick(float DeltaTime) {};
	virtual void BeginExit() {};

	virtual CClassObject* Class();

	bool IsTick()const { return bTick; }
	bool IsPeriodOfDestruction() const;

	virtual void Destroy();

	FORCEINLINE CCoreMinimalObject* GetOuter() const { return Outer; }
	std::string GetName() const;
	std::string GetDefaultObjectName();//ģ��Ķ��������

	void SetOuter(CCoreMinimalObject* InNewOuter) {Outer = InNewOuter;}
	void Rename(const std::string& InName);
	void SetFlag(unsigned char InNewFlag) { Flag = InNewFlag; }

	//�༭��API
#if EDITOR_ENGINE
	virtual bool UpdateEditorPropertyDetails(CPropertyObject* InProperty) { return false; }
#endif // EDITOR_ENGINE

	//������ʹ�� 
	virtual bool UpdateEditorContainerPropertyDetails(CPropertyObject* InProperty) { return false; }

	//�ֽ��������Ӧ������
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

	//��������
public:
	static void CallFunction(FFrame& Stack, void const* Data, CFunctionObject* Function);
	static void CallNestFunction(FFrame& Stack, void const* Data, CFunctionObject* Function);
	void ExecutionScript(CFunctionObject* Function, void const* Data);

	//��Ҫ����ִ���¼�
	virtual void ExecutionEventScript(const string& EventName, void* Param = nullptr) {};//�ɷ������� ��Ҫ����ʵ�ָú���
	//�ú������Ի�ȡ�������ֽ���ĺ���
	CFunctionObject* FindScriptFuntion(const std::string& FunName);
	
	static CFunctionObject* FindScriptStaticFuntion(const std::string& FunName, const std::string& InClassName);
	static CFunctionObject* FindScriptStaticFuntion(const std::string& FunName);

public:
	FNativeClass &GetNativeClass() { return NativeClass; }

protected:
	virtual void InitReflectionContent() {};

protected:
	bool bTick;
	FNativeClass NativeClass;
	//���������˭
	CCoreMinimalObject* Outer;
	
	unsigned char Flag;
	std::string Name;

	//class Impl;
	//std::unique_ptr<Impl> pImpl;
};

extern RENZHAICOREOBJECT_API vector<CCoreMinimalObject*> GObjects;