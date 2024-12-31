#pragma once
#include "include/Game.h"
#include "include/CommandContext.h"

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


};