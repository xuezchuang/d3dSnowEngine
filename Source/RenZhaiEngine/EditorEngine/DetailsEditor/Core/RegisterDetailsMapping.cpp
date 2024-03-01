#include "RegisterDetailsMapping.h"

map<std::string, shared_ptr<FClassDetailsMapping>> FRegisterDetailsMapping::ClassDetailsMappings;
map<std::string, shared_ptr<FPropertyDetailsMapping>> FRegisterDetailsMapping::PropertyDetailsMappings;

void FRegisterDetailsMapping::RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping)
{
	if (InMapping && !InClassName.empty())
	{
		ClassDetailsMappings.insert(std::pair<string, shared_ptr<FClassDetailsMapping>>(InClassName, InMapping));
	}
}

void FRegisterDetailsMapping::RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping)
{
	if (InMapping && !InPropertyName.empty())
	{
		PropertyDetailsMappings.insert(std::pair<string, shared_ptr<FPropertyDetailsMapping>>(InPropertyName, InMapping));
	}
}

shared_ptr<FClassDetailsMapping> FRegisterDetailsMapping::FindClass(const std::string& InClassName)
{
	auto iterator = ClassDetailsMappings.find(InClassName);
	if (iterator != ClassDetailsMappings.end())
	{
		return iterator->second;
	}

	return nullptr;
}

shared_ptr<FPropertyDetailsMapping> FRegisterDetailsMapping::FindProperty(const std::string& InPropertyName)
{
	auto iterator = PropertyDetailsMappings.find(InPropertyName);
	if (iterator != PropertyDetailsMappings.end())
	{
		return iterator->second;
	}

	return nullptr;
}

bool FRegisterDetailsMapping::UpdateClassWidget(CCoreMinimalObject* InObject)
{
	if (InObject)
	{
		for (auto& Tmp : InObject->GetNativeClass().InheritClassTypes)
		{
			//需要考虑继承关系 映射继承的内容
			if (shared_ptr<FClassDetailsMapping> ClassDetailsMapping = FindClass(Tmp))
			{
				ClassDetailsMapping->UpdateDetailsWidget(InObject);
			}
		}
	}

	return false;
}

bool FRegisterDetailsMapping::UpdatePropertyWidget(CPropertyObject* InProperty)
{
	if (InProperty)
	{
		std::map<std::string, std::vector<CPropertyObject*>> Categorys;
		auto AddCategory = [&](CPropertyObject* InNewProperty, const std::string& InCategoryName)
		{
			auto CategorysIt = Categorys.find(InCategoryName);
			if (CategorysIt != Categorys.end())
			{
				CategorysIt->second.push_back(InNewProperty);
			}
			else
			{
				Categorys.insert({InCategoryName,std::vector<CPropertyObject*>()});

				Categorys[InCategoryName].push_back(InNewProperty);
			}
		};

		//收集我们的类别
		while (InProperty)
		{
			auto MateValue = InProperty->GetNativeClass().Metas.find("Category");
			if (MateValue != InProperty->GetNativeClass().Metas.end())
			{
				AddCategory(InProperty, MateValue->second);
			}

			InProperty = dynamic_cast<CPropertyObject*>(InProperty->Nest);
		}

		ImGui::Separator();
		for (auto& Tmp : Categorys)
		{		
			if (ImGui::CollapsingHeader(Tmp.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (auto& SubTmp : Tmp.second)
				{
					if (auto InDetailsProperty = FindProperty(SubTmp->GetType().c_str()))
					{
						if (InDetailsProperty->UpdateDetailsWidget(SubTmp))
						{
							if (CCoreMinimalObject* InObject = dynamic_cast<CCoreMinimalObject*>(SubTmp->GetOuter()))
							{
								InObject->UpdateEditorPropertyDetails(SubTmp);
							}
						}
					}
				}
			}		
		}
		ImGui::Separator();

		return true;
	}
	
	return false;
}

bool FRegisterDetailsMapping::UpdatePropertyWidgetNoCategory(CPropertyObject* InProperty)
{
	//收集我们的类别
	while (InProperty)
	{
		InProperty = UpdatePropertyWidgetElement(InProperty);
	}

	return true;
}

CPropertyObject* FRegisterDetailsMapping::UpdatePropertyWidgetElement(CPropertyObject* InProperty)
{
	if (auto InDetailsProperty = FindProperty(InProperty->GetType().c_str()))
	{
		if (InDetailsProperty->UpdateDetailsWidget(InProperty))
		{
			if (CCoreMinimalObject* InObject = dynamic_cast<CCoreMinimalObject*>(InProperty->GetOuter()))
			{
				InObject->UpdateEditorPropertyDetails(InProperty);
			}
		}
	}

	return dynamic_cast<CPropertyObject*>(InProperty->Nest);
}

bool FRegisterDetailsMapping::UpdatePropertyWidgetKeyValue(CPropertyObject* InProperty)
{
	//收集我们的类别
	while (InProperty)
	{
		InProperty = UpdatePropertyWidgetElement(InProperty);//Key

		ImGui::SameLine();

		InProperty = UpdatePropertyWidgetElement(InProperty);//Value
	}

	return true;
}
