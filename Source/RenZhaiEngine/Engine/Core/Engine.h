#pragma once

#include "CoreObject/CoreMinimalObject.h"

#if defined(_WIN32)
#include "WinMainCommandParameters.h"
#endif // 1

class CEngine :public CCoreMinimalObject
{
public:
	CEngine();

	virtual int PreInit() = 0;

	virtual int Init() = 0;
	virtual int PostInit() = 0;

	virtual void Tick(float DeltaTime) {};
	virtual void OnResetSize(int InWidth, int InHeight, int wParam) = 0;


	virtual int PreExit() = 0;
	virtual int Exit() = 0;
	virtual int PostExit() = 0;
};