
#include "ObjectConstruction.h"
#include "../CoreObject/ClassObject.h"
#include "../Archive/Archive.h"

void RZ_UpdateClass(CClassObject* InClass, CCoreMinimalObject* InObject)
{
	//�����������µ���ͼ
	if (InClass)
	{
		vector<unsigned char> Buffer;
		FArchive Archive(Buffer);
		InClass->GetDefaultObject()->GetNativeClass().Serialize(Archive);

		//���������л����µĽṹ
		InObject->GetNativeClass().Deserialization(Archive);
	}
}