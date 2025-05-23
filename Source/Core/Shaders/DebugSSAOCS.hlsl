//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#include "SSAORS.hlsli"

Texture2D<float> SsaoBuffer : register( t0 );
RWTexture2D<float3> OutColor : register( u0 );

[RootSignature(SSAO_RootSig)]
[numthreads( 8, 8, 1 )]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//if (SsaoBuffer[DTid.xy].x < 0.9)
	//	OutColor[DTid.xy] = float3(1.0f, 1.0f, 0.0f);
	//else
		OutColor[DTid.xy] = SsaoBuffer[DTid.xy].xxx;
}
