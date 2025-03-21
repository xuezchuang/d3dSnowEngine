#pragma once
#include "../Core/EditorBase.h"
#include "Math/Common.h"
#include "EngineProfiling.h"

class FProfilingEditor :public FEditorBase
{
public:
	FProfilingEditor();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

protected:
	class FAssetEditor* AssetEditor;
	class FFileEditor* FileEditor;
	class FAssetMenuEditor* AssetMenuEditor;
private:
	std::vector<T_PROFILING> aProfilingTime;
};