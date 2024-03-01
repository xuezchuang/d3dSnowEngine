#pragma once

#include "Core/Mesh.h"
#include "SkinnedMesh.CodeReflection.h"

class GSkinnedMesh :public GMesh
{
	CODEREFLECTION()
public:
	GSkinnedMesh();

	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(const string &InPath, const FIEParam& InParam = FIEParam());
};