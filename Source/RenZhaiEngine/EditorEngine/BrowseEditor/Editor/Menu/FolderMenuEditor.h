#pragma once
#include "../../../Core/EditorBase.h"

class FFolderMenuEditor :public FEditorBase
{
public:
	FFolderMenuEditor();

	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void SetPath(const std::string& InPath) { Path = InPath; }

protected:
	std::string Path;
	char NewFolderName[128];
};