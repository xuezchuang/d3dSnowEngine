#pragma once 
#include "CoreObjectMinimal.h"
#include "../RenZhaiCoreObjectMacro.h"
#include "../CodeReflection/ScriptCommand.h"
#include "CoreObject/FunctionObject.h"

class RENZHAICOREOBJECT_API FArchive
{
public:
	FArchive(vector<unsigned char>& InData)
		:ScriptBuffer(InData)
		,Pos(0)
	{}
	//д��
public:
	virtual FArchive& operator<<(const vector<string>& Value)
	{
		WriteArray(Value);

		return *this;
	}

	virtual FArchive& operator<<(const vector<unsigned char>& Value)
	{
		int Num = int(Value.size());
		*this << Num;

		Serialize((void*)Value.data(), Num * sizeof(unsigned char));

		return *this;
	}

	virtual FArchive& operator<<(unsigned char Value)
	{
		Serialize(&Value, sizeof(unsigned char));

		return *this;
	}

	virtual FArchive& operator<<(const string& Value)
	{
		Serialize((void*)Value.c_str(), strlen(Value.c_str()) + 1);

		return *this;
	}

	virtual FArchive& operator<<(EScriptCommand Value)
	{
		Serialize(&Value,1);

		return *this;
	}

	virtual FArchive& operator<<(EPropertyType Value)
	{
		Serialize(&Value, sizeof(EPropertyType));

		return *this;
	}

	virtual FArchive& operator<<(int Value)
	{
		Serialize(&Value,sizeof(int));

		return *this;
	}

	virtual FArchive& operator<<(float Value)
	{
		Serialize(&Value, sizeof(float));

		return *this;
	}

	virtual FArchive& operator<<(bool Value)
	{
		Serialize(&Value, sizeof(bool));

		return *this;
	}

	virtual FArchive& operator<<(const simple_c_guid& Value)
	{
		Serialize((void*)&Value, sizeof(simple_c_guid));

		return *this;
	}

	virtual FArchive& operator<<(CFunctionObject* Value)
	{
		*this << Value->GetName();
		*this << Value->GetOuter()->GetName();

		return *this;
	}

	//��ȡ
public:
	virtual FArchive& operator>>(vector<string>& Value)
	{
		ReadArray(Value);

		return *this;
	}

	virtual FArchive& operator>>(vector<unsigned char>& Value)
	{
		int Num = 0;
		*this >> Num;

		Value.resize(Num);
		memcpy(Value.data(), &ScriptBuffer[Pos], Num * sizeof(unsigned char));

		Setp(Num);

		return *this;
	}

	virtual FArchive& operator>>(string& Value)
	{
		Value = (char*)&ScriptBuffer[Pos];

		//��¼λ��
		Setp(int(Value.length() + 1));

		return *this;
	}

	virtual FArchive& operator>>(int& Value)
	{
		Read(Value);

		return *this;
	}

	virtual FArchive& operator>>(float& Value)
	{
		Read(Value);

		return *this;
	}

	virtual FArchive& operator>>(bool& Value)
	{
		Read(Value);

		return *this;
	}

	virtual FArchive& operator>>(simple_c_guid& Value)
	{
		Read(Value);

		return *this;
	}

	virtual FArchive& operator>>(EPropertyType& Value)
	{
		Read(Value);

		return *this;
	}

	virtual FArchive& operator>>(EScriptCommand& Value)
	{
		Read(Value);

		return *this;
	}

	void Setp(int InSize);
public:
	template<class T>
	void Read(T& Value)
	{
		Value = *(T*)&ScriptBuffer[Pos];
		Setp(sizeof(T));
	}

	template<class T>
	void ReadArray(vector<T>& Value)
	{
		int Num = 0;
		*this >> Num;

		Value.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			*this >> Value[i];
		}
	}

	template<class T>
	void WriteArray(const vector<T>& Value)
	{
		int Num = int(Value.size());
		*this << Num;

		for (size_t i = 0; i < Num; i++)
		{
			*this << Value[i];
		}
	}
public:
	virtual void Serialize(void* Value, long long Length)
	{
		//��ȡIndex
		int Index = int(ScriptBuffer.size());

		//����Եȵ�����
		for (int i = 0; i < Length; i++)
		{
			ScriptBuffer.push_back(i);
		}

		//�������ݵ��ű�buffer����
		memcpy(&ScriptBuffer[Index],Value,Length);
	}
private:
	vector<unsigned char>&ScriptBuffer;

	//�α�
	int Pos;
};