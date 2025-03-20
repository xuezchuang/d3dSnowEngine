//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
 
struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
	float2 uv : TEXCOORD0;
};

VertexOut VS(uint VertexId : SV_VertexID)
{
	VertexOut output = (VertexOut) 0.0f;
	output.uv = float2((VertexId << 1) & 2, VertexId & 2);
	output.PosH = float4(output.uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	return output;
}

float4 PS(VertexOut pin) : SV_Target
{
	if (pin.PosH.x < 400 && pin.PosH.y < 300)
		return float4(1.f, 0.0f, 0, 1);
	else if (pin.PosH.x < 400 && pin.PosH.y > 300)
		return float4(0.f, 1.0f, 0, 1);
	else if (pin.PosH.x > 400 && pin.PosH.y < 300)
		return float4(0.f, 0.0f, 1, 1);
	else
		return float4(1, 1, 1, 1);
	
	//if (pin.uv.x < 0.5 && pin.uv.y < 0.5)
	//	return float4(1.f, 0.0f, 0, 1);
	//else if (pin.uv.x < 0.5 && pin.uv.y > 0.5)
	//	return float4(0.f, 1.0f, 0, 1);
	//else if (pin.uv.x > 0.5 && pin.uv.y < 0.5)
	//	return float4(0.f, 0.0f, 1, 1);
	//else
	//	return float4(1, 1, 1, 1);
}

//cbuffer cbPerObject : register(b0)
//{
//	float4x4 gWorld;
//};

//cbuffer cbPass : register(b1)
//{
//	float4x4 gViewProj;
//};

//struct VertexIn
//{
//	float3 PosL : POSITION;
//	float4 Color : COLOR;
//};

//struct VertexOut
//{
//	float4 PosH : SV_POSITION;
//	float4 Color : COLOR;
//};

//VertexOut VS(VertexIn vin)
//{
//	VertexOut vout;
	
//	// Transform to homogeneous clip space.
//	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
//	vout.PosH = mul(posW, gViewProj);
	
//	// Just pass vertex color into the pixel shader.
//	vout.Color = vin.Color;
    
//	return vout;
//}

//float4 PS(VertexOut pin) : SV_Target
//{
//	return pin.Color;
//}


