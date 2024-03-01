#pragma once
#include "../../Core/EditorBase.h"
#include "Element/BrowseAssetIcon.h"
#include "../Core/BrowseEditorType.h"

class FAssetEditor :public FEditorBase
{
public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	bool IsInTheScope(const fvector_2d& InCurrentPosition) const;

public:
	void OnMouseDragging();

protected:
	std::vector<FBrowseAssetIcon> Icons;
	FAssetGeometry Geometry;
};