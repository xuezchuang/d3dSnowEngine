#pragma once

#include "../RenZhaiCoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"

class CFunctionObject;
class CPropertyObject;

class FOutParm :public std::enable_shared_from_this<FOutParm>
{
public:
	FOutParm();

	CPropertyObject* PropAddr;
	std::shared_ptr<FOutParm> Nest;
};

struct FOutParmValue :public std::enable_shared_from_this<FOutParmValue>
{
	FOutParmValue(int InDataSize, void* InData);

	FOutParmValue()
		:Data(nullptr)
		, DataSize(0)
		, Key(NULL)
	{
		normalization_guid(&FuncID);
	}

	~FOutParmValue();

	unsigned char* Data;
	int DataSize;

	//������ֵ�����Ҷ�Ӧ����
	simple_c_guid FuncID;
	CPropertyObject* Key;//ע�⣬���ֵ������������ֵ��,ֻ�����ж���

	void InitSize(int InDataSize, void* InData);

	void MemoryAllocation(int InSize);
	void MemorySetAllocation();
};

struct FFrame
{
	FFrame();
	FFrame(CFunctionObject* InNewFunction);

	template<typename Type>
	Type Read();

	float ReadFloat();
	int ReadInt();

	//Ĭ�ϵ�ֵ ������ ȡֵ ���Ƿ���ֵ
	void AddParm(CPropertyObject* Addr);

	void Step(CCoreMinimalObject* Context, void const* RefData);

	string ReadString();
	simple_c_guid GetGuid();
	EPropertyType GetPropertyType();

	CFunctionObject *ReadFunction();

	RENZHAICOREOBJECT_API unsigned char* GetParmAddr();
	FORCEINLINE int GetCursorPos() const { return CursorIndex; }
	FORCEINLINE unsigned char* GetCode()const { return Code; }
	FORCEINLINE CCoreMinimalObject* GetOuter() { return Outer; }

public:
	unsigned char operator++(int)
	{
		unsigned char Tmp = *Code;
		Iteration();

		return Tmp;
	}

	FFrame &operator+=(int InSize)
	{
		Iteration(InSize);

		return *this;
	}

protected:
	void Iteration(int InSize = 1);

public:
	int CursorIndex;//�α� //��ҪΪDebug ���鿴��λ���Ǹ��ֽ��� Debug

	CFunctionObject* Function;
	std::shared_ptr<FOutParm> OutParm;//����

	vector<std::shared_ptr<FOutParmValue>> OutValues;//����洢������ִ����ͼ��&ȡֵ��Returnvalue����ֵ
protected:
	unsigned char* Code;
	CCoreMinimalObject* Outer;
};

typedef void(*FNativeFuntionPtr)(CCoreMinimalObject* Context, FFrame& TheStack, void const* RefData);

template<typename Type>
inline Type FFrame::Read()
{
	Type Result = 0;
	int TypeLen = sizeof(Type);

	memcpy(&Result, Code, TypeLen);

	*this += TypeLen;

	return Result;
}
