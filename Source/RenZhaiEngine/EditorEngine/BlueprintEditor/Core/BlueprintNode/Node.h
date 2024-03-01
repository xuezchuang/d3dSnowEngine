#pragma once
#include "../../../../Engine/EngineMinimal.h"
#include "../BlueprintEditorType.h"
#include "CoreObject/GuidInterface.h"

class FNode :public std::enable_shared_from_this<FNode>, public IGuidInterface
{
public:
	FNode();
public:
	virtual void BuildEditor(const FCanvasGeometry &InGeometry) = 0;
	virtual void DrawEditor(float DeltaTime) = 0;
	virtual void ExitEditor() = 0;

public:
	virtual void OnMouseHovered(const FCanvasGeometry& InGeometry){}
	virtual void OnMouseClicked(const FCanvasGeometry& InGeometry){}
	virtual void OnMouseDrag(const FCanvasGeometry& InGeometry) {}
	virtual void OnHighlight() {}

public:
	void SetName(const string& InNewNodeName) { NodeName = InNewNodeName; }

	void SetOuter(std::shared_ptr<FNode> InNode) { Outer = InNode; }

	void SetOffsetPosition(const fvector_2d& InPosition) { OffsetPosition = InPosition; }
	void SetSize(const fvector_2d& InNodeSize) { NodeSize = InNodeSize; }
	void SetStandardOffsetPosition(const fvector_2d& InNodeSize) { StandardOffsetPosition = InNodeSize; }
	void SetOriginPosition(const fvector_2d& InPosition) { OriginPosition = InPosition; }

	fvector_2d GetStandardOffsetPosition() const { return StandardOffsetPosition; }
	fvector_2d GetPosition()const { return OriginPosition + OffsetPosition; }
	fvector_2d GetOffsetPosition()const { return OffsetPosition; }
	fvector_2d GetSize()const { return NodeSize; }
	fvector_2d GetEndPosition()const { return OriginPosition + OffsetPosition + NodeSize; }
	
	std::shared_ptr<FNode> GetOuter() const { return Outer.lock(); }

	bool IsInTheScope(const fvector_2d& InCurrentPosition) const;

	const string& GetName() const { return NodeName; }
public:
	virtual void ZoomSize(float InRatio);
	virtual void ZoomOffset(float InRatio);

protected:
	std::string NodeName;
	fvector_2d OriginPosition;//原点
	fvector_2d OffsetPosition;//相对位置
	fvector_2d StandardOffsetPosition;//主要用于缩放
	fvector_2d NodeSize;

private:
	std::weak_ptr<FNode> Outer;
};