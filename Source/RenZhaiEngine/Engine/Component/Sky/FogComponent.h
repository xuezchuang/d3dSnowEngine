#pragma once

#include "Component/Component.h"
#include "FogComponent.CodeReflection.h"

class CFogComponent :public CComponent
{
	CODEREFLECTION()
public:
	CFogComponent();

	virtual void Tick(float DeltaTime);

	void SetFogColor(const fvector_color &InNewColor);
	void SetFogStart(float InNewFogStart);
	void SetFogRange(float InNewFogRange);
	void SetFogHeight(float InNewFogHeight);
	void SetFogTransparentCoefficient(float InNewFogTransparentCoefficient);
	void SetDirty(float InNewDirty);

public:
	FORCEINLINE fvector_color GetFogColor()const { return FogColor; }
	FORCEINLINE float GetFogStart()const { return FogStart; }
	FORCEINLINE float GetFogRange()const { return FogRange; }
	FORCEINLINE float GetFogHeight()const { return FogHeight; }
	FORCEINLINE float GetFogTransparentCoefficient()const { return FogTransparentCoefficient; }
	FORCEINLINE bool IsDirty()const { return bDirty; }
protected:
	fvector_color FogColor;

	float FogStart;
	float FogRange;
	float FogHeight;
	float FogTransparentCoefficient;

	bool bDirty;
};