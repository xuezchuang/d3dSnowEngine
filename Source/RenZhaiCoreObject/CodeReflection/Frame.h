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

	//这两个值用于找对应函数
	simple_c_guid FuncID;
	CPropertyObject* Key;//注意，这个值并非是真正的值了,只用于判定。

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

	//默认的值 或者是 取值 不是返回值
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
	int CursorIndex;//游标 //主要为Debug 来查看是位于那个字节码 Debug

	CFunctionObject* Function;
	std::shared_ptr<FOutParm> OutParm;//链表

	vector<std::shared_ptr<FOutParmValue>> OutValues;//里面存储的所以执行蓝图的&取值和Returnvalue返回值
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
