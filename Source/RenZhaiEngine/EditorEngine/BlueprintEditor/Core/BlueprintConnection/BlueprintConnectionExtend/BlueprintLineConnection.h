#pragma once
#include "../Interface/BlueprintConnection.h"

class FBlueprintLineConnection :public FBlueprintConnection
{
public:

	virtual void DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition);
};