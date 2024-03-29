#include "AssetPackageManage.h"
#include "AssetAssistLibrary.h"

FAssetPackageManage* FAssetPackageManage::Instance = nullptr;

FAssetPackageManage* FAssetPackageManage::Get()
{
	if (!Instance)
	{
		Instance = new FAssetPackageManage();
	}

	return Instance;
}

void FAssetPackageManage::Tick(float DeltaTime)
{
	bool bExit = false;
	std::map<string, CAssetPackage*> TmpPackags;
	for (auto& Tmp : Instance->Packages)
	{
		if (Tmp.second->IsPeriodOfDestruction())
		{
			bExit = true;
		}
		else
		{
			TmpPackags.insert(Tmp);
		}
	}

	//通过赋值的方式来删除
	if (bExit)
	{
		Instance->Packages = TmpPackags;
	}
}

void FAssetPackageManage::Destory()
{
	if (Instance)
	{
		delete Instance;
	}

	Instance = nullptr;
}

void FAssetPackageManage::Init()
{
	def_c_paths_v2 Paths;
	init_def_c_paths_v2(&Paths);

	std::string ContentPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineContentPath());

	find_files_v2(ContentPath.c_str(), &Paths, true, true);

	int offset = 0;
	for (int i = 0; i < Paths.num; i++)
	{
		char* TmpPath = get_def_c_paths_by_offset(&Paths, offset);
		offset += get_def_c_offset(TmpPath);

		if (find_string(TmpPath, ".rz", 0) != -1)
		{
			//c://xx//ww//dd//ww.rz
			char HeadFile[128] = { 0 };
			get_path_clean_filename(HeadFile, TmpPath);

			replace_string_inline(HeadFile, ".rz", "\0");

			char FliePath[512] = { 0 };
			get_path_directory(FliePath, TmpPath);

			AssetAssistLibrary::LoadAssetPackage(FliePath, HeadFile);
		}
	}

	destroy_def_c_paths_v2(&Paths);
}

CAssetPackage* FAssetPackageManage::AddPackage(const string& InName, CAssetPackage* InPackage)
{
	if (InPackage && InName.length() > 0)
	{
		if (!IsExitPackage(InName))
		{
			Packages.insert({ InName,InPackage });

			return InPackage;
		}
	}

	return nullptr;
}

void FAssetPackageManage::DeletePackage(const string& InName)
{
	Instance->Packages.erase(InName);
}

CAssetPackage* FAssetPackageManage::FindPackage(const string& InPackageName)
{
	return Instance->Packages[InPackageName];
}

CAssetPackage* FAssetPackageManage::FindPackageSafe(const string& InPackageName)
{
	auto Iter = Instance->Packages.find(InPackageName);
	if (Iter != Instance->Packages.end())
	{
		return Iter->second;
	}

	return nullptr;
}

bool FAssetPackageManage::IsExitPackage(const string& InPackageName)
{
	return FindPackageSafe(InPackageName) != nullptr;
}
