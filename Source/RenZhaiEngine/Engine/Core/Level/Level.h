#pragma once

#include "CoreObject/CoreMinimalObject.h"
#include "CodeReflection/CodeReflectionMacroTag.h"
#include "../../EngineType.h"
#include "Level.CodeReflection.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GActorObject;

class CLevel :public CCoreMinimalObject
{
	CODEREFLECTION()
public:
	CLevel();
public:
	//bool LineTraceBySingle(FCollisionResult &OutResult,const fvector_3d &InStart, const fvector_3d& InEnd);

	const vector<GActorObject*> &GetActors() const { return ActorObjects; }
	void AddActorObject(GActorObject* actor)
	{
		ActorObjects.push_back(actor);
	}
protected:

	//´æ´¢ÎÒÃÇµÄActors
	CVARIABLE()
	vector<GActorObject*> ActorObjects;
};