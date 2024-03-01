#pragma once
#include "Node.h"
#include "../BlueprintEditorType.h"

class CPropertyObject;
class FBlueprintPin :public FNode
{
	friend class GBlueprintFunctionLibrary;

	typedef FNode Super;
public:
	FBlueprintPin();

public:
	virtual void BuildEditor(const FCanvasGeometry& InGeometry);
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

	void CheckLinkPins(float DeltaTime);
	void AddLinkPin(std::shared_ptr<FNode> InPin);
public:
	virtual ImColor GetPinColor() const { return IM_COL32(255, 255, 255, 255); }
	virtual ImColor GetPinTextColor() const { return IM_COL32(255, 255, 255, 255); }
	vector<std::weak_ptr<FNode>>* GetLinkPins() { return &LinkPins; }
public:
	virtual void DrawText(float DeltaTime);
	virtual void DrawPin(float DeltaTime);

public:
	virtual void OnMouseHovered(const FCanvasGeometry& InGeometry);
	virtual void OnMouseClicked(const FCanvasGeometry& InGeometry);
	virtual void OnMouseDrag(const FCanvasGeometry& InGeometry);

public:
	virtual void ZoomSize(float InRatio);
	virtual void ZoomOffset(float InRatio);

public:
	float GetZoomRatio() const;
	CPropertyObject *GetProperty()const { return Property; }

public:
	void SetType(EBlueprintPinType InType) { Type = InType; }
	EBlueprintPinType GetType() const { return Type; }
	void SetProperty(CPropertyObject* InProperty);

protected:
	EBlueprintPinType Type;
	CPropertyObject* Property;

	vector<std::weak_ptr<FNode>> LinkPins;
};