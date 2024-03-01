#include "ArrayPropertyObject.h"

void* CArrayPropertyObject::AddItem()
{
	void* CurrentData = Add();

	AddProterty(CurrentData,ValueType,ValueSize);

	UpdateProterty();

	return nullptr;
}

bool CArrayPropertyObject::RemoveTopItem()
{
	if (Count > 0)
	{
		//охрфЁЩ╠Да©
		RemoveTopProterty();
		RemoveTop();

		UpdateProterty();

		return true;
	}

	return false;
}

bool CArrayPropertyObject::RemoveAllItem()
{
	if (Count != 0)
	{
		RemoveAllProterty();
		RemoveAll();

		return true;
	}

	return false;
}

CArrayPropertyObject::CArrayPropertyObject()
{

}
