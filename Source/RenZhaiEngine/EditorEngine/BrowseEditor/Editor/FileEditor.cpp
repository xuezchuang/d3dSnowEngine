#include "FileEditor.h"
#include "../Core/AssetEditorCommon.h"
#include "Menu/FolderMenuEditor.h"

FFileEditor::FFileEditor()
{
	UpdateFileTime = 1.f;
	CurrentFileTime = 0.f;

	FolderMenuEditor = std::make_shared<FFolderMenuEditor>();

	FileContentPath = make_shared<FFileDirectory>();
	FileContentPath->FileDirectory = FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineContentPath()) + "/";

	FileContentPath->PathHead = "Content";
}

void FFileEditor::BuildEditor()
{
	ResetPaths();

	FolderMenuEditor->BuildEditor();
}

void FFileEditor::DrawEditor(float DeltaTime)
{
	CurrentFileTime += DeltaTime;
	if (CurrentFileTime >= UpdateFileTime)
	{
		CurrentFileTime = 0.f;

		ResetPaths();
	}
	
	std::string SelectPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineContentPath()) + '/';

	RecursionSpawnFileWidget(FileContentPath, SelectPath);

	if (!SelectPath.empty())
	{
		RecursionSelectedPath(FileContentPath,SelectPath);
	}
}

void FFileEditor::ExitEditor()
{
	FolderMenuEditor->ExitEditor();
}

void FFileEditor::ResetPaths()
{
	FileContentPath->Clear();

	def_c_paths Paths;
	init_def_c_paths(&Paths);

	std::string ContentPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineContentPath());

	find_files(ContentPath.c_str(), &Paths, true,true);

	for (int i = 0; i < Paths.index; i++)
	{
#ifdef _WIN64
#else
		normalization_path(Paths.paths[i]);
#endif

		EResetFileType FileType = RESETFILETYPE_NONE;
		if (find_string(Paths.paths[i], ".rz", 0) != -1)
		{
			FileType = EResetFileType::RESETFILETYPE_ALLOW_FILE;
		}
		
		int EndPos = strlen(Paths.paths[i]) - sizeof(char);
		if (EndPos > 0)
		{
			if (Paths.paths[i][EndPos] == '/')
			{
				FileType = EResetFileType::RESETFILETYPE_PATH;
			}
		}
		else
		{
			continue;
		}

		if (FileType == EResetFileType::RESETFILETYPE_PATH ||
			FileType == RESETFILETYPE_ALLOW_FILE)
		{
			char Buff[1024] = { 0 };
			if (FileType == EResetFileType::RESETFILETYPE_ALLOW_FILE)
			{
				get_path_directory(Buff, Paths.paths[i]);
			}
			else
			{
				strcpy(Buff, Paths.paths[i]);
			}			

			if (!RecursionFindFileWidgetAndAdd(FileContentPath, Buff, Paths.paths[i], FileType))
			{
				//Î´Íê...
				char BuffPath[1024] = { 0 };
				get_path_directory(BuffPath, Buff);

				remove_char_end(BuffPath, '/');

				shared_ptr<FFileDirectory> InMyDirectory = RecursionFindFileWidget(FileContentPath, BuffPath);

				if (InMyDirectory)
				{
					InMyDirectory->Directorys.push_back(make_shared<FFileDirectory>());
					std::shared_ptr<FFileDirectory>& InDirectory = InMyDirectory->Directorys[InMyDirectory->Directorys.size() - 1];

					InDirectory->FileDirectory = Buff;
					if (FileType == EResetFileType::RESETFILETYPE_ALLOW_FILE)
					{
						InDirectory->AssetPaths.push_back(Paths.paths[i]);
					}

					char CleanFilename[64] = { 0 };
					get_path_clean_filename(CleanFilename, Buff);

					InDirectory->PathHead = CleanFilename;
				}
			}
		}
	}
}

void FFileEditor::RecursionSelectedPath(std::shared_ptr<FFileDirectory>& InFileDirectory, const std::string& InSelectPath)
{
	if (InFileDirectory)
	{
		if (InSelectPath == InFileDirectory->FileDirectory)
		{
			CurrentSelectAssetPath = InFileDirectory;
			return;
		}
	}

	for (int i = 0; i < InFileDirectory->Directorys.size(); i++)
	{
		RecursionSelectedPath(InFileDirectory->Directorys[i], InSelectPath);
	}
}

void FFileEditor::RecursionSpawnFileWidget(std::shared_ptr<FFileDirectory>& InFileDirectory, std::string& OutPath)
{
	if (InFileDirectory)
	{
		if (!InFileDirectory->PathHead.empty())
		{
			if (ImGui::TreeNode(InFileDirectory->PathHead.c_str()))
			{
				OutPath = InFileDirectory->FileDirectory;

				FolderMenuEditor->SetPath(OutPath);
				FolderMenuEditor->DrawEditor(0.03f);

				for (auto& Tmp : InFileDirectory->Directorys)
				{
					RecursionSpawnFileWidget(Tmp, OutPath);
				}

				ImGui::TreePop();
			}
		}
	}
}

bool FFileEditor::RecursionFindFileWidgetAndAdd(
	const std::shared_ptr<FFileDirectory>& InFileDirectory, 
	const char* InCompareStrings,
	const char* InAssetPath,
	EResetFileType InType)
{
	if (InFileDirectory)
	{
		if (InFileDirectory->FileDirectory == InCompareStrings)
		{
			if (InType == EResetFileType::RESETFILETYPE_ALLOW_FILE)
			{
				InFileDirectory->AssetPaths.push_back(InAssetPath);
			}

			return true;
		}

		for (auto& Tmp : InFileDirectory->Directorys)
		{
			if (RecursionFindFileWidgetAndAdd(Tmp, InCompareStrings, InAssetPath, InType))
			{
				return true;
			}
		}
	}

	return false;
}

shared_ptr<FFileDirectory> FFileEditor::RecursionFindFileWidget(const std::shared_ptr<FFileDirectory>& InFileDirectory, const char* InCompareDirectory)
{
	if (InFileDirectory)
	{
		if (InFileDirectory->FileDirectory == InCompareDirectory)
		{
			return InFileDirectory;
		}

		for (auto& Tmp : InFileDirectory->Directorys)
		{
			if (auto SubTmp = RecursionFindFileWidget(Tmp, InCompareDirectory))
			{
				return SubTmp;
			}
		}
	}

	return NULL;
}
