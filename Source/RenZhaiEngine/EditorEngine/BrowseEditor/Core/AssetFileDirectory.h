#pragma once
#include "../../../Engine/EngineMinimal.h"

struct FFileDirectory;
struct FFileDirectory :public std::enable_shared_from_this<FFileDirectory>
{
	//文件
	std::vector<std::string> AssetPaths;

	//当前路径 p:/xxx/aa/xxx/content/xxx/xx/xx/xq/
	std::string FileDirectory;//key

	//xq
	std::string PathHead;

	//文件夹
	std::vector<std::shared_ptr<FFileDirectory>> Directorys;
public:
	void Clear();
};