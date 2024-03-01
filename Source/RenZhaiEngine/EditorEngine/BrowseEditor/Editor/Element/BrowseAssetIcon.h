#pragma once
#include "../../../../Engine/EngineMinimal.h"

class FObjectAssetMenuEditor;
class FBrowseAssetIcon
{
public:
	FBrowseAssetIcon();

public:
	virtual void Draw(int UID, const char* AssetName, const char* AssetFileName, float DeltaTime);

public:
	void SetIconPosition(const fvector_2d& InNewPosition) { IconPosition = InNewPosition; }
	void SetIconSize(const fvector_2d& InNewSize) { IconSize = InNewSize; }

	fvector_2d GetIconPosition()const { return IconPosition; }
	fvector_2d GetSize()const { return IconSize; }
	fvector_2d GetIconEndPosition()const { return IconPosition + IconSize; }

	bool IsInTheScope(const fvector_2d& InCurrentPosition) const;

protected:
	fvector_2d IconPosition;
	fvector_2d IconSize;
};