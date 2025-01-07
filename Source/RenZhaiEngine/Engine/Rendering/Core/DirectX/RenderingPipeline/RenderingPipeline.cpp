#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "RootSignature/DirectXRootSignatureType.h"
#include "StaticSampler/StaticSamplerObject.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FSkinnedMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash, InMesh, MeshData);
}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash,InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData)
{
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	SSAO.UpdateCalculations(DeltaTime, ViewportInfo);
	GeometryMap.DynamicShadowCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::OnResetSize(int InWidth, int InHeight)
{
	SSAO.OnResetSize(InWidth,InHeight);
	DynamicCubeMap.OnResetSize(InWidth, InHeight);
	GeometryMap.OnResetSize(InWidth, InHeight);
	RenderLayer.OnResetSize(InWidth, InHeight);
}

void FRenderingPipeline::BuildPipeline()
{																									//��ʼ��GPS����
	DirectXPipelineState.ResetGPSDesc();

	//��Ⱦ�㼶�ĳ�ʼ��
	RenderLayer.Init(&GeometryMap,&DirectXPipelineState);

	//ͳһ�������Ⱦ�㼶��������
	RenderLayer.Sort();

	//��ȡ��ͼ����
	GeometryMap.LoadTexture();

	////��������
	//GeometryMap.BuildFog();

	////������̬��CubeMap
	//DynamicCubeMap.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//SSAO.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//SSAO.Init(GetVieportWidth(),GetVieportHeight());
	//
	//GeometryMap.DynamicShadowMap.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//GeometryMap.DynamicShadowCubeMap.Init(
	//	&GeometryMap,
	//	&DirectXPipelineState,
	//	&RenderLayer);

	//������ǩ��
	//��ԭ��ֻ��һ��һ����??? һ�����͵Ŀ���һ���...+
	FStaticSamplerObject StaticSamplerObject;
	StaticSamplerObject.BuildStaticSampler();
	m_RootSignature.Reset(11, StaticSamplerObject.SamplerDescs.size());
	m_RootSignature[Signature_Object].InitAsBufferCBV(0);
	m_RootSignature[Signature_ObjectSkinned].InitAsBufferCBV(1);
	m_RootSignature[Signature_Viewport].InitAsBufferCBV(2);
	m_RootSignature[Signature_Light].InitAsBufferCBV(3);
	m_RootSignature[Signature_Fog].InitAsBufferCBV(4);

	m_RootSignature[Signature_Material].InitAsBufferSRV(0, 1);


	m_RootSignature[Signature_Texture2D].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, GeometryMap.GetDrawTexture2DResourcesNumber(), D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_CubeMapTexture].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_ShadowMap].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_ShadowCubeMap].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_SSAO].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 1, D3D12_SHADER_VISIBILITY_PIXEL);


	m_RootSignature.InitStaticSampler(StaticSamplerObject.SamplerDescs);
	m_RootSignature.Finalize(L"RootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	DirectXPipelineState.BindRootSignature(&m_RootSignature);

	////����ģ��
	//GeometryMap.Build();

	////������̬����Mesh
	//GeometryMap.BuildDynamicReflectionMesh();

	////��������������
	//GeometryMap.BuildDescriptorHeap();

	//SSAO.BuildDescriptors();

	////��ʼ��CubeMap �����
	//DynamicCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));

	////�������ģ������
	//DynamicCubeMap.BuildDepthStencilDescriptor();

	////����RTVDes
	//DynamicCubeMap.BuildRenderTargetDescriptor();

	////�������ģ��
	//DynamicCubeMap.BuildDepthStencil();

	////������Ӱ
	//GeometryMap.BuildShadow();

	////��������������
	//GeometryMap.BuildMeshConstantBuffer();

	////�������ʳ���������
	//GeometryMap.BuildMaterialShaderResourceView();

	////�����ƹⳣ��������
	//GeometryMap.BuildLightConstantBuffer();

	////�������ǵ��ӿڳ�����������ͼ
	//GeometryMap.BuildViewportConstantBufferView();

	////������ͼ
	//GeometryMap.BuildTextureConstantBuffer();

	////������������������
	//GeometryMap.BuildFogConstantBuffer();

	////��Ƥ
	//GeometryMap.BuildSkinnedMeshConstantBuffer();

	////����SSAO
	//SSAO.Build();

	////�洢һ��Ĭ�ϵ�GPS��������
	//DirectXPipelineState.SaveGPSDescAsDefault();

	//ͨ���㼶������PSO
	RenderLayer.BuildPSO();
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//��Ҫһ��PSO
	DirectXPipelineState.PreDraw(DeltaTime);

	GeometryMap.SetDescriptorHeaps();
	RootSignature.SetGraphicsRootSignature();

	//��Ⱦ�ƹ������ͼ��(����Ҫ���� ���λ�� �����������)
	GeometryMap.Draw(DeltaTime);

	//��ȾSSAO
	//SSAO.Draw(DeltaTime);
	RootSignature.SetGraphicsRootSignature();

	//�洢���ǵ�SSAO��ָ����buffer
	SSAO.SaveToSSAOBuffer();

	//���ӿ��������
	//ClearMainSwapChainCanvas();

	////���°���ͼ
	//GeometryMap.Draw2DTexture(DeltaTime);

	////��Ⱦ
	//GeometryMap.DynamicShadowCubeMap.PreDraw(DeltaTime);

	////��Ⱦ��Ӱ
	//GeometryMap.DrawShadow(DeltaTime);

	////��̬����
	//if(DynamicCubeMap.IsExitDynamicReflectionMesh())
	//{
	//	DynamicCubeMap.PreDraw(DeltaTime);
	//}

	//RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(FCommandContext& context, float DeltaTime)
{	
	context.SetRootSignature(m_RootSignature);
	//���ӿ�
	GeometryMap.DrawViewport(DeltaTime);

	////CubeMap ����ԭ�ȱ��޸ĵ�CubeMap
	//GeometryMap.DrawCubeMapTexture(DeltaTime);

	////����㼶
	//RenderLayer.Draw(RENDERLAYER_BACKGROUND,DeltaTime);
	RenderLayer.Draw(context, RENDERLAYER_OPAQUE, DeltaTime);
	//RenderLayer.Draw(RENDERLAYER_SKINNED_OPAQUE, DeltaTime);//��Ⱦ������
	//RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);

	////��Ⱦѡ���
	//RenderLayer.Draw(RENDERLAYER_SELECT, DeltaTime);
	//
	////��Ⱦ��ת��Ƭ
	//RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE_ROT_PLANE, DeltaTime);

	////��Ⱦ�����ֱ�
	//RenderLayer.Draw(RENDERLAYER_OPERATION_HANDLE, DeltaTime);

	//DirectXPipelineState.CaptureKeyboardKeys();
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	RenderLayer.PostDraw(DeltaTime);
	DirectXPipelineState.PostDraw(DeltaTime);
}
