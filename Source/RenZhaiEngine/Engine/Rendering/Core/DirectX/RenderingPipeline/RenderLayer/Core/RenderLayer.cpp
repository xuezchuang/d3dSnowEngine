#include "RenderLayer.h"
#include "../RenderLayerManage.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"
#include "../../RootSignature/DirectXRootSignatureType.h"

FRenderLayer::FRenderLayer()
	:RenderPriority(0)
{
	
}

void FRenderLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderLayers.push_back(this->shared_from_this());
}

void FRenderLayer::BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& InMacro)
{
	 {
		 ShaderType::FShaderMacro ShaderMacro;

		 char TextureNumBuff[10] = { 0 };
		 ShaderMacro.Name = "TEXTURE2D_MAP_NUM";
		 ShaderMacro.Definition = _itoa(GeometryMap->GetDrawTexture2DResourcesNumber(), TextureNumBuff, 10);

		 InMacro.push_back(ShaderMacro);
	 }

	 //{
		// ShaderType::FShaderMacro ShaderMacro;

		// char TextureNumBuff[10] = { 0 };
		// ShaderMacro.Name = "CUBE_MAP_NUM";
		// ShaderMacro.Definition = _itoa(GeometryMap->GetDrawCubeMapResourcesNumber(), TextureNumBuff, 10);

		// InMacro.push_back(ShaderMacro);
	 //}

	 {
		 ShaderType::FShaderMacro ShaderMacro;
		 ShaderMacro.Name = "START_UP_FOG";
		 ShaderMacro.Definition = GeometryMap->IsStartUPFog() ? "1":"0";

		 InMacro.push_back(ShaderMacro);
	 }
}

void FRenderLayer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	GeometryMap = InGeometryMap;
	DirectXPipelineState = InDirectXPipelineState;
}

void FRenderLayer::PreDraw(float DeltaTime)
{

}

void FRenderLayer::Draw(GraphicsContext& context, float DeltaTime)
{
	//模型构建
	DrawMesh(context,DeltaTime);
}

void FRenderLayer::PostDraw(float DeltaTime)
{
	//删除RenderData弱指针的过程
	vector<vector<std::weak_ptr<FRenderingData>>::const_iterator> RemoveRenderingData;
	for (vector<std::weak_ptr<FRenderingData>>::const_iterator Iter = RenderDatas.begin();
		Iter != RenderDatas.end();
		++Iter)
	{
		if (Iter->expired())
		{
			RemoveRenderingData.push_back(Iter);
		}
	}

	for (auto &Tmp : RemoveRenderingData)
	{
		RenderDatas.erase(Tmp);
	}
}

void FRenderLayer::DrawObject(GraphicsContext& context,float DeltaTime,std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC)
{
	if (InWeakRenderingData.expired())//弱指针是不是被释放了
	{
		return;
	}


	if (std::shared_ptr<FRenderingData> InRenderingData = InWeakRenderingData.lock())
	{
		auto GetRenderingConditions = [&]() -> bool
		{
			if (InRenderingData->Mesh->IsVisible())
			{
				switch (RC)
				{
					case RC_Shadow:
					{
						return InRenderingData->Mesh->IsCastShadow();
					}
				}

				return true;
			}

			return false;
		};

		if (GetRenderingConditions())
		{
			//UINT MeshOffset = GeometryMap->MeshConstantBufferViews.GetConstantBufferByteSize();

			D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometrys[InRenderingData->GeometryKey].GetVertexBufferView(InRenderingData->GeometryKey);
			D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometrys[InRenderingData->GeometryKey].GetIndexBufferView(InRenderingData->GeometryKey);

			context.SetIndexBuffer(IBV);
			context.SetVertexBuffer(0, VBV);

			//D3D12_GPU_VIRTUAL_ADDRESS FirstVirtualMeshAddress = GeometryMap->MeshConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
			//auto DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());

			//定义我们要绘制的哪种图元 点 线 面
			EMaterialDisplayStatusType DisplayStatus = (*InRenderingData->Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
			//context.SetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)DisplayStatus);
			context.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//context.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			
			//模型起始地址偏移
			//DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);
			//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

			if (InRenderingData->GetMeshType() == ERenderingMeshType::SKINNED_MESH_TYPE)
			{
				//GeometryMap->UpdateSkinned(DeltaTime, InRenderingData->Mesh);
				////GeometryMap->DrawSkinned(DeltaTime, InRenderingData->SkinnedMeshObjectIndex);
				//int ConstantBufferByteSize = SkinnedConstantBufferViews.GetConstantBufferByteSize();
				//D3D12_GPU_VIRTUAL_ADDRESS SkinnedAddress =
				//	SkinnedConstantBufferViews.GetBuffer()->GetGPUVirtualAddress() +
				//	ConstantBufferByteSize * InSkinnedIndex;


				//context.SetDynamicConstantBufferView(Signature_ObjectSkinned, sizeof(FObjectTransformation), ObjectConstant);
			}
			else
			{
				//context.SetDynamicConstantBufferView(Signature_ObjectSkinned
			}

			for (FRenderingDataSection& Tmp : InRenderingData->Sections)
			{
				const FObjectTransformation& ObjectConstant = GeometryMap->MeshObjectConstant[Tmp.MeshObjectIndex];
				context.SetDynamicConstantBufferView(Signature_Object, sizeof(FObjectTransformation), &ObjectConstant);


				//真正的绘制
				context.DrawIndexedInstanced(
					Tmp.IndexSize,//顶点数量
					1,//绘制实例数量
					Tmp.IndexOffsetPosition,//顶点缓冲区第一个被绘制的索引
					InRenderingData->VertexOffsetPosition,//GPU 从索引缓冲区读取的第一个索引的位置。
					0);//在从顶点缓冲区读取每个实例数据之前添加到每个索引的值。
			}
		}
	}
}

void FRenderLayer::FindObjectDraw(float DeltaTime, const CMeshComponent* InKey)
{
	for (auto& InRenderingData : RenderDatas)
	{
		if (!InRenderingData.expired())
		{
			if (InRenderingData.lock()->Mesh == InKey)
			{
				//DrawObject(DeltaTime, InRenderingData);
				break;
			}
		}	
	}
}

void FRenderLayer::BuildPSO()
{
	//DirectXPipelineState->BuildParam();

	BuildShader();

	////需要额外定制，走代理
	//if (BuildPSODelegate.IsBound())
	//{
	//	BuildPSODelegate.Execute(DirectXPipelineState->GetGPSDesc());
	//}
}

void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& InWeakRenderingData : RenderDatas)//暂时先这么写
	{	
		if (!InWeakRenderingData.expired())
		{
			if (std::shared_ptr<FRenderingData> InRenderingData = InWeakRenderingData.lock())
			{
				//构造模型的world
				{
					XMFLOAT3& Position = InRenderingData->Mesh->GetPosition();
					fvector_3d Scale = InRenderingData->Mesh->GetScale();

					XMFLOAT3 RightVector = InRenderingData->Mesh->GetRightVector();
					XMFLOAT3 UPVector = InRenderingData->Mesh->GetUPVector();
					XMFLOAT3 ForwardVector = InRenderingData->Mesh->GetForwardVector();

					EngineMath::BuildMatrix(
						InRenderingData->WorldMatrix,
						Position,
						Scale,
						RightVector,
						UPVector, 
						ForwardVector);

				}

				//更新模型位置
				XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData->WorldMatrix);

				XMFLOAT4X4 TextureTransform(InRenderingData->TextureTransform);
				const XMFLOAT3& TexScale = InRenderingData->Mesh->GetTextureScale();
				XMStoreFloat4x4(&TextureTransform, XMMatrixScaling(TexScale.x, TexScale.y, TexScale.z));
				XMMATRIX ATRIXTextureTransform = XMLoadFloat4x4(&TextureTransform);
				
				//法线矩阵
				XMVECTOR AATRIXWorldDeterminant = XMMatrixDeterminant(ATRIXWorld);
				XMMATRIX NormalInverseMatrix = XMMatrixInverse(&AATRIXWorldDeterminant, ATRIXWorld);
				
				//更新ObjectIndex
				for (auto &Tmp : InRenderingData->Sections)
				{
					FObjectTransformation& ObjectTransformation = GeometryMap->MeshObjectConstant[Tmp.MeshObjectIndex];
					XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));
					XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRIXTextureTransform));

					//XMStoreFloat4x4(&ObjectTransformation.World, ATRIXWorld);
					//XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, ATRIXTextureTransform);
					XMStoreFloat4x4(&ObjectTransformation.NormalTransformation, NormalInverseMatrix);

					//收集材质Index
					if (auto& InMater = (*InRenderingData->Mesh->GetMaterials())[Tmp.MatrealIndex])
					{
						ObjectTransformation.MaterialIndex = InMater->GetMaterialIndex();
					}
				}
			}
		}
	}
}

void FRenderLayer::ResetPSO(GraphicsContext& GfxContext)
{
	
}

void FRenderLayer::ResetPSO(EPipelineState InPipelineState)
{

}

void FRenderLayer::DrawMesh(GraphicsContext& context,float DeltaTime, ERenderingConditions RC)
{
	for (auto& InRenderingData : RenderDatas)
	{
		DrawObject(context,DeltaTime, InRenderingData, RC);
	}
}

std::wstring FRenderLayer::BuildShadersPaths(const std::wstring& InShadersHLSLName)
{
	return FEnginePathHelper::GetEngineShadersPath() + L"\\" + InShadersHLSLName +L".hlsl";
}

void FRenderLayer::Add(std::weak_ptr<FRenderingData> InRenderingData)
{
	RenderDatas.push_back(InRenderingData);
}

void FRenderLayer::Remove(std::weak_ptr<FRenderingData> InRenderingData)
{
	if (!InRenderingData.expired())
	{
		for (vector<std::weak_ptr<FRenderingData>>::const_iterator Iter = RenderDatas.begin();
			Iter != RenderDatas.end();
			++Iter)
		{
			if (!Iter->expired())
			{
				if (Iter->lock() == InRenderingData.lock())
				{
					RenderDatas.erase(Iter);
					break;
				}
			}
		}
	}
}

void FRenderLayer::Clear()
{
	RenderDatas.clear();
}
