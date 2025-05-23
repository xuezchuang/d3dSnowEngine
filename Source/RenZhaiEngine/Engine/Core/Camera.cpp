#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"
#include "CameraType.h"
#include "../Config/EngineRenderConfig.h"
#include "../Library/RaycastSystemLibrary.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "../Component/Mesh/Core/MeshComponentType.h"
#include "../Math/EngineMath.h"
#include "../Core/Construction/MacroConstruction.h"
#include "CoreMacro.h"

#if EDITOR_ENGINE
#include "../../Common/OperationHandleSelectManage.h"
#endif // 0

extern CMeshComponent* SelectAxisComponent;
extern GActorObject* SelectedObject;

GCamera::GCamera()
	:Super()
{
	BUILD_OBJECT_PARAMETERS(, this);
	InputComponent = CreateObject<CInputComponent>(Param, new CInputComponent());
	
	MouseSensitivity = 0.7f;
	CmeraType = ECmeraType::CameraRoaming;

	Radius = 10.f;
	A = XM_PI;//
	B = XM_PI;

	bRightMouseDown = false;
	bFPress = false;
}

void GCamera::BeginInit()
{
	//初始化我们的投影矩阵
	float AspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScrrenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScrrenHight;
	//(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)
	//基于视野构建左手透视投影矩阵
	SetFrustum(
		0.25f * XM_PI,//以弧度为单位的自上而下的视场角。
		//XMConvertToRadians(60.0f),
		AspectRatio,//视图空间 X:Y 的纵横比。
		1.0f,//到近剪裁平面的距离。必须大于零。
		10000.f);//到远剪裁平面的距离。必须大于零。

	InputComponent->CaptureKeyboardInforDelegate.Bind(this, &GCamera::ExecuteKeyboard);

	InputComponent->OnLMouseButtonUpDelegate.Bind(this, &GCamera::OnLeftMouseButtoUP);
	InputComponent->OnLMouseButtonDownDelegate.Bind(this, &GCamera::OnLeftMouseButtonDown);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CmeraType = ECmeraType::CameraRoaming;

	Timeline.Tick(DeltaTime);
}

void GCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if (bLeftMouseDown || bRightMouseDown)
	{
		if (InputKey.KeyName == "W")
		{
			MoveForward(1.f);

			SetDirty(true);
		}
		else if (InputKey.KeyName == "S")
		{
			MoveForward(-1.f);

			SetDirty(true);
		}
		else if (InputKey.KeyName == "A")
		{
			MoveRight(-1.f);

			SetDirty(true);
		}
		else if (InputKey.KeyName == "D")
		{
			MoveRight(1.f);

			SetDirty(true);
		}
	}

	//if (InputKey.KeyName == "alt")
	//{
	//	CmeraType = ECmeraType::ObservationObject;

	//	SetDirty(true);
	//}

	if (InputKey.KeyName == "F")
	{
		if (!bFPress)
		{
			FTimelineDelegate TimelineDelegate;
			TimelineDelegate.Bind(this, &GCamera::LookAtAndMoveToSelectedObject);
			Timeline.BindTimeLineEvent(0.4f, TimelineDelegate);

			bFPress = true;
		}
	}
}

void GCamera::BuildViewMatrix(float DeltaTime)
{
	switch (CmeraType)
	{
		case CameraRoaming:
		{
			Super::BuildViewMatrix(DeltaTime);
			break;
		}
		case ObservationObject:
		{
			XMFLOAT3 &CameraPos = GetRootComponent()->GetPosition();

			CameraPos.x = Radius * sinf(B) * cosf(A);
			CameraPos.z = Radius * sinf(B) * sinf(A);
			CameraPos.y = Radius * cosf(B);
			
			XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f);
			XMVECTOR ViewTarget = XMVectorZero();

			XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

			XMMATRIX ViewLookAt = XMMatrixLookAtRH(Pos, ViewTarget, ViewUp);
			XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

			break;
		}
	}
}

void GCamera::OnLeftMouseButtoUP(int X, int Y)
{
	bLeftMouseDown = false;
}

void GCamera::OnLeftMouseButtonDown(int X, int Y)
{
	bLeftMouseDown = true;

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	OnClickedScreen(X, Y);

	SetCapture(GetMianWindowsHandle());
}

void GCamera::OnMouseButtonDown(int X, int Y)
{
	bRightMouseDown = true;

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMianWindowsHandle());

	SetDirty(true);
}

void GCamera::OnMouseButtonUp(int X, int Y)
{
	bRightMouseDown = false;

	ReleaseCapture();

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetDirty(true);
}

void GCamera::OnMouseMove(int X, int Y)
{
#if EDITOR_ENGINE
	if (FOperationHandleSelectManage::Get()->IsCaptureMouseNotOnUI())
#endif
	{
		if (bRightMouseDown)
		{
			float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * MouseSensitivity);
			float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * MouseSensitivity);

			switch (CmeraType)
			{
				case CameraRoaming:
				{
					RotateAroundXAxis(YRadians);
					RotateAroundYAxis(XRadians);
					break;
				}
				case ObservationObject:
				{
					A += (-XRadians);
					B += YRadians;

					A = math_libray::Clamp(A, 0.f, XM_2PI * 2.f);
					break;
				}
			}
		}

		LastMousePosition.x = X;
		LastMousePosition.y = Y;

		SetDirty(true);
	}
}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
#if EDITOR_ENGINE
	if (FOperationHandleSelectManage::Get()->IsCaptureMouseNotOnUI())
#endif
	{
		if (CmeraType == ECmeraType::ObservationObject)
		{
			Radius += (InDelta / 100.f);

			//限制在一定的范围内
			Radius = math_libray::Clamp(Radius, 7.f, 40.f);
		}

		SetDirty(true);
	}
}

void GCamera::MoveForward(float InValue)
{
#if EDITOR_ENGINE
	if (FOperationHandleSelectManage::Get()->IsCaptureMouseNotOnUI())
#endif

	if (CmeraType == ECmeraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = GetRootComponent()->GetPosition();
		XMFLOAT3 AT3ForwardVector = GetRootComponent()->GetForwardVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));
		GetRootComponent()->SetPosition(AT3Position);
	}
}

void GCamera::MoveRight(float InValue)
{
#if EDITOR_ENGINE
	if (FOperationHandleSelectManage::Get()->IsCaptureMouseNotOnUI())
#endif
	{
		if (CmeraType == ECmeraType::CameraRoaming)
		{
			XMFLOAT3 AT3Position = GetRootComponent()->GetPosition();
			XMFLOAT3 AT3RightVector = GetRootComponent()->GetRightVector();

			XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
			XMVECTOR Right = XMLoadFloat3(&AT3RightVector);
			XMVECTOR Position = XMLoadFloat3(&AT3Position);

			XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
			GetRootComponent()->SetPosition(AT3Position);
		}
	}
}

void GCamera::OnClickedScreen(int X, int Y)
{
#if EDITOR_ENGINE
	if (FOperationHandleSelectManage::Get()->IsCaptureMouseNotOnUI())
	{
		if (!SelectAxisComponent)
		{
			FCollisionResult CollisionResult;
			FRaycastSystemLibrary::HitResultByScreen(GetWorld(), X, Y, CollisionResult);

			if (CollisionResult.bHit)
			{
				Engine_Log("Clicked successfully.[time]=%f", CollisionResult.Time);

				if (FRenderLayerManage* InLayer = GetRenderLayerManage())
				{
					InLayer->HighlightDisplayObject(CollisionResult.RenderingData);
				}

				//设置选择对象
				FOperationHandleSelectManage::Get()->SetNewSelectedObject(CollisionResult.Actor);

				//显示操作手柄
				FOperationHandleSelectManage::Get()->DisplaySelectedOperationHandle();

			}
			else
			{
				if (FRenderLayerManage* InLayer = GetRenderLayerManage())
				{
					InLayer->Clear(EMeshRenderLayerType::RENDERLAYER_SELECT);
				}

				//设置选择对象
				FOperationHandleSelectManage::Get()->SetNewSelectedObject(nullptr);

				//显示操作手柄
				FOperationHandleSelectManage::Get()->HideSelectedOperationHandle();
			}
		}
	}
#endif
}

void GCamera::RotateAroundXAxis(float InRotateDegrees)
{
	//拿到相机的方向
	XMFLOAT3 RightVector = GetRootComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetRootComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetRootComponent()->GetForwardVector();

	//拿到关于Y的旋转矩阵
	XMMATRIX RotationY = XMMatrixRotationAxis(XMLoadFloat3(&GetRootComponent()->GetRightVector()),InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	//XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
	XMStoreFloat3(&GetRootComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}

void GCamera::RotateAroundYAxis(float InRotateDegrees)
{
	//拿到相机的方向
	XMFLOAT3 RightVector = GetRootComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetRootComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetRootComponent()->GetForwardVector();

	//拿到关于Z的旋转矩阵
	XMMATRIX RotationZ = XMMatrixRotationY(InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	XMStoreFloat3(&GetRootComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
	XMStoreFloat3(&GetRootComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationZ));
	XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));
}

void GCamera::LookAtAndMoveToSelectedObject(float InTime, float InDeltaTime)
{
	if (SelectedObject)
	{
		BoundingBox SelectAABB;
		SelectedObject->GetBoundingBox(SelectAABB);

		fvector_3d Extents = EngineMath::ToVector3d(SelectAABB.Extents);

		//离选择对象的距离
		float R = Extents.len();
		float H = 5.f;
		float FOV = GetFOV();
		assert(FOV != 0.f);

		float L = (R + H) / tan(FOV / 2.f);

		fvector_3d CameraPosition = EngineMath::ToVector3d(GetPosition());
		fvector_3d SelectedObjectPosition = EngineMath::ToVector3d(SelectedObject->GetPosition());

		fvector_3d CameraForwardVector = SelectedObjectPosition - CameraPosition;
		CameraForwardVector.normalize();

		fvector_3d CameraEndPosition = SelectedObjectPosition + CameraForwardVector * (-1.f) * L;

		fvector_3d CurrentCameraPosition = EngineMath::Lerp(CameraPosition, CameraEndPosition, InDeltaTime * 2.f / InTime);
		
		SetPosition(EngineMath::ToFloat3(CurrentCameraPosition));

		//是否启用四元数
		float LerpSpeed = 4.f / InTime;
		if (true)
		{
			fquat Q1 = GetRotationQuat();
			fquat Q2 = EngineMath::BuildQuat(CameraForwardVector);

			fquat CurrentQ = fquat::lerp(Q1, Q2, InDeltaTime * LerpSpeed);

			SetRotationQuat(CurrentQ);
		}
		//欧拉角做插值
		else
		{
			frotator Rotator1 = GetRotation();
			frotator Rotator2 = EngineMath::BuildRotatorMatrix(CameraForwardVector);

			frotator CurrentRotator = EngineMath::Lerp(Rotator1, Rotator2, InDeltaTime * LerpSpeed);
		
			SetRotation(CurrentRotator);

			//Engine_Log("Rotator1 y=%f,r=%f,p=%f", Rotator1.yaw, Rotator1.roll,Rotator1.pitch);
			//Engine_Log("Rotator2 y=%f,r=%f,p=%f", Rotator2.yaw, Rotator2.roll, Rotator2.pitch);
		}
	}

	bFPress = false;
}
