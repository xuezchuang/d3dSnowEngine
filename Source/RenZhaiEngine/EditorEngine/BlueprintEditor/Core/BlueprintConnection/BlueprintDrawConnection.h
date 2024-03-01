#pragma once
#include "../../../../Engine/EngineMinimal.h"

class FNode;
struct FBlueprintPinConnection
{
	FBlueprintPinConnection(){}
	FBlueprintPinConnection(
		std::weak_ptr<FNode> InPin1,
		std::weak_ptr<FNode> InPin2);

	std::weak_ptr<FNode> Pin1;
	std::weak_ptr<FNode> Pin2;

	bool bReadyToRemove;
};

struct FBlueprintDrawConnection :public std::enable_shared_from_this<FBlueprintDrawConnection>
{
	FBlueprintDrawConnection()
		:ConnectionID(0)
	{}

public:
	void SetConnectionType(int InConnectionID);
	void Draw(float DeltaTime);

	void Add(const FBlueprintPinConnection& InNewConnection);

	void BreakOff(std::shared_ptr<FNode> InBreakOffPin);

	bool IsExit(const FBlueprintPinConnection& InNewConnection);
protected:
	std::vector<FBlueprintPinConnection> PinConnections;
	int ConnectionID;
};