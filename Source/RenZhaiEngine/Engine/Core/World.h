#pragma once

#include "CoreObject/CoreMinimalObject.h"
#include "CodeReflection/CodeReflectionMacroTag.h"
#include "../EngineType.h"
#include "Level/Level.h"
#include "World.CodeReflection.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;


class CWorld :public CCoreMinimalObject
{
	CODEREFLECTION()
public:
	CWorld();

	GCamera* GetCamera() const { return Camera; }

	template<class T>
	T *CreateActorObject(CClassObject* InObjectClass = NULL,BOOL bWorld = TRUE)
	{
		FCreateObjectParam ParamType;
		ParamType.Class = InObjectClass;
		ParamType.Outer = this;
		T* NewObject = CreateObject<T>(ParamType, new T());

		char ObjectName[128] = { 0 };
		sprintf(ObjectName, "%s_%d",
			NewObject->GetName().c_str(),
			ActorObjects.size());

		NewObject->Rename(ObjectName);

		if (!bWorld && Level)
			Level->AddActorObject(NewObject);
		else
			ActorObjects.push_back(NewObject);
		return NewObject;
	}
public:
	bool LineTraceBySingle(FCollisionResult &OutResult,const fvector_3d &InStart, const fvector_3d& InEnd);

	const vector<GActorObject*> &GetActors() const { return ActorObjects; }

	bool SaveLevel();
protected:
	CVARIABLE()
	GCamera* Camera;

	CVARIABLE()
	CLevel* Level;

	//´æ´¢ÎÒÃÇµÄActors
	CVARIABLE()
	vector<GActorObject*> ActorObjects;
};