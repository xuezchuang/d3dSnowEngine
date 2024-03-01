#pragma once
#include "../../EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"
#include "ActorObject.CodeReflection.h"

class CTransformationComponent;
//G -> Game
class GActorObject :public CCoreMinimalObject
{
	CODEREFLECTION()

	CVARIABLE()
	CTransformationComponent* RootComponent;

public:
	GActorObject();

public:
	FORCEINLINE CTransformationComponent* GetRootComponent() const {return RootComponent;}

	void GetBoundingBox(BoundingBox &OutBoundingBox);
	BoundingBox GetBoundingBox();

public:
	virtual void BeginInit();
	virtual void Tick(float DeltaTime);
	virtual void BeginExit();

public:
	CDIAPER(CodeType = Event, Category = "Event", DisplayName = "Begin")
	void Begin_Blueprint();

	CDIAPER(CodeType = Event, Category = "Event", DisplayName = "End")
	void End_Blueprint(int InType);

	CDIAPER(CodeType = Event, Category = "Event", DisplayName = "Tick")
	void Tick_Blueprint(float DeltaTime);

public:
	CDIAPER(CodeType = PureFunction, Category = "Actor")
	string GetActorString() { return "ActorString"; }

	CDIAPER(CodeType = PureFunction, Category = "Actor")
	string GetString() { return "Hellllsdaskdaksdkasdmka"; }

	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetRotation(const frotator& InNewRotation);
	virtual void SetRotationQuat(const fquat& InNewQuatRotation);
	virtual void SetScale(const fvector_3d& InNewScale);

	CDIAPER(CodeType = PureFunction, Category = "Actor")
	virtual void SetPickup(bool bNewPickup);

public:
	XMFLOAT3& GetPosition();
	frotator GetRotation()const;

	CDIAPER(CodeType = PureFunction, Category = "Actor")
	fvector_3d GetScale()const;

	fquat GetRotationQuat() const;

	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUPVector();

	//≤‚ ‘
public:
	CVARIABLE(CodeType = Resources, Category="VVVV", aaaa = "asdasdas", www = dasdasd, xxxxx, wwww, ddd)
	string NameXXXX;

	CVARIABLE(CodeType = Resources)
	bool PlayerState;

	CVARIABLE(CodeType = Resources)
	fvector_3d posxcxxx;

	CVARIABLE(CodeType = Resources, Step = 4, Draggable)
	int helloInt;

	CVARIABLE(CodeType = Resources, MaxCrop="100", MinCrop=0)
	float PlayerState123;

	CVARIABLE(CodeType = Resources, AllowablePercentage)
	float PlayerState1233;

	CVARIABLE(CodeType = Resources)
	vector<int> TTTT;

	CVARIABLE(CodeType = Resources)
	vector<float> TTTTXXX;
	
	CVARIABLE(CodeType = Resources)
	vector<double> TTTTXXX111;

	CVARIABLE(CodeType = Resources)
	map<int,string> TTTT1234567;

	CVARIABLE(CodeType = Resources)
	map<string,string> TTTT12345671;

	CVARIABLE(CodeType = Resources)
	map<char,long> TTTT12345671long;
};