#include "AssetAssistLibrary.h"
#include "AssetPackage.h"
#include "AssetPackageManage.h"
#include "CodeReflection/ClassManage.h"
#include "CoreObject/ClassObject.h"

namespace AssetAssistLibrary
{
	CAssetPackage* CreateAssetPackage()
	{
		FCreateObjectParam ClassParam;
		return CreateObject<CAssetPackage>(ClassParam, new CAssetPackage(0));
	}

	CAssetPackage* CreateAssetPackage(
		const std::string& InLocalPath, 
		const std::string& InAssetName,
		int ClassSelectedIndex)
	{
		CAssetPackage *AssetPackage = CreateAssetPackage();
		AnalysisPackageString(AssetPackage, InLocalPath, InAssetName);
		
		auto Tmp = FAssetClassManage::AddClass(FClassManage::Get(ClassSelectedIndex));

		AssetPackage->SetClassObject(Tmp.second);

		return FAssetPackageManage::Get()->AddPackage(InAssetName, AssetPackage);
	}

	CAssetPackage* LoadAssetPackage(const std::string& InLocalPath, const std::string& InAssetName)
	{
		CAssetPackage* AssetPackage = FAssetPackageManage::Get()->FindPackageSafe(InAssetName);
		if (!AssetPackage)
		{
			AssetPackage = CreateAssetPackage();
			AnalysisPackageString(AssetPackage, InLocalPath, InAssetName);

			if (AssetPackage->Load())
			{
				return FAssetPackageManage::Get()->AddPackage(InAssetName, AssetPackage);
			}
		}

		return AssetPackage;
	}

	void AnalysisPackageString(CAssetPackage* InPackage, const std::string& InLocalPath, const std::string& InAssetName)
	{
		if (InPackage)
		{
			char R[512] = { 0 };
			char L[512] = { 0 };
			split(InLocalPath.c_str(), "/Content/", L, R, false);

			InPackage->SetAssetName(InAssetName);
			InPackage->SetAssetRelativePath(R);
		}
	}

	CClassObject* FindClass(const std::string& InAssetClassName)
	{
		if (CAssetPackage* AssetPackage = FAssetPackageManage::Get()->FindPackageSafe(InAssetClassName))
		{
			return AssetPackage->GetClassObject();
		}

		return nullptr;
	}

	CCoreMinimalObject* FindObject(const std::string& InAssetClassName)
	{
		if (CClassObject* InClass = FindClass(InAssetClassName))
		{
			return InClass->GetDefaultObject();
		}

		return nullptr;
	}
}