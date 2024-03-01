#include "AssetPackage.h"
#include "CodeReflection/ClassManage.h"
#include "CoreObject/ClassObject.h"
#include "Archive/Archive.h"

CAssetPackage::CAssetPackage()
{
}

CAssetPackage::CAssetPackage(int)
{
}

bool CAssetPackage::Save()
{
	string SavePath = simple_cpp_string_algorithm::printf("%s/%s%s.rz",
		FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineContentPath()).c_str(),
		AssetRelativePath.c_str(), 
		AssetName.c_str());

	vector<unsigned char> Buffer;
	FArchive Archive(Buffer);

	Archive << AssetName;
	Archive << AssetRelativePath;
	Archive << ClassObject->GetDefaultObject()->GetName();
	Archive << ClassObject->GetName();
	Archive << BlueprintDatas;

	//���л��ֽ���
	ClassObject->GetDefaultObject()->GetNativeClass().Serialize(Archive);

	return simple_cpp_helper_file::save_file_to_bytes(SavePath.c_str(), Buffer);
}

bool CAssetPackage::Load()
{
	string LoadPath = simple_cpp_string_algorithm::printf("%s/%s%s.rz",
		FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineContentPath()).c_str(),
		AssetRelativePath.c_str(), AssetName.c_str());

	vector<unsigned char> Buffer;
	bool bLoad = simple_cpp_helper_file::load_file_to_bytes(LoadPath.c_str(), Buffer);

	string DefaultObjectName;
	string ClassObjectName;

	FArchive Archive(Buffer);
	Archive >> AssetName;
	Archive >> AssetRelativePath;
	Archive >> DefaultObjectName;
	Archive >> ClassObjectName;
	Archive >> BlueprintDatas;

	//������������Class��
	if (!ClassObjectName.empty())
	{
		//����Class
		auto Tmp = FAssetClassManage::SetClass(DefaultObjectName, ClassObjectName);
		ClassObject = Tmp.second;
	}

	//�����л��ֽ���
	ClassObject->GetDefaultObject()->GetNativeClass().Deserialization(Archive);

	return bLoad;
}
