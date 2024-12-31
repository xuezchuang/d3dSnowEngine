#include "DirectXRenderingEngine.h"
#include "CoreObject/CoreMinimalObject.h"

#include "../../../../Debug/EngineDebug.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Rendering/Core/Rendering.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/CylinderMesh.h"
#include "../../../../Mesh/ConeMesh.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/CustomMesh.h"
#include "../../../../Mesh/PyramidMesh.h"
#include "../../../../Mesh/PipeMesh.h"
#include "../../../../Mesh/TorusMesh.h"
#include "../../../../Mesh/SkinnedMesh.h"
#include "../../../../Core/World.h"
#include "../../../../Mesh/Core/MeshManage.h"
#include "../../../../Mesh/Core/Material/Material.h"
#include "../../../../Core/World.h"
#include "../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../Manage/LightManage.h"
#include "../../../../Actor/Light/ParallelLight.h"
#include "../../../../Actor/Light/SpotLight.h"
#include "../../../../Actor/Light/PointLight.h"
#include "../../../../Actor/Sky/Fog.h"
#include "../../../../Actor/Sky/Sky.h"
#include "../../../../Core/Camera.h"
#include "../../../../Core/Construction/MacroConstruction.h"
#include "CoreMacro.h"
#include "Core/AssetAssistLibrary.h"

#if defined(_WIN32)
#include "../../../../Core/WinMainCommandParameters.h"

#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")

#if EDITOR_ENGINE
#include "../../../../../EditorEngine/SelectEditor/OperationHandle/MoveArrow.h"
#include "../../../../../EditorEngine/SelectEditor/OperationHandle/RotatorArrow.h"
#include "../../../../../EditorEngine/SelectEditor/OperationHandle/ScalingArrow.h"

extern GMoveArrow* MoveArrow;
extern GScalingArrow* ScalingArrow;
extern GRotatorArrow* RotatorArrow;
#endif
//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CDirectXRenderingEngine::CDirectXRenderingEngine()
	: M4XQualityLevels(1)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
	, DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	bTick = false;

	BUILD_OBJECT_PARAMETERS_BY_NO_COMPONENT(, this);
	MeshManage = CreateObject<CMeshManage>(Param,new CMeshManage());
	LightManage = CreateObject<CLightManage>(Param, new CLightManage());
}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	delete MeshManage;
	delete LightManage;
}

int CDirectXRenderingEngine::PreInit()
{
	Engine_Log("DirectXRenderingEngine pre initialization complete.");
	return 0;
}


int CDirectXRenderingEngine::Init()
{
	MeshManage->Init();
	Engine_Log("DirectXRenderingEngine initialization complete.");
	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	{
#if EDITOR_ENGINE
		
		if (GMoveArrow* InMoveArrow = World->CreateActorObject<GMoveArrow>())
		{
			InMoveArrow->CreateMesh();

			MoveArrow = InMoveArrow;
		}

		if (GScalingArrow* InScalingArrow = World->CreateActorObject<GScalingArrow>())
		{
			InScalingArrow->CreateMesh();

			ScalingArrow = InScalingArrow;
		}

		if (GRotatorArrow* InRotatorArrow = World->CreateActorObject<GRotatorArrow>())
		{
			InRotatorArrow->CreateMesh();

			RotatorArrow = InRotatorArrow;
		}
#endif
		//构建Mesh
		/*if (GMesh* BoxMesh = MeshManage->CreateBoxMesh(4.f, 3.f, 1.5f))
		{
			BoxMesh->SetPosition(XMFLOAT3(4,3,5));
			BoxMesh->SetRotation(fvector_3d(60.f, 1.f, 20.f));
		}*/

		//灯光生成
		if (GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
		{
			ParallelLight->SetPosition(XMFLOAT3(10.f, -10.f, 10.f));
			ParallelLight->SetRotation(fvector_3d(30.f, 0.f, 0.f));
			ParallelLight->SetScale(fvector_3d(1));
			ParallelLight->SetLightIntensity(fvector_3d(1.1f,1.1f,1.1f));
		}
		//点灯光生成
		//if (GPointLight* PointLight = World->CreateActorObject<GPointLight>())
		//{
		//	PointLight->SetPosition(XMFLOAT3(0.f, -6.f, 10.f));
		//	PointLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//	
		//	PointLight->SetLightIntensity(fvector_3d(0.9f));
		//	PointLight->SetEndAttenuation(190.f);
		//}

		//聚灯光生成
		//if (GSpotLight* SpotLight = World->CreateActorObject<GSpotLight>())
		//{
		//	SpotLight->SetPosition(XMFLOAT3(0.f, -3.f, -10.f));
		//	SpotLight->SetRotation(fvector_3d(5.f, 180.f, 0.f));
		//
		//	SpotLight->SetLightIntensity(fvector_3d(1.f));
		//	//SpotLight->SetStartAttenuation(1.f);
		//	SpotLight->SetEndAttenuation(130.f);
		//
		//	SpotLight->SetConicalInnerCorner(40.f);
		//	SpotLight->SetConicalOuterCorner(60.f);
		//}	

		//
		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(5.f, 5.f, 5.f);
			InBoxMesh->SetPosition(XMFLOAT3(0.0f, 2.5f, 0.0f));
			InBoxMesh->SetScale(fvector_3d(1));
			if(CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
				//	InMaterial->SetBaseColor(fvector_4d(0.5f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
				InMaterial->SetBaseColor("stone"); 
			}
		}

		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			InPlaneMesh->CreateMesh(20.0f, 30.0f, 60, 40);
			InPlaneMesh->SetPosition(XMFLOAT3(0.f, -0.01f, 0.f));
			InPlaneMesh->SetRotation(fvector_3d(0.f, 0.f, 0.0f));
			InPlaneMesh->GetMeshComponent()->SetTextureScale(XMFLOAT3(8.0f, 8.0f, 1.0f));
			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
				InMaterial->SetBaseColor("tile");
			}
		}

		goto FINIAL;

		////甜甜圈
		if (GTorusMesh* InTorusMesh = World->CreateActorObject<GTorusMesh>())
		{
			InTorusMesh->CreateMesh(6.f,2.f,40.f, 40.f);
			InTorusMesh->SetPosition(XMFLOAT3(-22.f, -8, 20.f));
			InTorusMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InTorusMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);

				InMaterial->SetBaseColor("Zhuan");
				InMaterial->SetNormal("Zhuan_NRM");
			}
		}

		//读取蒙皮网格模型
		//if (GSkinnedMesh* InSkinnedMesh = World->CreateActorObject<GSkinnedMesh>())
		//{
		//	std::string SkinnedPath = FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineContentPath() + "//TestAnim_123.fbx");
		//	InSkinnedMesh->CreateMesh(SkinnedPath);
		//	InSkinnedMesh->SetPosition(XMFLOAT3(-1.f, 10, 20.f));
		//}

		//三楞锥
		if (GPyramidMesh* InPyramidMesh = World->CreateActorObject<GPyramidMesh>())
		{
			InPyramidMesh->CreateMesh(EPyramidNumberSides::Pyramid_3, 1);
			InPyramidMesh->SetPosition(XMFLOAT3(-1.f, -8, 20.f));
			InPyramidMesh->SetRotation(fvector_3d(0.f,90.f,0.f));
			InPyramidMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InPyramidMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(4.f,0.f,0.f,1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//Pipe模型
		if (GPipeMesh* InPipeMesh = World->CreateActorObject<GPipeMesh>())
		{
			InPipeMesh->CreateMesh(3.f,3.f,6.f,1.f,20.f,20.f);
			InPipeMesh->SetPosition(XMFLOAT3(-9.f, -9, 20.f));
			InPipeMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InPipeMesh->GetMaterials())[0])
			{
				//InMaterial->SetBaseColor(fvector_4d(5.f));
				//InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);

				InMaterial->SetBaseColor("Zhuan");
				InMaterial->SetNormal("Zhuan_NRM");
			}
		}

		//锥形
		if (GConeMesh* InConeMesh = World->CreateActorObject<GConeMesh>())
		{
			InConeMesh->CreateMesh(2.f,3.f,20.f, 20.f);

			InConeMesh->SetPosition(XMFLOAT3(7.f, -11.f, 20.f));
			InConeMesh->SetScale(fvector_3d(1.f, 1.f, 1.f));
			if (CMaterial* InMaterial = (*InConeMesh->GetMaterials())[0])
			{
			//	InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(5.f, 5.f, 5.f);

			InBoxMesh->SetPosition(XMFLOAT3(22.f, -10.f, 20.f));
			InBoxMesh->SetScale(fvector_3d(1));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
			//	InMaterial->SetBaseColor(fvector_4d(0.5f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GCylinderMesh* InCylinderMesh = World->CreateActorObject<GCylinderMesh>())
		{
			InCylinderMesh->CreateMesh(2.f, 2.f, 5.f, 20.f, 20.f);

			InCylinderMesh->SetPosition(XMFLOAT3(14.f, -10.f, 20.f));
			InCylinderMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InCylinderMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Zhuan");
				InMaterial->SetNormal("Zhuan_NRM");

				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			InPlaneMesh->CreateMesh(4.f, 3.f, 20, 20);

			InPlaneMesh->SetPosition(XMFLOAT3(0.f, -12.f, 0.f));
			InPlaneMesh->SetScale(fvector_3d(50.f, 1.f, 50.f));
			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}

			InPlaneMesh->SetPickup(false);
		}

		//兰伯特
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f,2,0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//半兰伯特
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					221.f / 255.f,
					154.f / 255.f,
					255.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 2, 0.f));
			SphereMesh->SetScale(1.f);
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				//模拟黄铜
				InMaterial->SetBaseColor(fvector_4d(
					191.f / 255.f,
					173.f / 255.f,
					111.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Phong);
				InMaterial->SetSpecular(fvector_3d(1.f));

				InMaterial->SetFresnelF0(fvector_3d(0.08f));
				InMaterial->SetRoughness(0.4f);
			}
		}

		//blinn-phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					220.f / 255.f,
					223.f / 255.f,
					227.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::BinnPhong);
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.3f);

				InMaterial->SetFresnelF0(fvector_3d(0.1f));
				InMaterial->SetDynamicReflection(true);
			}
		}

		//菲尼尔
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Fresnel);

				InMaterial->SetRoughness(0.8f);
			}
		}

		//Wrap 模拟皮肤
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					234.f/255.f,
					154.f/255.f,
					139.f/255.f,1.f));

				InMaterial->SetMaterialType(EMaterialType::Wrap);
			}
		}
	
		//Minnaert
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.9f,
					0.9f,
					1.0f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Minnaert);

				InMaterial->SetRoughness(0.95f);
			}
		}

		//AnisotropyKajiyaKay
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Hair");
				InMaterial->SetMaterialType(EMaterialType::AnisotropyKajiyaKay);
			}
		}

		//OrenNayar
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.7f,
					0.7f,
					1.4f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::OrenNayar);

				InMaterial->SetRoughness(0.7f);//如果是0 就是兰伯特
			}
		}

		//
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.7f,
					0.7f,
					1.4f, 1.f));
				
				InMaterial->SetMaterialType(EMaterialType::Banded);
			}
		}

		//GradualBanded
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					247.f / 255.f,
					150.f / 255.f,
					85.f  / 255.f, 1.f));
				
				InMaterial->SetMaterialType(EMaterialType::GradualBanded);
			}
		}

		//最终Banded
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					213.f / 255.f,
					132.f / 255.f,
					234.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::FinalBanded);
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.6f);
			}
		}

		//back
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					2.f / 255.f,
					214.f / 255.f,
					17.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Back);
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.2f);
			}
		}

		//以线框显示
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));
			
			}
		}

		//以点显示
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::PointDisplay);
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));
			}
		}

		//以世界法线显示
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::WorldNormal);
			}
		}

		//以法线显示
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 24, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Normal);
			}
		}

		//显示BaseColor贴图1
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Wood");
				InMaterial->SetNormal("Wood_NRM");
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetRoughness(4.f);
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);
			}
		}

		//显示BaseColor贴图2
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("../RenZhaiEngine/Asset/Texture/MMOARPG.dds");
				InMaterial->SetBaseColor(fvector_4d(0.7f));
				InMaterial->SetNormal("MMOARPG_NRM");
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);
			}
		}

		////显示BaseColor贴图2
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);

			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetDynamicReflection(true);

				InMaterial->SetBaseColor("Texture'/Project/Texture/Earth.Earth'");
				InMaterial->SetBaseColor(fvector_4d(0.7f));
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetMaterialType(EMaterialType::BinnPhong);

				InMaterial->SetFresnelF0(fvector_3d(0.1f));
				InMaterial->SetRoughness(0.1f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(9.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetNormal("Wood2_Nor");
				//InMaterial->SetBaseColor("Wood2");
				InMaterial->SetMaterialType(EMaterialType::BinnPhong);
				InMaterial->SetRoughness(0.8f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetNormal("Wood2_Nor");
				//InMaterial->SetBaseColor("Wood2");
				InMaterial->SetSpecular("Wood2_SPEC");
				InMaterial->SetMaterialType(EMaterialType::BinnPhong);
				InMaterial->SetRoughness(0.76f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//透明
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_TRANSPARENT);

			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 7, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("TransparentContent");
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//反射球
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 12, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetDynamicReflection(true);
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Phong);

				InMaterial->SetRoughness(0.01f);
				InMaterial->SetFresnelF0(fvector_3d(0.5f));
			}
		}

		//FBX
		if (GCustomMesh* CustomMesh = World->CreateActorObject<GCustomMesh>())
		{
			//
			string MeshPath = FEnginePathHelper::GetEngineContentPath() + "/SK_Mannequin.FBX";
			CustomMesh->CreateMesh(MeshPath);
			CustomMesh->SetPosition(XMFLOAT3(0.f, 0, 50.f));
			CustomMesh->SetRotation(fvector_3d(90.f, 0.f, 0.f));
			CustomMesh->SetCastShadow(true);

			if (CMaterial* InMaterial = (*CustomMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GCustomMesh* CustomMesh = World->CreateActorObject<GCustomMesh>())
		{
			string Path = FEnginePathHelper::GetEngineContentPath() + "\\TestC_1.fbx";
			CustomMesh->CreateMesh(Path);
		
			CustomMesh->SetPosition(XMFLOAT3(0.f, -6, 50.f));
			if (CMaterial* InMaterial = (*CustomMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
		
				InMaterial->SetRoughness(0.01f);
				InMaterial->SetFresnelF0(fvector_3d(0.5f));
			}
		}

		//if (GCustomMesh* CustomMesh = World->CreateActorObject<GCustomMesh>())//反射球
		//{
		//	string Path = "../RenZhaiEngine/Asset/man.obj";
		//	CustomMesh->CreateMesh(Path);
		//
		//	CustomMesh->SetPosition(XMFLOAT3(0.f, -12, 30.f));
		//	CustomMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//	if (CMaterial* InMaterial = (*CustomMesh->GetMaterials())[0])
		//	{
		//		InMaterial->SetBaseColor(fvector_4d(1.f));
		//		InMaterial->SetMaterialType(EMaterialType::HalfLambert);
		//
		//		InMaterial->SetRoughness(0.01f);
		//		InMaterial->SetFresnelF0(fvector_3d(0.5f));
		//	}
		//}
		if (CClassObject* InObjectClass = AssetAssistLibrary::FindClass("Actor123"))
		{
			if (GActorObject* InActorObject = World->CreateActorObject<GActorObject>(InObjectClass))//透明的珠子
			{

			}

		}


		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//透明的珠子
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(0.f, 9, 10.f));
			//SphereMesh->SetPosition(XMFLOAT3(15.f, 18, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.f));
				InMaterial->SetMaterialType(EMaterialType::Transparency);

				InMaterial->SetRoughness(0.01f);
				InMaterial->SetFresnelF0(fvector_3d(0.5f));
				InMaterial->SetTransparency(1.f);
				InMaterial->SetDynamicReflection(true);

				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRefractiveValue(1.11f);
			}
		}

		//PBR模型组
		{
			//自由设定
			if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//PBR模型
			{
				SphereMesh->CreateMesh(2.f, 30, 30);
				SphereMesh->SetPosition(XMFLOAT3(15.f, 2, 0.f));
				SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
				if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
				{
					InMaterial->SetBaseColor(fvector_4d(1.f));
					InMaterial->SetMaterialType(EMaterialType::PBR);
				}
			}

			int Row = 6;
			int Colum = 6;
			for (int i = 0; i < Colum; i++)
			{
				for (int j = 0; j < Row; j++)
				{
					fvector_3d PBRPosition(25.f, -3.f, 5.f);
					PBRPosition.y += i * 5.f;
					PBRPosition.z += j * 5.f;

					if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//PBR模型
					{
						SphereMesh->CreateMesh(2.f, 30, 30);
						SphereMesh->SetPosition(XMFLOAT3(PBRPosition.x, PBRPosition.y, PBRPosition.z));
						SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
						if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
						{
							InMaterial->SetBaseColor(fvector_4d(1.f));
							InMaterial->SetMaterialType(EMaterialType::PBR);

							InMaterial->SetRoughness(((float)j + 1.f)   / (float)Row);
							InMaterial->SetMetallicity(((float)i + 1.f) / (float)Colum);
						}
					}
				}
			}
		}

		////well
		//if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		//{
		//	InBoxMesh->CreateMesh(30.f, 150, 0.4f);
		//
		//	InBoxMesh->SetPosition(XMFLOAT3(0.f, 0.f, -60.f));
		//	if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
		//	{
		//		InMaterial->SetBaseColor(fvector_4d(1.f));
		//		InMaterial->SetMaterialType(EMaterialType::Lambert);
		//	}
		//}

		//well
		//if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		//{
		//	InBoxMesh->CreateMesh(30.f, 150, 0.4f);
		//
		//	InBoxMesh->SetPosition(XMFLOAT3(0.f, 0.f, 70.f));
		//	if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
		//	{
		//		InMaterial->SetBaseColor(fvector_4d(1.f));
		//		InMaterial->SetMaterialType(EMaterialType::Lambert);
		//	}
		//}

		//well
		//if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		//{
		//	InBoxMesh->CreateMesh(30.f, 20.f, 150.f);
		//
		//	InBoxMesh->SetPosition(XMFLOAT3(70.f, 0.f, 0.f));
		//	if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
		//	{
		//		InMaterial->SetBaseColor(fvector_4d(1.f));
		//		InMaterial->SetMaterialType(EMaterialType::Lambert);
		//	}
		//}
		
		////Top Well
		//if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		//{
		//	InBoxMesh->CreateMesh(20.f, 150, 150.f);
		//
		//	InBoxMesh->SetPosition(XMFLOAT3(0.f, 20.f, 0.f));
		//	if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
		//	{
		//		InMaterial->SetBaseColor(fvector_4d(1.f));
		//		InMaterial->SetMaterialType(EMaterialType::Lambert);
		//	}
		//}

		if (GSky* InSky = World->CreateActorObject<GSky>())//天空
		{
			InSky->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
		}

		//雾的实例
		if (GFog* Fog = World->CreateActorObject<GFog>())
		{
			Fog->SetFogColor(fvector_color(0.7f, 0.7f, 0.9f,1.f));
			Fog->SetFogStart(10.f);
			Fog->SetFogRange(500.f);
		
			Fog->SetFogHeight(5000.f);
			Fog->SetFogTransparentCoefficient(0.00f);
		
		}

		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			InPlaneMesh->CreateMesh(7.f,7.f,2,2);
			InPlaneMesh->SetPosition(XMFLOAT3(0.f,0.f,40.f));
			InPlaneMesh->SetRotation(fvector_3d(90.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::ShadowTexture);
			}
		}

		/*if (GMesh* CylinderMesh = MeshManage->CreateCylinderMesh(1.f, 1.f, 5.f, 20, 20))
		{
			CylinderMesh->SetPosition(XMFLOAT3(1, -2, -4));
		}

		if (GMesh* ConeMesh = MeshManage->CreateConeMesh(1.f, 5.f, 20, 20))
		{
			ConeMesh->SetPosition(XMFLOAT3(-1, 1, 9));
			ConeMesh->SetRotation(fvector_3d(90.f, 1.f, 20.f));
		}*/
	}
	FINIAL:
	MeshManage->BuildMesh();

	//ANALYSIS_HRESULT(GraphicsCommandList->Close());

	return 0;
}

void CDirectXRenderingEngine::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	MeshManage->UpdateCalculations(DeltaTime, ViewportInfo);
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	//重置录制相关的内存，为下一帧做准备

	MeshManage->PreDraw(DeltaTime);

	StartSetMainViewportRenderTarget();

	MeshManage->Draw(DeltaTime);
	//MeshManage->PostDraw(DeltaTime);

	EndSetMainViewportRenderTarget();

	////录入完成
	//ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//////提交命令
	//ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	//CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	////交换两个buff缓冲区
	//ANALYSIS_HRESULT(SwapChain->Present(1, 0));
	////CurrentSwapBuffIndex = (CurrentSwapBuffIndex + 1) % FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	//CurrentSwapBuffIndex = !(bool)CurrentSwapBuffIndex;

	////CPU等GPU
	//WaitGPUCommandQueueComplete();
}

void CDirectXRenderingEngine::OnResetSize(int InWidth, int InHeight, int wParam)
{
	//if (D3dDevice)
	//{
	//	//同步
	//	WaitGPUCommandQueueComplete();

	//	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	//	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	//	{
	//		SwapChainBuffer[i].Reset();
	//	}
	//	DepthStencilBuffer.Reset();

	//	//自适应屏幕变大
	//	SwapChain->ResizeBuffers(
	//		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
	//		InWidth,
	//		InHeight,
	//		BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//	CurrentSwapBuffIndex = 0;

	//	//拿到描述size
	//	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	//	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	//	{
	//		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
	//		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
	//		HeapHandle.Offset(1, RTVDescriptorSize);
	//	}

	//	D3D12_RESOURCE_DESC ResourceDesc;
	//	ResourceDesc.Width = InWidth;
	//	ResourceDesc.Height = InHeight;
	//	ResourceDesc.Alignment = 0;
	//	ResourceDesc.MipLevels = 1;
	//	ResourceDesc.DepthOrArraySize = 1;
	//	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	//	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	//	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	//	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	//	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//	D3D12_CLEAR_VALUE ClearValue;
	//	ClearValue.DepthStencil.Depth = 1.f;
	//	ClearValue.DepthStencil.Stencil = 0;
	//	ClearValue.Format = DepthStencilFormat;

	//	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//	D3dDevice->CreateCommittedResource(
	//		&Properties,
	//		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
	//		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
	//		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	//	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	//	DSVDesc.Format = DepthStencilFormat;
	//	DSVDesc.Texture2D.MipSlice = 0;
	//	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
	//	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	//	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
	//		D3D12_RESOURCE_STATE_COMMON,
	//		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	//	GraphicsCommandList->ResourceBarrier(1, &Barrier);

	//	GraphicsCommandList->Close();

	//	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	//	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//	WaitGPUCommandQueueComplete();

	//	//摄像机自适应
	//	if (World &&
	//		World->GetCamera())
	//	{
	//		World->GetCamera()->OnResetSize(InWidth, InHeight);
	//	}

	//	//模型自适应
	//	//MeshManage->OnResetSize(InWidth, InHeight);
	//}
}

int CDirectXRenderingEngine::PreExit()
{
	
	Engine_Log("Engine post exit complete.");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{

	Engine_Log("Engine exit complete.");
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	//WaitGPUCommandQueueComplete();
	//ImGui_ImplDX12_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

//#if defined(_DEBUG)
//	// 假设你已经创建了ID3D12Device对象d3dDevice
//	IDXGIDebug1* dxgiDebug;
//	if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
//	{
//		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
//		dxgiDebug->Release();
//	}
//#endif


	Engine_Log("Engine post exit complete.");
	return 0;
}

void CDirectXRenderingEngine::StartSetMainViewportRenderTarget()
{
	////指向哪个资源 转换其状态
	//CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
	//	GetCurrentSwapBuff(),D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	//ClearMainSwapChainCanvas();
	////需要每帧执行
	////绑定矩形框
	//GraphicsCommandList->RSSetViewports(1, &World->GetCamera()->ViewprotInfo);
	//GraphicsCommandList->RSSetScissorRects(1, &World->GetCamera()->ViewprotRect);

	////输出的合并阶段
	//D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	//D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	//GraphicsCommandList->OMSetRenderTargets(1, &SwapBufferView,
	//	true, &DepthStencilView);
}

void CDirectXRenderingEngine::EndSetMainViewportRenderTarget()
{
	//CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
	//	D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
}

void CDirectXRenderingEngine::ClearMainSwapChainCanvas()
{
	////清除画布
	//GraphicsCommandList->ClearRenderTargetView(GetCurrentSwapBufferView(),
	//	DirectX::Colors::Black,
	//	0, nullptr);

	////清除深度模板缓冲区
	//GraphicsCommandList->ClearDepthStencilView(GetCurrentDepthStencilView(),
	//	D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
	//	1.f, 0, 0, NULL);
}


UINT CDirectXRenderingEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;;
}

UINT CDirectXRenderingEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
}

bool CDirectXRenderingEngine::InitDirect3D()
{
	return true;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
}

#endif