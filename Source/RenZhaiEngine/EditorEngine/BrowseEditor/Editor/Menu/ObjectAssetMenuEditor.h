#pragma once
#include "../../../Core/EditorBase.h"

class FObjectAssetMenuEditor :public FEditorBase
{
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void SetAssetFullPath(const char* InPath) { AssetFullPath = InPath; }

protected:
	const char* AssetFullPath;
};