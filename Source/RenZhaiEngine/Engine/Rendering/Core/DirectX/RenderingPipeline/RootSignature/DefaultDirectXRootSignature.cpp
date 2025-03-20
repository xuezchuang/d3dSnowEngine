#include "DefaultDirectXRootSignature.h"
#include "DirectXRootSignatureType.h"

FDefaultDirectXRootSignature::FDefaultDirectXRootSignature()
{

}

void FDefaultDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
	FStaticSamplerObject StaticSamplerObject;
	StaticSamplerObject.BuildStaticSampler();
	m_RootSignature.Reset(11, StaticSamplerObject.SamplerDescs.size());
	m_RootSignature[Signature_Object].InitAsConstantBuffer(0);
	m_RootSignature[Signature_ObjectSkinned].InitAsConstantBuffer(1);
	m_RootSignature[Signature_Viewport].InitAsConstantBuffer(2);
	m_RootSignature[Signature_Light].InitAsConstantBuffer(3);
	m_RootSignature[Signature_Fog].InitAsConstantBuffer(4);

	m_RootSignature[Signature_Material].InitAsBufferSRV(0, D3D12_SHADER_VISIBILITY_ALL, 1);

	m_RootSignature[Signature_Texture2D].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		4,
		InTextureNum,
		//GeometryMap.GetDrawTexture2DResourcesNumber(),
		D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_CubeMapTexture].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_ShadowMap].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_ShadowCubeMap].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSignature[Signature_SSAO].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	for (size_t i = 0; i < StaticSamplerObject.SamplerDescs.size(); i++)
	{
		m_RootSignature.InitStaticSampler(i, StaticSamplerObject.SamplerDescs[i]);
	}
	

	/**
	 * Test
	 */
	//m_RootSignature.Reset(2);
	//m_RootSignature[0].InitAsConstantBuffer(0);
	//m_RootSignature[1].InitAsConstantBuffer(1);
	
	m_RootSignature.Finalize(L"RootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
}

void FDefaultDirectXRootSignature::SetGraphicsRootSignature()
{
    Super::SetGraphicsRootSignature();
}

void FDefaultDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDefaultDirectXRootSignature::PostDraw(float DeltaTime)
{
}
