#pragma once
#include "../Interface/BlueprintConnection.h"

class FBlueprintBezierCubicConnection :public FBlueprintConnection
{
public:

	virtual void DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition);
};