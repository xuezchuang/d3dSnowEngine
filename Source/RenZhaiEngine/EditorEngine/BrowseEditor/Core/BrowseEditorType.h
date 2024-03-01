#pragma once
#include "../../../Engine/EngineMinimal.h"

enum EResetFileType
{
	RESETFILETYPE_NONE,
	RESETFILETYPE_ALLOW_FILE,//������ʾ���ļ�
	RESETFILETYPE_PATH,//·�� �ļ���
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