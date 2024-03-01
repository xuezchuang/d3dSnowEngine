#pragma once

#include "Core/Mesh.h"
#include "BoxMesh.CodeReflection.h"

class GBoxMesh :public GMesh
{
	CODEREFLECTION()
public:
	GBoxMesh();

	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Draw(float DeltaTime);

	void CreateMesh(float InHeight, float InWidth, float InDepth);
};