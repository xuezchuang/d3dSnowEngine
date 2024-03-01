#include "EngineMisLibrary.h"
#include "CoreMacro.h"

GEngineMisLibrary::GEngineMisLibrary()
	:Super()
{

}

void GEngineMisLibrary::PrintLog(const string& InMsg, bool& bPrint, bool bPrintLog, bool bPrintScreen)
{
#if EDITOR_ENGINE
	if (bPrintLog)
	{
		Engine_Log("%s", InMsg.c_str());
	}

	bPrint = true;
#endif
}

string GEngineMisLibrary::Vector3ToString(const fvector_3d& InValue)
{
	return simple_cpp_string_algorithm::printf("(x=%f,y=%f,z=%f)", InValue.x, InValue.y, InValue.z);
};

string GEngineMisLibrary::BoolToString(bool InValue)
{
	return simple_cpp_string_algorithm::printf("%s", InValue ? "true" : "false");
}

string GEngineMisLibrary::FloatToString(float InValue)
{
	return simple_cpp_string_algorithm::printf("%f", InValue);
}

string GEngineMisLibrary::IntToString(int InValue)
{
	return simple_cpp_string_algorithm::printf("%i", InValue);
}