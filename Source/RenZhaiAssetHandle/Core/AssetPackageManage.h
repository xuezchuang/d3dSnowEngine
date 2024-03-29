#pragma once
#include "../RenZhaiAssetHandle.h"
#include "../RenZhaiAssetHandleMacro.h"
#include "AssetPackage.h"

struct RENZHAIASSETHANDLE_API FAssetPackageManage
{
public:
	static FAssetPackageManage* Get();

	static void Tick(float DeltaTime);

	static void Destory();

public:
	void Init();

public:
	CAssetPackage* AddPackage(const string& InName, CAssetPackage* InPackage);
	void DeletePackage(const string& InName);
	CAssetPackage* FindPackage(const string& InPackageName);
	CAssetPackage* FindPackageSafe(const string& InPackageName);

	bool IsExitPackage(const string& InPackageName);
private:
	std::map<string, CAssetPackage*> Packages;

	static FAssetPackageManage* Instance;
};