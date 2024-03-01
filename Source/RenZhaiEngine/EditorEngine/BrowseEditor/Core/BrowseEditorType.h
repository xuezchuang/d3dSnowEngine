#pragma once
#include "../../../Engine/EngineMinimal.h"

enum EResetFileType
{
	RESETFILETYPE_NONE,
	RESETFILETYPE_ALLOW_FILE,//允许显示的文件
	RESETFILETYPE_PATH,//路径 文件夹
};

enum EAssetOperationCommand
{
	ASSETOPERATION_NONE,
	ASSETOPERATION_DRAG,
};

struct FAssetGeometry
{
	ImVec2 CanvasSize;
	fvector_2d DeltaValue;
	fvector_2d MousePos;

	EAssetOperationCommand OperationCommand;
};