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
	//��ȡ�������֣����������ʾ���� ����ѡ����ʾ����
	static string GetFunction(CFunctionObject* InFunction);

	static std::shared_ptr<FBlueprintNode> CreateBulueprintNode(CFunctionObject* InFunction, FGraphicBlueprintEditor* InGraphicBlueprint, const fvector_2d& InOffsetPos = fvector_2d(FLT_MIN));

	static std::shared_ptr<FBlueprintNode> CreateBulueprintNodeByName(CFunctionObject* InFunction, FGraphicBlueprintEditor* InGraphicBlueprint, const char* InBlueprintName, const fvector_2d& InOffsetPos = fvector_2d(FLT_MIN));
public:
	//����
	static void Compile(std::shared_ptr<FGraphicBlueprintEditor> InGraphic);

	//ģ�ⲥ��
	static void SimulationPlay(const string& InClassName, const string& EventName, void* InParam = nullptr);

private:
	//��ȡ�¼���ͼ
	static vector<shared_ptr<FBlueprintNode>> GetEventBlueprintRoot(std::shared_ptr<FGraphicBlueprintEditor> InGraphic);

	//�޼���������ͼ�ڵ�
	static void PruneSolatedNodes(const vector<shared_ptr<FBlueprintNode>>& RootEvent, vector<vector<shared_ptr<FBlueprintNode>>>& OutVisitedNodes);

	//Ѱ���¼������µ����Խڵ�
	static void TraverseNodes(shared_ptr<FBlueprintNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);

	//Ѱ���¼������µ����Խڵ�
	static void TraverseParamPureNodes(shared_ptr<FBlueprintNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);

	//�ж��Ƿ����
	static bool Contains(shared_ptr<FNode> Node, vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);

	//����ϴε��ֽ���
	static void ClearBytecode(vector<shared_ptr<FBlueprintNode>>& OutVisitedNodes);
};