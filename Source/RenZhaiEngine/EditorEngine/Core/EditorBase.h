#pragma once
#include "../../Engine/EngineMinimal.h"

class FEditorBase :public std::enable_shared_from_this<FEditorBase>
{
public:
	virtual void BuildEditor() = 0;
	virtual void DrawEditor(float DeltaTime) = 0;
	virtual void ExitEditor() = 0;
};