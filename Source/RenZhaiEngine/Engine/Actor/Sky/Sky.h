#pragma once
#include "../../Mesh/SphereMesh.h"
#include "Sky.CodeReflection.h"

class GSky :public GSphereMesh
{
	CODEREFLECTION()

public:
	GSky();

public:
	virtual void Tick(float DeltaTime);
};