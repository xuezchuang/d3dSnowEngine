#pragma once
#include "../../../../../Engine/EngineMinimal.h"
#include "../BlueprintSADType.h"

class FGraphicBlueprintEditor;
namespace BlueprintSerialize
{
	//����ͼ����µ������������л�ΪFBlueprintDatas�ṹ
	bool Serialize(FBlueprintDatas& Out, FGraphicBlueprintEditor* GraphicBlueprintEditor);


	//��FBlueprintDatas���л�Ϊvector<string>����
	bool Serialize(const FBlueprintDatas& InData, vector<string>& InBlueprintString);
	//��FBlueprintNodeData���л�Ϊvector<string>����
	bool Serialize(const FBlueprintNodeData& InData, vector<string>& InBlueprintString);
	//�������л�����
	bool Serialize(const FCustomProperties& InData, string& InBlueprintString);
}