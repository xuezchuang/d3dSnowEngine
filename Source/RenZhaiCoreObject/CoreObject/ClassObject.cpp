#include "ClassObject.h"

CClassObject::CClassObject(int)
	:Super(0)
{
	DefaultObject = nullptr;
}

CCoreMinimalObject* CClassObject::GetDefaultObject()
{
	if (!DefaultObject)
	{
		if (RegisterClassObjectDelegate.IsBound())
		{
			DefaultObject = RegisterClassObjectDelegate.Execute();
			if (DefaultObject)
			{
				DefaultObject->SetOuter(this);
				//DefaultObject->InitMinimalObject();//收集反射信息
			}
		}
	}

	return DefaultObject;
}

