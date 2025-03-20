#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Component/Sky/FogComponent.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../RenderingTextureResourcesUpdate.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Component/Sky/SkyConstantBuffer.h"
#include "../../../../../Component/Mesh/SkinnedMeshComponent.h"
#include "../../../../../Mesh/Core/SkinnedTransformation.h"
#include "../RootSignature/DirectXRootSignatureType.h"

UINT MeshObjectCount = 0;

map<size_t, std::shared_ptr<FRenderingData>> FGeometry::UniqueRenderingDatas;
vector<std::shared_ptr<FRenderingData>> FGeometry::RenderingDatas;

FGeometryMap::FGeometryMap()
{
	for (int i = 0; i < ERenderingMeshType::MAX_TYPE; i++)
	{
		Geometrys.insert(pair<int, FGeometry>(i, FGeometry()));
	}

	RenderingTexture2DResources = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingTexture2DResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);

	RenderingCubeMapResources = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingCubeMapResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);

	Fog = NULL;
}

FGeometryMap::~FGeometryMap()
{
	MeshObjectCount = 0;
}

void FGeometryMap::SetDescriptorHeaps()
{
	DescriptorHeap.SetDescriptorHeaps();
}

void FGeometryMap::Draw(GraphicsContext& gfxContext, float DeltaTime)
{
	//��Ⱦ�ӿ�
	{
		gfxContext.SetDynamicConstantBufferView(Signature_Viewport, sizeof(FViewportTransformation), &ViewportTransformation);
	}

	//���Ƶƹ�
	DrawLight(DeltaTime);

	//������ͼ
	{
		gfxContext.SetDynamicDescriptors(Signature_Texture2D, 0, GetDrawTexture2DResourcesNumber(), m_TexturesDesHandle.get());
	}

	//������������ͼ
	DrawCubeMapTexture(DeltaTime);

	//���Ʋ���
	{
		gfxContext.SetDynamicSRV(Signature_Material, m_MaterialConstantBufferViews.size() * sizeof(FMaterialConstantBuffer), m_MaterialConstantBufferViews.data());
	}

	//��������
	DrawFog(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::OnResetSize(int InWidth, int InHeight)
{
	DynamicShadowCubeMap.OnResetSize(InWidth, InHeight);
}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);

	//���µ�
	UpdateLight(DeltaTime, ViewportInfo);

	//�����ӿ�
	UpdateCalculationsViewport(DeltaTime, ViewportInfo, 0);

	UpdateFog(DeltaTime, ViewportInfo);

	//�����ӿ�
	DynamicShadowMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FGeometryMap::UpdateCalculationsViewport(float DeltaTime, const FViewportInfo& ViewportInfo, UINT InConstantBufferOffset)
{
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);

	XMMATRIX HalfLambert(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX TexViewProjectionMatrix = XMMatrixMultiply(ViewProject, HalfLambert);

	
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));
	XMStoreFloat4x4(&ViewportTransformation.TexViewProjectionMatrix, XMMatrixTranspose(TexViewProjectionMatrix));

	//�õ��ӿ�λ��
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;
}

void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (m_MaterialConstantBufferViews.size() != Materials.size())
		m_MaterialConstantBufferViews.resize(Materials.size());

	for (size_t i = 0; i < Materials.size(); i++)
	{
		//�任����
		if (CMaterial* InMaterial = Materials[i])
		{
			if (InMaterial->IsDirty())
			{
				auto& MaterialConstantBuffer = m_MaterialConstantBufferViews[i];
				//BaseColor
				MaterialConstantBuffer.BaseColor = EngineMath::ToFloat4(InMaterial->GetBaseColor());

				//�߹���ɫ
				MaterialConstantBuffer.SpecularColor = EngineMath::ToFloat3(InMaterial->GetSpecularColor());

				//�ֲڶ�
				MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

				//������
				MaterialConstantBuffer.Refraction = InMaterial->GetRefractiveValue();

				//��������
				MaterialConstantBuffer.MaterialType = (int)InMaterial->GetMaterialType();

				//F0����
				fvector_3d F0 = InMaterial->GetFresnelF0();
				MaterialConstantBuffer.FresnelF0 = XMFLOAT3(F0.x, F0.y, F0.z);

				//͸����
				MaterialConstantBuffer.Transparency = InMaterial->GetTransparency();

				//������
				MaterialConstantBuffer.Metallicity = EngineMath::ToFloat3(InMaterial->GetMetallicity());

				//�ⲿ��Դ����
				{
					//�����BaseColor
					if (auto BaseColorTextureResourcesPtr = FindRenderingTexture(InMaterial->GetBaseColorIndexKey()))
					{
						MaterialConstantBuffer.BaseColorIndex = (*BaseColorTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.BaseColorIndex = -1;
					}

					//����
					if (auto NormalTextureResourcesPtr = FindRenderingTexture(InMaterial->GetNormalIndexKey()))
					{
						MaterialConstantBuffer.NormalIndex = (*NormalTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.NormalIndex = -1;
					}


					//�߹�
					if (auto SpecularTextureResourcesPtr = FindRenderingTexture(InMaterial->GetSpecularKey()))
					{
						MaterialConstantBuffer.SpecularIndex = (*SpecularTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.SpecularIndex = -1;
					}
				}

				//���ʾ���
				XMMATRIX MaterialTransform = XMLoadFloat4x4(&InMaterial->GetMaterialTransform());
				XMStoreFloat4x4(&MaterialConstantBuffer.TransformInformation,
					XMMatrixTranspose(MaterialTransform));

				InMaterial->SetDirty(false);

				//�Զ�����
				//float
				MaterialConstantBuffer.Param0 = InMaterial->GetFloatParam(0);
				MaterialConstantBuffer.Param1 = InMaterial->GetFloatParam(1);
				MaterialConstantBuffer.Param2 = InMaterial->GetFloatParam(2);

				//MaterialConstantBufferViews.Update(InMaterial->GetMaterialIndex(), &MaterialConstantBuffer);
			}
		}
	}
}

void FGeometryMap::UpdateLight(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	////���µƹ�
	//FLightConstantBuffer LightConstantBuffer;
	//for (size_t i = 0; i < GetLightManage()->Lights.size(); i++)
	//{
	//	if (CLightComponent* InLightComponent = GetLightManage()->Lights[i])
	//	{
	//		fvector_3d LightIntensity = InLightComponent->GetLightIntensity();
	//		LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
	//		LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetForwardVector();

	//		LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
	//		LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();

	//		switch (InLightComponent->GetLightType())
	//		{
	//		case ELightType::DirectionalLight:
	//		{
	//			XMFLOAT3 ForwardVector = InLightComponent->GetForwardVector();

	//			DynamicShadowMap.BuildParallelLightMatrix(
	//				EngineMath::ToVector3d(ForwardVector), fvector_3d(0.f), 100.f);

	//			XMFLOAT4X4 ShadowViewMatrix;
	//			XMFLOAT4X4 ShadowProjectMatrix;
	//			DynamicShadowMap.GetViewportMatrix(ShadowViewMatrix, ShadowProjectMatrix);

	//			XMMATRIX ShadowViewMatrixRTX = XMLoadFloat4x4(&ShadowViewMatrix);
	//			XMMATRIX ShadowProjectMatrixRTX = XMLoadFloat4x4(&ShadowProjectMatrix);

	//			//NDC [-1,1]; = >[0,1]
	//			//��������˼��
	//			XMMATRIX Transform =
	//			{
	//				0.5f, 0.0f, 0.0f, 0.0f,
	//				0.0f, -0.5f, 0.0f, 0.0f,
	//				0.0f, 0.0f, 1.0f, 0.0f,
	//				0.5f, 0.5f, 0.0f, 1.0f
	//			};

	//			XMMATRIX ShadowViewProjectMatrixRTX =
	//				ShadowViewMatrixRTX * ShadowProjectMatrixRTX * Transform;

	//			//�洢Shadow�任��Ϣ
	//			XMStoreFloat4x4(&LightConstantBuffer.SceneLights[i].ShadowTransform, XMMatrixTranspose(ShadowViewProjectMatrixRTX));

	//			break;
	//		}
	//		case ELightType::PointLight:
	//		{
	//			if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))
	//			{
	//				LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
	//				LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
	//			}

	//			break;
	//		}
	//		case ELightType::SpotLight:
	//		{
	//			if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))
	//			{
	//				LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
	//				LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
	//			}

	//			if (InLightComponent->GetLightType() == ELightType::SpotLight)
	//			{
	//				if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))
	//				{
	//					LightConstantBuffer.SceneLights[i].ConicalInnerCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalInnerCorner());
	//					LightConstantBuffer.SceneLights[i].ConicalOuterCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalOuterCorner());
	//				}
	//			}

	//			XMFLOAT3 ForwardVector = InLightComponent->GetForwardVector();
	//			XMFLOAT3 Position = InLightComponent->GetPosition();

	//			DynamicShadowMap.BuildSpotLightMatrix(
	//				EngineMath::ToVector3d(ForwardVector),
	//				EngineMath::ToVector3d(Position),
	//				370.f);

	//			XMFLOAT4X4 ShadowViewMatrix;
	//			XMFLOAT4X4 ShadowProjectMatrix;
	//			DynamicShadowMap.GetViewportMatrix(ShadowViewMatrix, ShadowProjectMatrix);

	//			XMMATRIX ShadowViewMatrixRTX = XMLoadFloat4x4(&ShadowViewMatrix);
	//			XMMATRIX ShadowProjectMatrixRTX = XMLoadFloat4x4(&ShadowProjectMatrix);

	//			//NDC [-1,1]; = >[0,1]
	//			//��������˼��
	//			XMMATRIX Transform =
	//			{
	//				0.5f, 0.0f, 0.0f, 0.0f,
	//				0.0f, -0.5f, 0.0f, 0.0f,
	//				0.0f, 0.0f, 1.0f, 0.0f,
	//				0.5f, 0.5f, 0.0f, 1.0f
	//			};

	//			XMMATRIX ShadowViewProjectMatrixRTX =
	//				ShadowViewMatrixRTX * ShadowProjectMatrixRTX * Transform;

	//			//�洢Shadow�任��Ϣ
	//			XMStoreFloat4x4(&LightConstantBuffer.SceneLights[i].ShadowTransform, XMMatrixTranspose(ShadowViewProjectMatrixRTX));

	//			break;
	//		}
	//		}
	//	}
	//}

	//LightConstantBufferViews.Update(0, &LightConstantBuffer);
}

void FGeometryMap::UpdateFog(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//������
	if (Fog)
	{
		if (Fog->IsDirty())
		{
			FFogConstantBuffer FogConstantBuffer;
			{
				fvector_color FogColor = Fog->GetFogColor();
				FogConstantBuffer.FogColor = XMFLOAT3(FogColor.r, FogColor.g, FogColor.b);

				FogConstantBuffer.FogStart = Fog->GetFogStart();
				FogConstantBuffer.FogRange = Fog->GetFogRange();
				FogConstantBuffer.FogHeight = Fog->GetFogHeight();
				FogConstantBuffer.FogTransparentCoefficient = Fog->GetFogTransparentCoefficient();
			}

			//FogConstantBufferViews.Update(0, &FogConstantBuffer);

			Fog->SetDirty(false);
		}
	}
}

void FGeometryMap::UpdateSkinned(float DeltaTime, CComponent* InSkinComponent)
{
	if (CSkinnedMeshComponent* InSkinnedMeshComponent = dynamic_cast<CSkinnedMeshComponent*>(InSkinComponent))
	{
		FSkinnedTransformation SkinnedTransformation;
		InSkinnedMeshComponent->TickAnimation(DeltaTime, SkinnedTransformation);

		//SkinnedConstantBufferViews.Update(0,&SkinnedTransformation);
	}
}

void FGeometryMap::BuildFog()
{
	for (auto& Tmp : GObjects)
	{
		if (CFogComponent* InFogComponent = dynamic_cast<CFogComponent*>(Tmp))
		{
			Fog = InFogComponent;
			break;
		}
	}
}

void FGeometryMap::BuildShadow()
{
	DynamicShadowMap.Init(2048, 2048);
	DynamicShadowMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));
	DynamicShadowMap.BuildDepthStencilDescriptor();
	DynamicShadowMap.BuildRenderTargetDescriptor();

	DynamicShadowCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));
	DynamicShadowCubeMap.BuildDepthStencilDescriptor();
	DynamicShadowCubeMap.BuildRenderTargetDescriptor();
	DynamicShadowCubeMap.BuildDepthStencil();
}

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& MeshData)
{
	int MeshIndex = (int)InMesh->GetMeshType();
	Geometrys[MeshIndex].BuildMesh(InMeshHash, InMesh, MeshData, MeshIndex);
}

void FGeometryMap::BuildDynamicReflectionMesh()
{
	for (auto& Tmp : GObjects)
	{
		if (CMeshComponent* InMeshComponent = dynamic_cast<CMeshComponent*>(Tmp))
		{
			if (InMeshComponent->IsDynamicReflection())
			{
				DynamicReflectionMeshComponents.push_back(InMeshComponent);
			}
		}
	}
}

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	int MeshIndex = (int)InMesh->GetMeshType();
	Geometrys[MeshIndex].BuildMesh(InMeshHash, InMesh, MeshData, MeshIndex);
}

void FGeometryMap::DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData)
{
	int MeshIndex = (int)InMesh->GetMeshType();
	Geometrys[MeshIndex].DuplicateMesh(InMesh, MeshData, MeshIndex);
}

bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	for (auto& Tmp : Geometrys)
	{
		if (Tmp.second.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex))
		{
			return true;
		}
	}

	return false;
}

void FGeometryMap::LoadTexture()
{
	def_c_paths_v2 Paths;
	init_def_c_paths_v2(&Paths);

	std::string ContentPath = 
		FEnginePathHelper::RelativeToAbsolutePath(
		FEnginePathHelper::GetEngineContentPath());

	Engine_Log("Load texture root path: [%s]", ContentPath.c_str());

	find_files_v2(ContentPath.c_str(), &Paths, true, true);

	Engine_Log("Load texture num: [%i]", Paths.index);

	int offset = 0;
	for (int i = 0; i < Paths.num; i++)
	{
		char* TmpPath = get_def_c_paths_by_offset(&Paths, offset);
		offset += get_def_c_offset(TmpPath);

		//......./filename_cubemap.
		if (find_string(TmpPath, ".dds", 0) != -1)
		{
#ifdef _WIN64
#else
			//��λ��·��
			normalization_path(TmpPath);
#endif 
			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, TmpPath);

			if (wfind_string(TexturePath, L"_CubeMap.") != -1 ||
				wfind_string(TexturePath, L"_cubemap.") != -1)
			{
				//CubeMap
				m_Textures.push_back(TextureRef());
				TextureRef& CubeMapResources = m_Textures[m_Textures.size() - 1];
				CubeMapResources = TextureManager::LoadDDSFromFile(TexturePath);
			}
			else
			{
				//Texture2D
				m_Textures.push_back(TextureRef());
				TextureRef& Texture2DResources = m_Textures[m_Textures.size() - 1];
				Texture2DResources = TextureManager::LoadDDSFromFile(TexturePath);
				//Texture2DResources.LoadFromFile(TexturePath);
			}
		}

		Engine_Log("texture fullname: [%s]", TmpPath);
	}

	m_TexturesDesHandle = std::make_unique<D3D12_CPU_DESCRIPTOR_HANDLE[]>(m_Textures.size());
	for (size_t i = 0; i < m_Textures.size(); i++)
	{
		m_TexturesDesHandle[i] = m_Textures[i].GetSRV();
	}
	

	destroy_def_c_paths_v2(&Paths);
}

void FGeometryMap::Build()
{
	//����ģ��
	for (auto& Tmp : Geometrys)
	{
		Tmp.second.Build(Tmp.first);
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1�����
	DescriptorHeap.Build(
		GetDrawTexture2DResourcesNumber() + //Texture2D
		GetDrawCubeMapResourcesNumber() + //��̬Cube��ͼ ���� �����
		1 + //��̬Cube��ͼ ����
		1 + //Shadow ֱ��� �۹�� Shadow
		1 + //ShadowCubeMap ���Դ�� Shadow
		1 + //UI
		1 + //����
		1 + //���
		1 + //Noiseͼ
		1 + //SSAO
		1); //˫��ģ��
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	MeshObjectConstant.clear();
	MeshObjectConstant.resize(GetDrawMeshObjectNumber());
	//��������������
	//MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectNumber());

	////Handle
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////��������������
	//MeshConstantBufferViews.BuildConstantBuffer(DesHandle,GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildFogConstantBuffer()
{
	//��������������
	FogConstantBufferViews.CreateConstant(sizeof(FFogConstantBuffer), 1);
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
	//�ռ�����
	//�������Shader-Index
	for (auto& InData : FGeometry::RenderingDatas)
	{
		if (auto InMaterials = InData->Mesh->GetMaterials())
		{
			for (size_t j = 0; j < InMaterials->size(); j++)
			{
				//��ShaderIndex����
				(*InMaterials)[j]->SetMaterialIndex(Materials.size());

				Materials.push_back((*InMaterials)[j]);
			}
		}
	}

	//��������������
	//MaterialConstantBufferViews.CreateConstant(
	//	sizeof(FMaterialConstantBuffer),
	//	GetDrawMaterialObjectNumber(),
	//	false);
}

void FGeometryMap::BuildLightConstantBuffer()
{
	//��������������
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

	////Handle
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////��������������
	//LightConstantBufferViews.BuildConstantBuffer(
	//	DesHandle,
	//	GetDrawLightObjectNumber(),
	//	GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildSkinnedMeshConstantBuffer()
{
	SkinnedConstantBufferViews.CreateConstant(sizeof(FSkinnedTransformation), 1);
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
	return Geometrys[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
	return Materials.size();
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
	return 1;
}

UINT FGeometryMap::GetDrawTexture2DResourcesNumber()
{
	return m_Textures.size();
	//return RenderingTexture2DResources->Size();
}

UINT FGeometryMap::GetDrawCubeMapResourcesNumber()
{
	return RenderingCubeMapResources->Size();
}

UINT FGeometryMap::GetDynamicReflectionViewportNum()
{
	return DynamicReflectionMeshComponents.size() * 6;
}

void FGeometryMap::BuildTextureConstantBuffer()
{
	//����Texture2D
	RenderingTexture2DResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(), 0);//�ӿ�

	//����CubeMap
	RenderingCubeMapResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(),
		GetDrawTexture2DResourcesNumber());//�ӿ�
}


UINT FGeometryMap::GetDynamicReflectionMeshComponentsSize()
{
	return DynamicReflectionMeshComponents.size();
}

CMeshComponent* FGeometryMap::GetDynamicReflectionMeshComponents(int Index)
{
	return DynamicReflectionMeshComponents[Index];
}


bool FGeometryMap::IsStartUPFog()
{
	return Fog != NULL;
}

std::unique_ptr<FRenderingTexture>* FGeometryMap::FindRenderingTexture(const std::string& InKey)
{
	if (auto RenderingTexture2DPtr = RenderingTexture2DResources->FindRenderingTexture(InKey))
	{
		return RenderingTexture2DPtr;
	}
	else if (auto RenderingCubeMapPtr = RenderingCubeMapResources->FindRenderingTexture(InKey))
	{
		return RenderingCubeMapPtr;
	}

	return nullptr;
}

void FGeometryMap::DrawShadow(float DeltaTime)
{
	DynamicShadowMap.Draw(DeltaTime);
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//
	//auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	//DesHandle.Offset(
	//	GetDrawMeshObjectNumber(), DescriptorOffset);
	//
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);

	//GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
	//	Signature_Light,
	//	LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}


void FGeometryMap::DrawMesh(float DeltaTime)
{

}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime)
{
//	//UINT DescriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
//
//	//auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
//	//DesHandle.Offset(GetDrawTexture2DResourcesNumber(), DescriptorOffset);
//
//	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(Signature_CubeMapTexture, DesHandle);
}

void FGeometryMap::DrawFog(float DeltaTime)
{
	//GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
	//	Signature_Fog,
	//	FogConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawSkinned(float DeltaTime, int InSkinnedIndex)
{
	//int ConstantBufferByteSize = SkinnedConstantBufferViews.GetConstantBufferByteSize();
	//
	//D3D12_GPU_VIRTUAL_ADDRESS SkinnedAddress = 
	//	SkinnedConstantBufferViews.GetBuffer()->GetGPUVirtualAddress() +
	//	ConstantBufferByteSize * InSkinnedIndex;

	//GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(Signature_ObjectSkinned, SkinnedAddress);
}

void FGeometryMap::DrawEmptySkinned(float DeltaTime)
{
	//GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(Signature_ObjectSkinned,NULL);
}

bool FGeometry::IsRenderingDataExistence(CMeshComponent* InKey)
{
	//if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InKey->GetRenderLayerType()))
	//{
	//	for (auto& Tmp : InRenderLayer->RenderDatas)
	//	{
	//		if (!Tmp.expired())
	//		{
	//			if (Tmp.lock()->Mesh == InKey)
	//			{
	//				return true;
	//			}
	//		}

	//	}
	//}

	return false;
}

template<class T>
void FGeometry::DuplicateMesh_Interior(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData, FMeshData<T>& InMyRenderingData, int InKey)
{
	if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
	{
		RenderingDatas.push_back(std::make_shared<FRenderingData>());
		std::shared_ptr<FRenderingData> InRenderingData = RenderingDatas[RenderingDatas.size() - 1];

		InRenderLayer->RenderDatas.push_back(InRenderingData);

		//����ע��
		InRenderingData->VertexTotalxSize = MeshData->VertexTotalxSize;
		InRenderingData->IndexTotalSize = MeshData->IndexTotalSize;
		InRenderingData->Mesh = InMesh;
		InRenderingData->GeometryKey = InKey;
		InRenderingData->MeshHash = MeshData->MeshHash;
		InRenderingData->VertexOffsetPosition = MeshData->VertexOffsetPosition;
		InRenderingData->MeshRenderingData = &InMyRenderingData;
		InRenderingData->Bounds = MeshData->Bounds;
		InRenderingData->VertexTypeSize = MeshData->VertexTypeSize;

		DuplicateMeshRenderingSection(MeshData, InRenderingData);
	}
}

template<class T>
void FGeometry::BuildMesh_Interior(
	const size_t InMeshHash,
	CMeshComponent* InMesh,
	const FRenderContent<T>& MeshData,
	int InKey,
	FMeshData<T>& InMeshRenderingData, 
	std::function<void(std::shared_ptr<FRenderingData>)> InFun)
{
	//�ҵ���Ӧ�㼶
	if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
	{
		UniqueRenderingDatas.insert(std::make_pair(InMeshHash, std::make_shared<FRenderingData>()));
		RenderingDatas.push_back(std::make_shared<FRenderingData>());
		std::shared_ptr<FRenderingData> InRenderingData = RenderingDatas[RenderingDatas.size() - 1];

		InRenderLayer->Add(InRenderingData);

		//����AABB
		BuildBoundingBox(MeshData, InRenderingData->Bounds);
		
		//������Ⱦ����ע��
		InRenderingData->Mesh = InMesh;
		InRenderingData->MeshHash = InMeshHash;
		InRenderingData->GeometryKey = InKey;
		InRenderingData->MeshRenderingData = &InMeshRenderingData;
		InRenderingData->VertexOffsetPosition = InMeshRenderingData.VertexData.size();
		InRenderingData->VertexTypeSize = sizeof(T);//��������ʹ�С

		//����Section
		BuildRenderingSection(MeshData, InRenderingData, InMeshRenderingData);

		//Ψһ���ݵ�ע��
		UniqueRenderingDatas[InMeshHash]->Mesh = InRenderingData->Mesh;
		UniqueRenderingDatas[InMeshHash]->MeshHash = InRenderingData->MeshHash;
		UniqueRenderingDatas[InMeshHash]->GeometryKey = InRenderingData->GeometryKey;
		UniqueRenderingDatas[InMeshHash]->Bounds = InRenderingData->Bounds;
		UniqueRenderingDatas[InMeshHash]->VertexOffsetPosition = InRenderingData->VertexOffsetPosition;
		UniqueRenderingDatas[InMeshHash]->VertexTypeSize = InRenderingData->VertexTypeSize;
		
		BuildUniqueRenderingSection(InRenderingData, UniqueRenderingDatas[InMeshHash]);

		//ִ����չ������
		InFun(InRenderingData);
	}
}

template<class T>
void FGeometry::BuildBoundingBox(const FRenderContent<T>& MeshData,BoundingBox& OutBounds)
{
	//��AABB
	{
		fvector_3d MaxPoint = fvector_3d(-FLT_MAX);
		fvector_3d MinPoint = fvector_3d(+FLT_MAX);
		for (auto& Tmp : MeshData.Data.VertexData)
		{
			MinPoint.x = math_libray::Min(Tmp.Position.x, MinPoint.x);
			MinPoint.y = math_libray::Min(Tmp.Position.y, MinPoint.y);
			MinPoint.z = math_libray::Min(Tmp.Position.z, MinPoint.z);

			MaxPoint.x = math_libray::Max(Tmp.Position.x, MaxPoint.x);
			MaxPoint.y = math_libray::Max(Tmp.Position.y, MaxPoint.y);
			MaxPoint.z = math_libray::Max(Tmp.Position.z, MaxPoint.z);
		}

		XMFLOAT3 XMFMaxPoint = EngineMath::ToFloat3(MaxPoint);
		XMFLOAT3 XMFMinPoint = EngineMath::ToFloat3(MinPoint);

		XMVECTOR XMFMaxPointTOR = XMLoadFloat3(&XMFMaxPoint);
		XMVECTOR XMFMinPointTOR = XMLoadFloat3(&XMFMinPoint);

		XMStoreFloat3(&OutBounds.Center, (XMFMaxPointTOR + XMFMinPointTOR) * 0.5f);
		XMStoreFloat3(&OutBounds.Extents, (XMFMaxPointTOR - XMFMinPointTOR) * 0.5f);
	}
}

template<class T>
void FGeometry::BuildRenderingSection(const FRenderContent<T>& MeshData, std::shared_ptr<FRenderingData> InRenderingData, FMeshData<T>& InMeshRenderingData)
{
	if (InRenderingData)
	{
		UINT VertexOffsetPosition = InMeshRenderingData.VertexData.size();
		UINT IndexOffsetPosition = InMeshRenderingData.IndexData.size();
		for (auto& Tmp : MeshData.SectionDescribe)
		{
			InRenderingData->Sections.push_back(FRenderingDataSection());
			FRenderingDataSection& InSection = InRenderingData->Sections[InRenderingData->Sections.size() - 1];

			InSection.MeshObjectIndex = MeshObjectCount++;

			//ÿ��Section�Ĵ�С
			InSection.IndexSize = Tmp.IndexSize;
			InSection.VertexSize = Tmp.VertexSize;

			//����ID
			InSection.MatrealIndex = Tmp.MatrealIndex;

			//��¼indexƫ��λ��
			InSection.IndexOffsetPosition = IndexOffsetPosition;
			IndexOffsetPosition += Tmp.IndexSize;

			//��¼Vertex
			InSection.VertexOffsetPosition = VertexOffsetPosition;
			VertexOffsetPosition += Tmp.VertexSize;

			//�ռ�����
			InRenderingData->IndexTotalSize += Tmp.IndexSize;
			InRenderingData->VertexTotalxSize += Tmp.VertexSize;
		}

		//��Ч�Ĳ���
		//�����ĺϲ�
		InMeshRenderingData.IndexData.insert(InMeshRenderingData.IndexData.end(), MeshData.Data.IndexData.begin(), MeshData.Data.IndexData.end());

		//����ĺϲ�
		InMeshRenderingData.VertexData.insert(InMeshRenderingData.VertexData.end(), MeshData.Data.VertexData.begin(), MeshData.Data.VertexData.end());
	}
}

void FGeometry::BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh,const FMeshRenderingData& MeshData,int InKey)
{
	BuildMesh_Interior(
	InMeshHash, 
	InMesh,
	MeshData,
	InKey,
	MeshRenderingData,
	[&](std::shared_ptr<FRenderingData> InRenderingData)
	{

	});
}

void FGeometry::BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh, const FSkinnedMeshRenderingData& SkinnedMeshData,int InKey)
{
	BuildMesh_Interior(
	InMeshHash,
	InMesh,
	SkinnedMeshData,
	InKey,
	SkinnedMeshRenderingData,
	[&](std::shared_ptr<FRenderingData> InRenderingData)
	{

	});
}

void FGeometry::DuplicateMesh(CMeshComponent* InMesh,std::shared_ptr<FRenderingData>& MeshData, int InKey)
{
	ERenderingMeshType MeshType = InMesh->GetMeshType();
	switch (MeshType)
	{
	case MESH_TYPE:
		DuplicateMesh_Interior(InMesh, MeshData, MeshRenderingData, InKey);
		break;
	case SKINNED_MESH_TYPE:
		DuplicateMesh_Interior(InMesh, MeshData, SkinnedMeshRenderingData, InKey);
		break;
	}
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	auto FindElement = UniqueRenderingDatas.find(InHash);
	if (FindElement != UniqueRenderingDatas.end())
	{
		MeshData = FindElement->second;

		return true;
	}

	return false;
}

UINT FGeometry::GetDrawObjectNumber() const
{
	return MeshObjectCount;

	int Count = 0;
	for (auto& Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto& SubTmp : Tmp->RenderDatas)
		{
			Count++;
		}
	}

	return Count;
}

void FGeometry::Build(int InType)
{
	UINT VertexSizeInBytes = 0;
	UINT IndexSizeInBytes = 0;

	void* VertexDataPtr = nullptr;
	void* IndexDataPtr = nullptr;

	if (GetRenderingDataInfo(
		(ERenderingMeshType)InType,
		VertexSizeInBytes,
		IndexSizeInBytes,
		VertexDataPtr,
		IndexDataPtr))
	{
		//ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes,&CPUVertexBufferPtr));

		//memcpy(CPUVertexBufferPtr->GetBufferPointer(),VertexDataPtr, VertexSizeInBytes);

		//ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes,&CPUIndexBufferPtr));

		//memcpy(CPUIndexBufferPtr->GetBufferPointer(), IndexDataPtr, IndexSizeInBytes);

		//ConstructBuffer::FConstructBuffer ConstructBuffer;
		//GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(VertexBufferTmpPtr,VertexDataPtr, VertexSizeInBytes);

		//GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(IndexBufferTmpPtr, IndexDataPtr, IndexSizeInBytes);

		m_GPUVertexBufferPtr.Create(L"VertexStream", 1, VertexSizeInBytes, VertexDataPtr);
		m_GPUIndexBufferPtr.Create(L"MeshIndexBuffer", 1, IndexSizeInBytes, IndexDataPtr);
	}
}

bool FGeometry::GetRenderingDataInfo(ERenderingMeshType InMeshType,UINT& VertexSizeInBytes, UINT& IndexSizeInBytes, void*& VertexDataPtr,void*& IndexDataPtr)
{
	VertexSizeInBytes = 0;
	IndexSizeInBytes = 0;

	switch (InMeshType)
	{
		case MESH_TYPE:
		{
			VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
			IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

			VertexDataPtr = MeshRenderingData.VertexData.data();
			IndexDataPtr = MeshRenderingData.IndexData.data();

			break;
		}
		case SKINNED_MESH_TYPE:
		{
			VertexSizeInBytes = SkinnedMeshRenderingData.GetVertexSizeInBytes();
			IndexSizeInBytes = SkinnedMeshRenderingData.GetIndexSizeInBytes();

			VertexDataPtr = SkinnedMeshRenderingData.VertexData.data();
			IndexDataPtr = SkinnedMeshRenderingData.IndexData.data();

			break;
		}
	}

	return VertexSizeInBytes != 0 && IndexSizeInBytes != 0;
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView(int InType)
{
	D3D12_VERTEX_BUFFER_VIEW VBV = m_GPUVertexBufferPtr.VertexBufferView();
	//VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();

	switch ((ERenderingMeshType)InType)
	{
	case MESH_TYPE:
	{
		UINT SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
		if (SizeInBytes != VBV.SizeInBytes)
		{
			assert(FALSE);
		}
		VBV.StrideInBytes = sizeof(FVertex);
		break;
	}
	case SKINNED_MESH_TYPE:
	{
		VBV.SizeInBytes = SkinnedMeshRenderingData.GetVertexSizeInBytes();
		VBV.StrideInBytes = sizeof(FSkinnedVertex);
		break;
	}
	}
	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView(int InType)
{
	D3D12_INDEX_BUFFER_VIEW IBV = m_GPUIndexBufferPtr.IndexBufferView();
	//IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	//IBV.Format = DXGI_FORMAT_R16_UINT;
	switch ((ERenderingMeshType)InType)
	{
	case MESH_TYPE:
	{
		UINT SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
		if (SizeInBytes != IBV.SizeInBytes)
		{
			assert(FALSE);
		}
		break;
	}
	case SKINNED_MESH_TYPE:
	{
		IBV.SizeInBytes = SkinnedMeshRenderingData.GetIndexSizeInBytes();
		break;
	}
	}
	return IBV;
}

void FGeometry::FindRenderingDatas(std::function<EFindValueType(std::shared_ptr<FRenderingData>&)> InFun)
{
	for (auto &Tmp : RenderingDatas)
	{
		if (InFun(Tmp) == EFindValueType::TYPE_COMPLETE)
		{
			break;
		}
	}
}

void FGeometry::DuplicateMeshRenderingSection(const std::shared_ptr<FRenderingData>& MeshData,std::shared_ptr<FRenderingData>& InMeshRenderingData)
{
	for (auto& Tmp : MeshData->Sections)
	{
		InMeshRenderingData->Sections.push_back(FRenderingDataSection());
		FRenderingDataSection& MeshRendering = InMeshRenderingData->Sections[
			InMeshRenderingData->Sections.size() - 1];

		MeshRendering.MeshObjectIndex = MeshObjectCount++;

		MeshRendering.IndexSize = Tmp.IndexSize;
		MeshRendering.VertexSize = Tmp.VertexSize;

		MeshRendering.MatrealIndex = Tmp.MatrealIndex;

		MeshRendering.IndexOffsetPosition = Tmp.IndexOffsetPosition;
		MeshRendering.VertexOffsetPosition = Tmp.VertexOffsetPosition;
	}
}

void FGeometry::BuildUniqueRenderingSection(const std::shared_ptr<FRenderingData>& MeshData,std::shared_ptr<FRenderingData>& InMeshRenderingData)
{
	for (auto& Tmp : MeshData->Sections)
	{
		InMeshRenderingData->Sections.push_back(FRenderingDataSection());
		FRenderingDataSection& InUniqueSection = InMeshRenderingData->Sections[InMeshRenderingData->Sections.size() - 1];

		InUniqueSection.MeshObjectIndex = Tmp.MeshObjectIndex;

		InUniqueSection.IndexSize = Tmp.IndexSize;
		InUniqueSection.VertexSize = Tmp.VertexSize;

		InUniqueSection.MatrealIndex = Tmp.MatrealIndex;

		InUniqueSection.IndexOffsetPosition = Tmp.IndexOffsetPosition;
		InUniqueSection.VertexOffsetPosition = Tmp.VertexOffsetPosition;
	}
}

