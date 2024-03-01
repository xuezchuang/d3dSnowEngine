#include "ViewportDataManage.h"
#include "../Config/EngineRenderConfig.h"

FViewportDataManage::FViewportDataManage()
	:Width(FEngineRenderConfig::GetRenderConfig()->ScrrenWidth)
	,Height(FEngineRenderConfig::GetRenderConfig()->ScrrenHight)
	,YFOV(0.25f * XM_PI)
	,Aspect((float)Width / (float)Height)
	,ZNear(0.1f)
	,ZFar(10000.f)
{

}

void FViewportDataManage::ResetSize(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;

	Aspect = (float)InWidth / (float)InHeight;
}
