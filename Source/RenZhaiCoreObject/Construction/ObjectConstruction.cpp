
#include "ObjectConstruction.h"
#include "../CoreObject/ClassObject.h"
#include "../Archive/Archive.h"

void RZ_UpdateClass(CClassObject* InClass, CCoreMinimalObject* InObject)
{
	//比如制作了新的蓝图
	if (InClass)
	{
		vector<unsigned char> Buffer;
		FArchive Archive(Buffer);
		InClass->GetDefaultObject()->GetNativeClass().Serialize(Archive);

		//将数据序列化到新的结构
		InObject->GetNativeClass().Deserialization(Archive);
	}
}