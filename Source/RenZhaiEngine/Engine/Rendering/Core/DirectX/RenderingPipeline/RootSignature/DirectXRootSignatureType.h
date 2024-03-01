#pragma once

enum EDefaultDirectXRootSignatureType
{
	Signature_Object = 0,
	Signature_ObjectSkinned = 1,
	Signature_Viewport = 2,
	Signature_Light = 3,
	Signature_Fog = 4,

	Signature_Material = 5,
	Signature_Texture2D = 6,
	Signature_CubeMapTexture = 7,
	Signature_ShadowMap = 8,
	Signature_ShadowCubeMap = 9,
	Signature_SSAO = 10,
};

enum ESSAODirectXRootSignatureType
{
	Signature_SSAO_Viewport = 0,
	Signature_SSAO_VerticalOrHorizontal = 1,
	Signature_SSAO_Blur = 2,
	Signature_SSAO_Normal = 3,
	Signature_SSAO_Depth = 4,
	Signature_SSAO_Noise = 5,
	Signature_SSAO_Accept = 6,
};