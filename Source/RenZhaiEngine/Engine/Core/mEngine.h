#pragma once
#include "include/Game.h"
#include "include/CommandContext.h"

class CEditorEngine;

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

#if EDITOR_ENGINE
	CEditorEngine* GetEditorEngine()const;
#endif // 0

	std::chrono::high_resolution_clock::time_point tStart, tEnd;
	int m_SizeX = 0;
	int m_SizeY = 0;;
};