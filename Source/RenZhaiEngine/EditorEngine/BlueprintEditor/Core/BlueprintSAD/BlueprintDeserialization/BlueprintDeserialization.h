#pragma once

#include "../../../../../Engine/EngineMinimal.h"
#include "../BlueprintSADType.h"

class FGraphicBlueprintEditor;
namespace BlueprintDeserialization
{
	//����ͼ���ݷ������ɵ�����
	bool Deserialization(FGraphicBlueprintEditor* GraphicBlueprintEditor, const FBlueprintDatas& InDatas);

	bool Deserialization(const string& InBlueprintString, FBlueprintDatas& OutData);

	bool Deserialization(const vector<string>& BlueprintStrings, FBlueprintDatas& OutData);
}