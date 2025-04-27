#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();

public:
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);

	virtual void OnResetSize(int InWidth, int InHeight);
public:
	//和屏幕的视口有关
	D3D12_VIEWPORT ViewprotInfo;
	D3D12_RECT ViewprotRect;

	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;

	// 右手坐标系矩阵（专用于 HBAO+）
	XMFLOAT4X4 ViewMatrixRH;
	XMFLOAT4X4 ProjectMatrixRH;

};