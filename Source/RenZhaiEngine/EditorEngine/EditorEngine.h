#pragma once
#include "../Engine/Core/Engine.h"
#include "Core/EditorBase.h"
#include "imgui.h"

class CEditorEngine :public CEngine,public FEditorBase
{
public:
	CEditorEngine();

	virtual int PreInit();

	virtual int Init() ;
	virtual int PostInit();

	virtual void Tick(float DeltaTime) {};
	virtual void OnResetSize(int InWidth, int InHeight,int nWara);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

public:
	virtual void BuildEditor();
	virtual void DrawEditor(float DeltaTime);
	virtual void ExitEditor();

public:
	void DrawLayer(float DeltaTime);

protected:
	std::vector<std::shared_ptr<FEditorBase>> Editors;
};