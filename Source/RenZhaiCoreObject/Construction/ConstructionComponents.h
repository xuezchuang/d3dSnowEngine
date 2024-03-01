#pragma once
#include "../RenZhaiCoreObjectMacro.h"

class CCoreMinimalObject;
class CComponent;
struct FCreateObjectParam;

namespace ConstructionComponent
{
	void RENZHAICOREOBJECT_API UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent);
	void RENZHAICOREOBJECT_API ConstructionComponents(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject);
}