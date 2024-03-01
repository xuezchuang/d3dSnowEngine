#pragma once
#include "AssetPackage.h"
#include "../RenZhaiAssetHandleMacro.h"

class CClassObject;
namespace AssetAssistLibrary
{
	RENZHAIASSETHANDLE_API CAssetPackage* CreateAssetPackage();
	RENZHAIASSETHANDLE_API CAssetPackage* CreateAssetPackage(const std::string& InLocalPath, const std::string& InAssetName, int ClassSelectedIndex);
	RENZHAIASSETHANDLE_API CAssetPackage* LoadAssetPackage(const std::string& InLocalPath, const std::string& InAssetName);

	RENZHAIASSETHANDLE_API void AnalysisPackageString(CAssetPackage* InPackage, const std::string& InLocalPath, const std::string& InAssetName);

	RENZHAIASSETHANDLE_API CClassObject* FindClass(const std::string& InAssetClassName);
	RENZHAIASSETHANDLE_API CCoreMinimalObject* FindObject(const std::string& InAssetClassName);
}