#pragma once
#include "../../../Engine/EngineMinimal.h"

struct FFileDirectory;
struct FFileDirectory :public std::enable_shared_from_this<FFileDirectory>
{
	//�ļ�
	std::vector<std::string> AssetPaths;

	//��ǰ·�� p:/xxx/aa/xxx/content/xxx/xx/xx/xq/
	std::string FileDirectory;//key

	//xq
	std::string PathHead;

	//�ļ���
	std::vector<std::shared_ptr<FFileDirectory>> Directorys;
public:
	void Clear();
};