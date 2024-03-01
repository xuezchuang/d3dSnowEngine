#pragma once

#include "Core/Mesh.h"
#include "CylinderMesh.CodeReflection.h"

class GCylinderMesh :public GMesh
{
	CODEREFLECTION()
public:
	GCylinderMesh();

	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(
		float InTopRadius, 
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision, 
		uint32_t InHeightSubdivision);
};