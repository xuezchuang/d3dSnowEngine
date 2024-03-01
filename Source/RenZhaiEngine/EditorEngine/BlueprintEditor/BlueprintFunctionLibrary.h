#pragma once
#include "../../Engine/Library/Core/BlueprinFunctiontLibrary.h"
#include "BlueprintFunctionLibrary.CodeReflection.h"

class FGraphicBlueprintEditor;
class FBlueprintNode;
class FNode;

class GBlueprintFunctionLibrary :public GBlueprinFunctiontLibrary
{
	CODEREFLECTION()

public:
	GBlueprintFunctionLibrary(){}

public:
	//获取函数名字，如果存在显示名字 优先选择显示名字
	static string GetFunction(CFunctionObject* InFunction);

	static std::shared_ptr<FBlueprintNode> CreateBulueprintNode(CFunctionObject* InFunction, FGraphicBlueprintEditor* InGraphicBlueprint, const fvector_2d& InOffsetPos = fvector_2d(FLT_MIN));

	static std::shared_ptr<FBlueprintNode> CreateBulueprintNodeByName(CFunctionObject* InFunction, FGraphicBlueprintEditor* InGraphicBlueprint, const char* InBlueprintName, const fvector_2d& InOffsetPos = fvector_2d(FLT_MIN));
public:
	//编译
	static void Compile(std::shared_ptr<FGraphicBlueprintEditor> InGraphic);

	//模拟播放
	static void SimulationPlay(const string& InClassName, const string& EventName, void* InParam = nullptr);

private:
	//获取事件蓝图
	static vector<shared_ptr<FBlueprintNode>> GetEventBlueprintRoot(std::shared_ptr<FGraphicBlueprintEditor> InGraphic);

	//修剪孤立的蓝图节点
	static void PruneSolatedNodes(const vector<shared_ptr<FBlueprintNode>>& RootEvent, vector<vector<shared_ptr<FBlueprintNode>>>& OutVisitedNodes);

	//寻找事件链接下的所以节点
	static void TraverseNodes(shared_ptr<FBlueprintNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);

	//寻找事件链接下的所以节点
	static void TraverseParamPureNodes(shared_ptr<FBlueprintNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);

	//判定是否包含
	static bool Contains(shared_ptr<FNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);

	//清除上次的字节码
	static void ClearBytecode(vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);
};