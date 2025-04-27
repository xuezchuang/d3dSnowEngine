#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../Core/MeshManage.h"
#include "Mesh.CodeReflection.h"

class CTransformationComponent;
class CMeshComponent;
class CMaterial;
enum EMeshRenderLayerType;

class GMesh :public GActorObject,public IRenderingInterface,public IDirectXDeviceInterfece
{
	CODEREFLECTION()

	//CVARIABLE()
	//CTransformationComponent* TransformationComponent;

	CVARIABLE()
	CMeshComponent* MeshComponent;
public:
	GMesh();

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);
public:
	virtual void SetPickup(bool bNewPickup);
public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetScale(const fvector_3d& InNewScale);
public:
	virtual CMeshComponent * GetMeshComponent() { return MeshComponent; }
	
	template<class T>
	T* GetMeshComponent()
	{
		return dynamic_cast<T*>(MeshComponent);
	}
public:
	virtual void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);

public:
	void SetCastShadow(bool bNewCastShadow);
	bool IsCastShadow()const;

protected:
	virtual void SetMeshComponent(CMeshComponent* InMeshComponent);

public:
	UINT GetMaterialNum()const;

	vector<CMaterial*>* GetMaterials();
};

