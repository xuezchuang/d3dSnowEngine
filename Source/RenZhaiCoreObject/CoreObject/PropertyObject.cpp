#include "PropertyObject.h"

CPropertyObject::CPropertyObject()
	:Data(nullptr)
	, ElementSize(0)
{
}

void CPropertyObject::MemoryAllocation()
{
	Data = (char*)malloc(GetElementSize());
}

void CPropertyObject::MemorySetAllocation()
{
	memset(Data, 0, GetElementSize());
}
