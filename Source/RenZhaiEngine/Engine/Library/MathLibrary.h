#pragma once
#include "Core/BlueprinFunctiontLibrary.h"
#include "MathLibrary.CodeReflection.h"

class GMathLibrary :public GBlueprinFunctiontLibrary
{
	CODEREFLECTION()

public:
	GMathLibrary();

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)+(float)")
	static float FloatAddFloat(float A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)+(float)")
	static int IntAddFloat(int A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)+(int)")
	static int IntAddInt(int A, int B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)+(int)")
	static float FloatAddInt(float A, int B);


	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)-(float)")
	static float FloatSubtractFloat(float A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)-(float)")
	static int IntSubtractFloat(int A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)-(int)")
	static int IntSubtractInt(int A, int B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)-(int)")
	static float FloatSubtractInt(float A, int B);


	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)/(float)")
	static float FloatDivideFloat(float A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)/(float)")
	static int IntDivideFloat(int A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)/(int)")
	static int IntDivideInt(int A, int B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)/(int)")
	static float FloatDivideInt(float A, int B);


	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)x(float)")
	static float FloatMultiplyFloat(float A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)x(float)")
	static int IntMultiplyFloat(int A, float B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(int)x(int)")
	static int IntMultiplyInt(int A, int B);

	CDIAPER(CodeType = PureFunction, Category = "Math", DisplayName = "(float)x(int)")
	static float FloatMultiplyInt(float A, int B);
};