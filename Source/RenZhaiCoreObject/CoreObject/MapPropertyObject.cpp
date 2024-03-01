#include "MapPropertyObject.h"

void* CMapPropertyObject::AddItem()
{
	if (!IsExitKey(""))
	{
		void* KeyData = Add();
		AddProterty(KeyData,KeyType,KeySize);

		void* ValueData = Add();
		AddProterty(ValueData,ValueType,ValueSize);

		UpdateProterty();

		return KeyData;
	}

	return nullptr;
}

bool CMapPropertyObject::RemoveTopItem()
{
	if (Count > 0)
	{
		//先移除Value变量
		RemoveTopProterty();
		RemoveTop();

		//先移除Key变量
		RemoveTopProterty();
		RemoveTop();

		UpdateProterty();

		return true;
	}

	return false;
}

bool CMapPropertyObject::RemoveAllItem()
{
	if (Count != 0)
	{
		RemoveAllProterty();
		RemoveAll();

		return true;
	}

	return false;
}

bool CMapPropertyObject::IsExitKey(const char* InKey)
{
	CPropertyObject* PropertyPtr = GetNativeClass().Property;

	while (PropertyPtr)
	{
		char* InKeyData = (char*)PropertyPtr->GetData();
		if (string(InKeyData) == InKey)
		{
			return true;
		}

		PropertyPtr = dynamic_cast<CPropertyObject*>(PropertyPtr->Nest->Nest);
	}

	return false;
}

CMapPropertyObject::CMapPropertyObject()
{

}
