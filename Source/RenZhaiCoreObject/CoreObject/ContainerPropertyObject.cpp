#include "ContainerPropertyObject.h"

CContainerPropertyObject::CContainerPropertyObject()
{
	Count = 0;
}

void* CContainerPropertyObject::Add()
{
	int CurrentIndex = ValueSize * Count;

	if (!Data)
	{
		Data = (char*)malloc(ValueSize);
	}
	else
	{
		Data = (char*)realloc(Data, ValueSize * Count + ValueSize);
	}

	memset(&Data[CurrentIndex], 0, ValueSize);

	Count++;

	return &Data[CurrentIndex];
}

bool CContainerPropertyObject::AddProterty(void* InData, const std::string& InType, int InValueSize)
{
	int PropertyNumber = GetNativeClass().GetPropertyNumber();;

	char ObjectName[128] = { 0 };
	sprintf(ObjectName, "%d", PropertyNumber);

	if (auto Property = GetNativeClass().AddProperty(
		ObjectName,
		InType,
		InValueSize,
		InData,EPropertyType::INPUT_PROPERTY,""))
	{
		return true;
	}

	return false;
}

bool CContainerPropertyObject::RemoveTop()
{
	if (Count > 0)
	{
		Count--;
		if (Count == 0)
		{
			free(Data);
			Data = nullptr;
		}
		else
		{
			Data = (char*)realloc(Data, ValueSize * Count + ValueSize);
		}

		return true;
	}

	return false;
}

bool CContainerPropertyObject::RemoveTopProterty()
{
	return GetNativeClass().RemoveTopProperty();
}

bool CContainerPropertyObject::RemoveAll()
{
	if (Count != 0)
	{
		free(Data);
		Data = nullptr;
		Count = 0;

		return true;
	}

	return false;
}

bool CContainerPropertyObject::RemoveAllProterty()
{
	return GetNativeClass().RemoveAllProperty();;
}

void CContainerPropertyObject::UpdateProterty()
{
	int i = 0;

	CPropertyObject* InProperty = GetNativeClass().Property;
	while (InProperty)
	{
		void* Ptr = &Data[i * ValueSize];
		InProperty->InitializeValue((char*)Ptr);

		InProperty = dynamic_cast<CPropertyObject*>(InProperty->Nest);

		i++;
	}
}

#if EDITOR_ENGINE
bool CContainerPropertyObject::UpdateEditorPropertyDetails(CPropertyObject* InProperty)
{
	if (CCoreMinimalObject* InObject = dynamic_cast<CCoreMinimalObject*>(GetOuter()))
	{
		return InObject->UpdateEditorContainerPropertyDetails(this);;
	}

	return false;
}
#endif