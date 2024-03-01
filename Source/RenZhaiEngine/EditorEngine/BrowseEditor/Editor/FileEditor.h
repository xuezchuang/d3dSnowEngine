#pragma once
#include "../../Core/EditorBase.h"
#include "../Core/BrowseEditorType.h"

struct FFileDirectory;
struct FFileNode;

class FFileEditor :public FEditorBase
{
public:
	FFileEditor();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void ResetPaths();
	void RecursionSelectedPath(std::shared_ptr<FFileDirectory>& InFileDirectory, const std::string& InSelectPath);
	
	void RecursionSpawnFileWidget(
		std::shared_ptr<FFileDirectory>& InFileDirectory,
		std::string& OutPath);

	//递归寻找我们的文件夹 找到了 如果我们是文件的话就添加到FFileDirectory里面的资源里面
	bool RecursionFindFileWidgetAndAdd(
		const std::shared_ptr<FFileDirectory>& InFileDirectory,
		const char* InCompareStrings,
		const char* InAssetPath,
		EResetFileType InType);

	shared_ptr<FFileDirectory> RecursionFindFileWidget(
		const std::shared_ptr<FFileDirectory>& InFileDirectory,
		const char* InCompareDirectory);

private:
	std::shared_ptr<FFileDirectory> FileContentPath;

	float UpdateFileTime;
	float CurrentFileTime;

	std::shared_ptr<class FFolderMenuEditor> FolderMenuEditor;
};