#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterfece.h"
#include "LightType.h"
#include "LightComponent.CodeReflection.h"

class CMeshComponent;
class CLightComponent :public CTransformationComponent,public IDirectXDeviceInterfece
{
	CODEREFLECTION()

	CVARIABLE()
	CMeshComponent* LightMesh;
public:
	CLightComponent();

	virtual ~CLightComponent();

	void SetLightIntensity(const fvector_3d& InNewLightIntensity);
public:
	CMeshComponent* GetLightMesh() { return LightMesh; }
	const fvector_3d &GetLightIntensity() {return LightIntensity;}
	ELightType GetLightType() const { return LightType; }
public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	virtual void SetRotation(const fvector_3d& InNewRotation);
	virtual void SetScale(const fvector_3d& InNewScale);

	virtual void SetForwardVector(const XMFLOAT3& InForwardVector);
	virtual void SetRightVector(const XMFLOAT3& InRightVector);
	virtual void SetUPVector(const XMFLOAT3& InUPVector);

protected:
	void SetLightMesh(CMeshComponent *InLightMesh);

protected:
	fvector_3d LightIntensity;
	ELightType LightType;
};
