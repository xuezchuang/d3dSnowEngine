#pragma once
#include "../../../../../Engine/EngineMinimal.h"
#include "../BlueprintSADType.h"

class FGraphicBlueprintEditor;
namespace BlueprintSerialize
{
	//将蓝图面板下的所有内容序列化为FBlueprintDatas结构
	bool Serialize(FBlueprintDatas& Out, FGraphicBlueprintEditor* GraphicBlueprintEditor);


	//将FBlueprintDatas序列化为vector<string>类型
	bool Serialize(const FBlueprintDatas& InData, vector<string>& InBlueprintString);
	//将FBlueprintNodeData序列化为vector<string>类型
	bool Serialize(const FBlueprintNodeData& InData, vector<string>& InBlueprintString);
	//单独序列化变量
	bool Serialize(const FCustomProperties& InData, string& InBlueprintString);
}