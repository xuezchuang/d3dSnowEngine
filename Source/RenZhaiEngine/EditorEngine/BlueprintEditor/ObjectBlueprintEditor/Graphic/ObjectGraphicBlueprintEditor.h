#pragma once
#include "../../Core/BlueprintGraphic/GraphicBlueprintEditor.h"

class FBlueprintNodeMenuEditor;
class FObjectGraphicBlueprintEditor :public FGraphicBlueprintEditor 
{
	typedef FGraphicBlueprintEditor Super;
public:
	FObjectGraphicBlueprintEditor();

	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

	virtual std::shared_ptr<FBlueprintNode> CreateBlueprintNode(const fvector_2d& InNewOffsetPosition, const fvector_2d& InNewOriginPosition, CFunctionObject* InFunction);

protected:
	virtual std::shared_ptr<FBlueprintDrawConnection> MakePinConnection();

	std::shared_ptr<FBlueprintNodeMenuEditor> Menu;
};