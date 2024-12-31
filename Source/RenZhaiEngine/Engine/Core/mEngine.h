#pragma once
#include "include/Game.h"
#include "include/CommandContext.h"

class CEditorEngine;
class CDirectXRenderingEngine;

class mEngine : public FGame
{
public:
	mEngine(const GameDesc& Desc) : FGame(Desc)
	{
	}

	virtual void OnStartup() override;

	virtual void OnUpdate() override;

	virtual void OnRender() override;
private:
	void OnGUI(FCommandContext& CommandContext);
	void OnTest(FCommandContext& CommandContext);
	std::chrono::high_resolution_clock::time_point tStart, tEnd;
	int m_SizeX = 0;
	int m_SizeY = 0;


	CEditorEngine* GetEditorEngine()const;
	CDirectXRenderingEngine* GetDirectRenderingEngine()const;

};