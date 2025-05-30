#include "SSAODirectXRootSignature.h"

FSSAODirectXRootSignature::FSSAODirectXRootSignature()
{

}

void FSSAODirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    ////构建根签名
    //CD3DX12_ROOT_PARAMETER RootParam[7];
   
    ////texture描述表
    //CD3DX12_DESCRIPTOR_RANGE DescriptorNormalTextureSRV;
    //DescriptorNormalTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1, 0);

    ////Depth
    //CD3DX12_DESCRIPTOR_RANGE DescriptorDepthTextureSRV;
    //DescriptorDepthTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    ////Noise
    //CD3DX12_DESCRIPTOR_RANGE DescriptorNoiseTextureSRV;
    //DescriptorNoiseTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

    ////Accept 接受图
    //CD3DX12_DESCRIPTOR_RANGE DescriptorAcceptTextureSRV;
    //DescriptorAcceptTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

    //RootParam[Signature_SSAO_Viewport].InitAsConstantBufferView(0);//SSAOView对象

    //RootParam[Signature_SSAO_VerticalOrHorizontal].InitAsConstants(1,1);//通过常数直接控制

    //RootParam[Signature_SSAO_Blur].InitAsConstantBufferView(2);//Blur对象

    ////Normal
    //RootParam[Signature_SSAO_Normal].InitAsDescriptorTable(1, &DescriptorNormalTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    ////Depth
    //RootParam[Signature_SSAO_Depth].InitAsDescriptorTable(1, &DescriptorDepthTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    ////Noise
    //RootParam[Signature_SSAO_Noise].InitAsDescriptorTable(1, &DescriptorNoiseTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    ////Accept
    //RootParam[Signature_SSAO_Accept].InitAsDescriptorTable(1, &DescriptorAcceptTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    ////构建静态采样
    //StaticSamplerObject.BuildStaticSampler();

    //CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
    //    7,
    //    RootParam,
    //    StaticSamplerObject.GetSize(),//采样数量
    //    StaticSamplerObject.GetData(),//采样PTR
    //    D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ////创建
    //ComPtr<ID3DBlob> SerializeRootSignature;
    //ComPtr<ID3DBlob> ErrorBlob;
    //D3D12SerializeRootSignature(
    //    &RootSignatureDesc,
    //    D3D_ROOT_SIGNATURE_VERSION_1,
    //    SerializeRootSignature.GetAddressOf(),
    //    ErrorBlob.GetAddressOf());

    //if (ErrorBlob)
    //{
    //   char *p = (char*)ErrorBlob->GetBufferPointer();
    //   Engine_Log_Error("%s", p);
    //}

    ////创建
    ////GetD3dDevice()->CreateRootSignature(
    ////    0,
    ////    SerializeRootSignature->GetBufferPointer(),
    ////    SerializeRootSignature->GetBufferSize(),
    ////    IID_PPV_ARGS(&RootSignature));
}

void FSSAODirectXRootSignature::SetGraphicsRootSignature()
{
    Super::SetGraphicsRootSignature();
}

void FSSAODirectXRootSignature::Draw(float DeltaTime)
{
}

void FSSAODirectXRootSignature::PostDraw(float DeltaTime)
{
}
