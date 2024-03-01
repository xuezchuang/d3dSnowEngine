#pragma once
#include "Viewport/Viewport.h"
#include "Viewport/ClientViewport.h"
#include "CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterfece.h"
#include "../Component/TimelineComponent.h"
#include "Camera.CodeReflection.h"

enum ECmeraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class GCamera :public GClientViewport,public IDirectXDeviceInterfece
{
	CODEREFLECTION()

	CVARIABLE()
	CInputComponent* InputComponent;
public:
	GCamera();

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	virtual void ExecuteKeyboard(const FInputKey& InputKey);

	virtual void BuildViewMatrix(float DeltaTime);

public:
	virtual void OnLeftMouseButtoUP(int X, int Y);
	virtual void OnLeftMouseButtonDown(int X, int Y);
	virtual void OnMouseButtonDown(int X, int Y);//Right
	virtual void OnMouseButtonUp(int X, int Y);//Right
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);

	virtual void MoveForward(float InValue);
	virtual void MoveRight(float InValue);

public:
	virtual void OnClickedScreen(int X, int Y);

protected:
	void RotateAroundXAxis(float InRotateDegrees);
	void RotateAroundYAxis(float InRotateDegrees);

protected:
	void LookAtAndMoveToSelectedObject(float InTime,float InDeltaTime);

public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }

protected:
	POINT LastMousePosition;
	bool bLeftMouseDown;
	bool bRightMouseDown;

	float MouseSensitivity;
	ECmeraType CmeraType;

	//ÇòÃæ×ø±ê
	float Radius;
	float A;//Theta
	float B;//

	class CMeshComponent* SphereMesh;
	fvector_3d FocusPoint;

	bool bFPress;
	FTimeline Timeline;
};