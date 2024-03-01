#include "EngineRenderConfig.h"

FEngineRenderConfig* FEngineRenderConfig::RenderConfig = nullptr;

FEngineRenderConfig::FEngineRenderConfig()
	//:ScrrenWidth(1920)
	//,ScrrenHight(1080)
	:ScrrenWidth(800)
	,ScrrenHight(600)
	,RefreshRate(60)
	,SwapChainCount(2)
{
}

FEngineRenderConfig* FEngineRenderConfig::GetRenderConfig()
{
	if (!RenderConfig)
	{
		RenderConfig = new FEngineRenderConfig();
	}

	return RenderConfig;
}

void FEngineRenderConfig::Destroy()
{
	if (RenderConfig)
	{
		delete RenderConfig;
		RenderConfig = nullptr;
	}
}
