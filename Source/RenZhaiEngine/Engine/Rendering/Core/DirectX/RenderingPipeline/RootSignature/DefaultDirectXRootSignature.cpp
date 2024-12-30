#include "DefaultDirectXRootSignature.h"

FDefaultDirectXRootSignature::FDefaultDirectXRootSignature()
{

}

void FDefaultDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[11];
   
    //texture������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 
        InTextureNum, 4);

    //SSAO
    CD3DX12_DESCRIPTOR_RANGE DescriptorSSAOMapSRV;
    DescriptorSSAOMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

    //��̬CubeMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
    DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ShadowMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowMapSRV;
    DescriptorShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

    //ShadowCubeMap
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowCubeMapSRV;
    DescriptorShadowCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    RootParam[Signature_Object].InitAsConstantBufferView(0);//����
    RootParam[Signature_ObjectSkinned].InitAsConstantBufferView(1);//��Ƥ
    RootParam[Signature_Viewport].InitAsConstantBufferView(2);//�ӿ�
    RootParam[Signature_Light].InitAsConstantBufferView(3);//�ƹ�
    RootParam[Signature_Fog].InitAsConstantBufferView(4);//��

    //t
    RootParam[Signature_Material].InitAsShaderResourceView(0,1);//����

    //2D��ͼ
    RootParam[Signature_Texture2D].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //CubeMap��ͼ
    RootParam[Signature_CubeMapTexture].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //ShadowMap
    RootParam[Signature_ShadowMap].InitAsDescriptorTable(1, &DescriptorShadowMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //ShadowCubeMap
    RootParam[Signature_ShadowCubeMap].InitAsDescriptorTable(1, &DescriptorShadowCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //SSAO
    RootParam[Signature_SSAO].InitAsDescriptorTable(1, &DescriptorSSAOMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //������̬����
    StaticSamplerObject.BuildStaticSampler();

    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        11,
        RootParam,
        StaticSamplerObject.GetSize(),//��������
        StaticSamplerObject.GetData(),//����PTR
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    //����
    ComPtr<ID3DBlob> SerializeRootSignature;
    ComPtr<ID3DBlob> ErrorBlob;
    D3D12SerializeRootSignature(
        &RootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        SerializeRootSignature.GetAddressOf(),
        ErrorBlob.GetAddressOf());

    if (ErrorBlob)
    {
       char *p = (char*)ErrorBlob->GetBufferPointer();
       Engine_Log_Error("%s", p);
    }

    //����
    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));
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
