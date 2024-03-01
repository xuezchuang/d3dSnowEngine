#include "ScriptBuilder.h"
#include "Terminal.h"
#include "CoreObject/PropertyObject.h"
#include "CodeReflection/ScriptCommand.h"

void FScriptBuilder::ProcessString(const FFunctionTerminal& Terminal)
{
	bool bLink = ProcessLinkParam(Terminal);

	Writer << EScriptCommand::Script_ConstString;

	ProcessInputParam(Terminal, [&]() {Writer << Terminal.DefaultObject; });
}

void FScriptBuilder::ProcessInt(const FFunctionTerminal& Terminal)
{
	bool bLink = ProcessLinkParam(Terminal);

	Writer << EScriptCommand::Script_Int;

	ProcessInputParam(Terminal, [&]() {Writer << atoi(Terminal.DefaultValue.c_str()); });
}

void FScriptBuilder::ProcessFloat(const FFunctionTerminal& Terminal)
{
	bool bLink = ProcessLinkParam(Terminal);

	Writer << EScriptCommand::Script_Float;

	ProcessInputParam(Terminal, [&]() {Writer << (float)atof(Terminal.DefaultValue.c_str()); });
}

void FScriptBuilder::ProcessBool(const FFunctionTerminal& Terminal)
{
	bool bLink = ProcessLinkParam(Terminal);
	bool InValue = Terminal.DefaultValue == "true" ? true : false;

	Writer << (InValue ? EScriptCommand::Script_True : EScriptCommand::Script_False);

	ProcessInputParam(Terminal, [&](){});
}

void FScriptBuilder::ProcessVector3D(const FFunctionTerminal& Terminal)
{
	bool bLink = ProcessLinkParam(Terminal);

	Writer << EScriptCommand::Script_Vector3D;

	ProcessInputParam(Terminal, [&]() {Writer << Terminal.DefaultValue; });
}

bool FScriptBuilder::ProcessLinkParam(const FFunctionTerminal& Terminal)
{
	Writer << Terminal.FuncGUID;
	Writer << (Terminal.LinkParam != nullptr);//是否位link值 主要是为了虚拟机那边存储

	if (Terminal.LinkParam)
	{
		Writer << Terminal.LinkParam->FuncGUID;
		Writer << Terminal.LinkParam->ParamName;

		return true;
	}

	return false;
}

void FScriptBuilder::ProcessInputParam(const FFunctionTerminal& Terminal, std::function<void()> InDefault)
{
	Writer << Terminal.PropertyType;
	if (Terminal.PropertyType == EPropertyType::INPUT_PROPERTY)
	{
		bool bLink = (Terminal.LinkParam != nullptr);
		Writer << bLink;

		if (!bLink)
		{
			InDefault();
		}
	}
}

void FScriptBuilder::SerializeTerminal(CPropertyObject* InProperty, FFunctionTerminal Terminal)
{
	if (InProperty->GetType() == "string")
	{
		ProcessString(Terminal);
	}
	else if (InProperty->GetType() == "int")
	{
		ProcessInt(Terminal);
	}
	else if (InProperty->GetType() == "float")
	{
		ProcessFloat(Terminal);
	}
	else if (InProperty->GetType() == "bool")
	{
		ProcessBool(Terminal);
	}
	else if (InProperty->GetType() == "fvector_3d")
	{
		ProcessVector3D(Terminal);
	}
}
