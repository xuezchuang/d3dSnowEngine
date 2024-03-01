#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../RenZhaiAssetHandleMacro.h"

class CClassObject;
class RENZHAIASSETHANDLE_API CAssetPackage :public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;

public:
	CAssetPackage();
	CAssetPackage(int);

public:
	virtual bool Save();
	virtual bool Load();
public:
	void SetAssetRelativePath(const string& InNewPath) { AssetRelativePath = InNewPath; }
	void SetAssetName(const string& InAssetName) { AssetName = InAssetName; }
	void SetClassObject(CClassObject* InClass) { ClassObject = InClass; }

public:
	CClassObject* GetClassObject() const { return ClassObject; }
	vector<std::string>& GetBlueprintData() { return BlueprintDatas; }
protected:
	std::string AssetRelativePath;
	std::string AssetName;
	vector<std::string> BlueprintDatas;
	CClassObject* ClassObject;
};