#pragma once

#include "Core/Mesh.h"
#include "CustomMesh.CodeReflection.h"

class GCustomMesh :public GMesh
{
	CODEREFLECTION()
public:
	GCustomMesh();

	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(const string &InPath, const FIEParam& InParam = FIEParam());


	CVARIABLE(CodeType = Resources, Category = Render, Draggable)
	bool mTest;
};