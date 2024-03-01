#pragma once
#include "Archive/Archive.h"

struct FFunctionTerminal;
class CPropertyObject;

struct FScriptBuilder
{
	FScriptBuilder(vector<unsigned char>& InData)
		:Writer(InData)
	{}

	FArchive Writer;

public:
	void ProcessString(const FFunctionTerminal& Terminal);
	void ProcessInt(const FFunctionTerminal& Terminal);
	void ProcessFloat(const FFunctionTerminal& Terminal);
	void ProcessBool(const FFunctionTerminal& Terminal);
	void ProcessVector3D(const FFunctionTerminal& Terminal);

	bool ProcessLinkParam(const FFunctionTerminal& Terminal);
	void ProcessInputParam(const FFunctionTerminal& Terminal, std::function<void()> InDefault);

public:
	void SerializeTerminal(CPropertyObject* InProperty, FFunctionTerminal Terminal);
};