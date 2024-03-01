#pragma once
#include "../Core/EditorBase.h"

class FBrowseEditor :public FEditorBase
{
public:
	FBrowseEditor();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

protected:
	class FAssetEditor* AssetEditor;
	class FFileEditor* FileEditor;
	class FAssetMenuEditor* AssetMenuEditor;
};