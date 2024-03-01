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

	//�ݹ�Ѱ�����ǵ��ļ��� �ҵ��� ����������ļ��Ļ�����ӵ�FFileDirectory�������Դ����
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