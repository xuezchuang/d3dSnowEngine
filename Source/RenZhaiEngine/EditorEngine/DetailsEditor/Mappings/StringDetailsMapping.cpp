#include "StringDetailsMapping.h"

bool FStringDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	char Buff[1024] = { 0 };
	string* BuffChar = InProperty->GetData<string>();
	strcpy(Buff, BuffChar->c_str());

	return ImGui::InputText(
		InProperty->GetName().c_str(),
		Buff, 1024, ImGuiInputTextFlags_CallbackEdit,
		FStringDetailsMapping::StringCallback,
		(void*)BuffChar);
}

int FStringDetailsMapping::StringCallback(ImGuiInputTextCallbackData* InData)
{
	InData->Buf;
	string *UserDataString = (string*)InData->UserData;

	*UserDataString = InData->Buf;

	return 0;
}

shared_ptr<FPropertyDetailsMapping> FStringDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FStringDetailsMapping>();
}
