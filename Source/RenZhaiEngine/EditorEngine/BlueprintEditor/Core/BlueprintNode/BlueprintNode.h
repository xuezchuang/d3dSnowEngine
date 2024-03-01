#pragma once
#include "Node.h"
#include "../BlueprintEditorType.h"

enum EConstructionPinType
{
	ACCORDING_PROPERTY_TYPE,//自适应
	ALL_INPUT_PARAM,//设置所有pin Input
	ALL_OUTPUT_PARAM,//设置所有 pin output
};

class CPropertyObject;
class CFunctionObject;
struct FBlueprintPin;
class FBlueprintNode :public FNode
{
	friend class FBlueprintCompilerContext;
	friend class GBlueprintFunctionLibrary;
	typedef FNode Super;
public:
	FBlueprintNode();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();
public:
	virtual void OnMouseHovered(const FCanvasGeometry& InGeometry);
	virtual void OnMouseClicked(const FCanvasGeometry& InGeometry);
	virtual void OnMouseDrag(const FCanvasGeometry& InGeometry);
	virtual void OnHighlight();

public:
	virtual ImColor GetBodyColor() const { return IM_COL32(124.f, 124.f, 150.f, 255.0f); }
	virtual ImColor GetTopFrontColor() const { return IM_COL32(100, 100, 200, 255); }
	virtual ImColor GetTopBackColor() const { return IM_COL32(100, 100, 200, 100); }
	virtual ImColor GetTextColor() const { return IM_COL32(255, 255, 255, 255); }

public:
	virtual std::shared_ptr<FBlueprintPin> AddPin(EBlueprintPinType InType, CPropertyObject* InPinProperty);
	virtual std::shared_ptr<FBlueprintPin> AddPin(EBlueprintPinType InType, const FCanvasGeometry& InGeometry, CPropertyObject* InPinProperty);

	virtual std::shared_ptr<FBlueprintPin> MakePin(CPropertyObject* InPinProperty) = 0;

	int GetOutputNumber() const { return OutputNumber; }
	int GetInputNumber() const { return InputNumber; }
	CFunctionObject* GetFunction()const { return Function; }

public:
	void ZoomPinsSize(float InRatio);
	void ZoomPinsOffset(float InRatio);
	float GetZoomRatio() const;

	std::shared_ptr<FNode> IsInPinRange(const fvector_2d& InCurrentPosition);

	void ConstructionPin(CPropertyObject* InProperty, const FCanvasGeometry& InGeometry, EConstructionPinType InType = EConstructionPinType::ACCORDING_PROPERTY_TYPE);
protected:
	void BuildPinsNameLenBasedPixels();
	void BlueprintNodeSelfAdaption(float InRatio);

public:
	void SetType(EBlueprintNodeType InType) { Type = InType; }
	EBlueprintNodeType GetType() const { return Type; }
	void SetFunction(CFunctionObject* NewFunction);

	FORCEINLINE std::vector<std::shared_ptr<FBlueprintPin>>* GetPins() { return &Pins; }
public:
	virtual void ZoomSize(float InRatio);
	virtual void ZoomOffset(float InRatio);

protected:
	void BlueprintPinInteraction( const FCanvasGeometry& InGeometry);

protected:
	void DrawPins(float DeltaTime);

protected:
	EBlueprintNodeType Type;
	std::vector<std::shared_ptr<FBlueprintPin>> Pins;

	int OutputNumber;
	int InputNumber;

	int InputNameMaxLen;
	int OutputNameMaxLen;

	fvector_2d StandardNodeSize;

	CFunctionObject* Function;
};
