#pragma once

#include "RenZhaiEngineCoreMacro.h"
#include "RenZhaiEngineCore.h"

struct RENZHAIENGINECORE_API FEnginePathHelper
{
	static std::string GetEngineRootPath();

	static std::string GetEngineBinariesPath();

	static std::string GetEngineLogsPath();

	static std::string GetEngineSourcePath();

	static std::string GetEngineIntermediatePath();

	static std::string GetEngineCodeReflectionPath();

	static std::string GetEngineContentPath();

	static std::wstring GetEngineShadersPath();

	static std::string RelativeToAbsolutePath(const std::string &InPath);
};