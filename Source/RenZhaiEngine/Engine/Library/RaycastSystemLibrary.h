#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

class CWorld;
struct FRaycastSystemLibrary
{
	static bool HitResultByScreen(CWorld *InWorld,int ScreenX,int ScreenY, FCollisionResult &OutResult);
	static bool HitSpecificObjectsResultByScreen(
		CWorld* InWorld, GActorObject* InSpecificObjects, 
		const std::vector<CComponent*>& IgnoreComponents, 
		int ScreenX, int ScreenY, FCollisionResult& OutResult);

	static bool GetRaycastByScreenParam(
		CWorld* InWorld,
		const fvector_2id& ScreenXY,
		XMVECTOR& OriginPoint,
		XMVECTOR& Direction,
		XMMATRIX& ViewInverseMatrix);
};