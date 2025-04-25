#include "Viewport.h"
#include "../../Mesh/Core/ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Math/EngineMath.h"

FViewport::FViewport()
	:ViewMatrix(EngineMath::IdentityMatrix4x4())
    ,ProjectMatrix(EngineMath::IdentityMatrix4x4())
{
    ResetScissorRect(
        FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,
        FEngineRenderConfig::GetRenderConfig()->ScrrenHight);

    ResetViewport(
        FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,
        FEngineRenderConfig::GetRenderConfig()->ScrrenHight);
}

void FViewport::ResetViewport(UINT InWidth, UINT InHeight)
{
    ViewprotInfo =
    {
        0.0f,//TopLeftX
        0.0f,//TopLeftY
        (float)InWidth,//Width
        (float)InHeight,//Height
        0.0f,//MinDepth
        1.0f //MaxDepth
    };
}

void FViewport::ResetScissorRect(UINT InWidth, UINT InHeight)
{
    ViewprotRect =
    {
        0,//left
        0,//top
        (LONG)InWidth,//right
        (LONG)InHeight//bottom
    };
}

void FViewport::OnResetSize(int InWidth, int InHeight)
{ 
    ResetScissorRect(InWidth,InHeight);
    ResetViewport(InWidth, InHeight);
}
