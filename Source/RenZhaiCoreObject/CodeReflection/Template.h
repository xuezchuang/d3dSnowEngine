#pragma once
#include "../CoreObjectMinimal.h"

template<class Type>
struct FStaticDataBase
{
	static const char* GetData() { return Data; }
	static int GetNum() { return DataNumber; }

	static int Add(const Type& InType);
	static Type* Get(int Index);

	static int GetTypeSize() { return sizeof(Type); }
protected:
	static char* Data;
	static int DataNumber;
};

template<class Type>
char* FStaticDataBase<Type>::Data = nullptr;

template<class Type>
int FStaticDataBase<Type>::DataNumber = 0;

template<class Type>
int FStaticDataBase<Type>::Add(const Type& InType)
{
	int Size = GetTypeSize();
	if (!Data)
	{
		Data = (char*)malloc(Size);
	}
	else
	{
		Data = (char*)realloc(Data,Size * DataNumber + Size);
	}

	memset(&Data[DataNumber * Size], 0, Size);

	memcpy(&Data[DataNumber * Size], &InType, Size);

	DataNumber++;
	return DataNumber;
}

template<class Type>
Type* FStaticDataBase<Type>::Get(int Index)
{
	if (Index >= 0 && Index < DataNumber)
	{
		return (Type*)&((char*)Data)[Index * GetTypeSize()];
	}

	return nullptr;
}
