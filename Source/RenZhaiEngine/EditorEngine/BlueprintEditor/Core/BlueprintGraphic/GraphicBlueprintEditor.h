#pragma once
#include "../../../Core/EditorBase.h"
#include "../BlueprintEditorType.h"
#include "../../../../Engine/Component/TimelineComponent.h"

class FBlueprintDrawConnection;
class FBlueprintNode;
class CFunctionObject;
class CCoreMinimalObject;

class FGraphicBlueprintEditor :public FEditorBase
{
	friend class GBlueprintFunctionLibrary;
	typedef FEditorBase Super;

public:
	FGraphicBlueprintEditor();

	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	float GetZoomRatio()const;

	void ZoomBlueprintNodeSize(float InRatio);
	void ZoomBlueprintNodeOffset(float InRatio);

	//链接两个pin
	void LinkPin(std::shared_ptr<FNode> InStartPin, std::shared_ptr<FNode> InEndPin);
public:
	virtual std::shared_ptr<FBlueprintNode> CreateBlueprintNode(const fvector_2d& InNewOffsetPosition, const fvector_2d& InNewOriginPosition, CFunctionObject* InFunction) { return nullptr; }

public:
	void StructureGeometryParam(FCanvasGeometry& OutGeometry);
	std::shared_ptr<FNode> IsInPinRange(const fvector_2d& InCurrentPosition);

public:
	virtual void Save();
	virtual void Load();

protected:
	void ComputationalGeometry(float DeltaTime);
	void DrawBackground(float DeltaTime);
	void DrawGrid(float DeltaTime);

	void ComputationalZoom(float DeltaTime);
	void DrawZoomText(float DeltaTime);

	//绘制选取
	void ComputationalSelection(float DeltaTime);
	void DrawSelection(float DeltaTime);

	void DrawBlueprintNode(float DeltaTime);

protected:
	void CheckKeyboard(float DeltaTime);

	void OnDelete();
	void OnLookAt();
protected:
	void LookAtTargetBlueprintNode(float InTime, float InDeltaTime);

public:
	int GetSelectedNodeIndex();
	bool IsExitIndexArrays(int InNewIndex);

	void SetBlueprintAssetName(const std::string& InNewName) { BlueprintName = InNewName; }

	FORCEINLINE std::vector<std::shared_ptr<FBlueprintNode>>* GetBlueprintNodes() { return &Nodes; }
	FORCEINLINE std::string GetBlueprintAssetName() const { return BlueprintName; }
	const FGraphicBlueprintGeometry *GetGeometry() { return &Geometry; };
	CCoreMinimalObject* GetGraphicBlueprintObject();
protected:
	void BlueprintNodeInteraction(float DeltaTime,const FCanvasGeometry &InGeometry);

	virtual std::shared_ptr<FBlueprintDrawConnection> MakePinConnection();

protected:
	FGraphicBlueprintGeometry Geometry;//记录面板的位置信息
	std::vector<std::shared_ptr<FBlueprintNode>> Nodes;
	std::shared_ptr<FBlueprintDrawConnection> DrawConnection;
	FTimeline Timeline;

	std::string BlueprintName;//蓝图资源的名字
};