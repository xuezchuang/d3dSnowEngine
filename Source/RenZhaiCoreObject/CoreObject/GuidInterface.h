#pragma once
#include "../CoreObjectMinimal.h"
#include "../RenZhaiCoreObjectMacro.h"

//提供一个对象的身份ID
class RENZHAICOREOBJECT_API IGuidInterface
{
public:
	IGuidInterface();

	bool operator==(const IGuidInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

	bool operator()(const simple_c_guid& InFirst, const simple_c_guid& InSecond) const
	{
		return InFirst.a < InSecond.a;
	}

	string GetGuidString() const;

	simple_c_guid GetGuid() { return Guid; }

	void ResetGuid(const simple_c_guid& InNewGuid) { Guid = InNewGuid; }
private:
	simple_c_guid Guid;
};