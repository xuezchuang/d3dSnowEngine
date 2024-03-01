#pragma once
#include "../../../Engine/EngineMinimal.h"
#include "CoreObject/PropertyObject.h"

class CCoreMinimalObject;
struct FLinkParam :public std::enable_shared_from_this<FLinkParam>
{
	simple_c_guid FuncGUID;
	string ParamName;
};

struct FFunctionTerminal
{
	FFunctionTerminal()
		:PropertyType(EPropertyType::INPUT_PROPERTY)
		,DefaultObject(nullptr)
	{}

	std::string Name;
	std::string Type;

	std::string DefaultValue;
	CCoreMinimalObject* DefaultObject;

	EPropertyType PropertyType;

	simple_c_guid FuncGUID;//本身属于谁的函数
	//如果这个值是有链接 认为是传入的那个 否则没有链接
	std::shared_ptr<FLinkParam> LinkParam;
};