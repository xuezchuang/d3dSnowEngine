#pragma once
#include "Core/BlueprinFunctiontLibrary.h"
#include "EngineMisLibrary.CodeReflection.h"

class GEngineMisLibrary :public GBlueprinFunctiontLibrary
{
	CODEREFLECTION()

public:
	GEngineMisLibrary();

	CDIAPER(CodeType = Function, Category = "Engine Mis")
	static void PrintLog(const string& InMsg, bool &bPrint, bool bPrintLog = true, bool bPrintScreen = true);

public:
	CDIAPER(CodeType = PureFunction, Category = "Type Convert")
	static fvector_3d GetVector() { return fvector_3d(1.f, 12.f, 333.f); }

	CDIAPER(CodeType = PureFunction, Category = "Type Convert")
	static string GetString() { return "Hello World."; }

	CDIAPER(CodeType = PureFunction, Category = "Type Convert")
	static string Vector3ToString(const fvector_3d& InValue);

	CDIAPER(CodeType = PureFunction, Category = "Type Convert")
	static string BoolToString(bool InValue);

	CDIAPER(CodeType = PureFunction, Category = "Type Convert")
	static string FloatToString(float InValue);

	CDIAPER(CodeType = PureFunction, Category = "Type Convert")
	static string IntToString(int InValue);
};