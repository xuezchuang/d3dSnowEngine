#pragma once

#include "../../RenZhaiBuildTool.h"
#include "../CppSourceCodeType.h"

namespace IntermediateFile
{
	bool Builder(
		const FClassAnalysis& InClassAnalysis,
		std::vector<std::string>& OutAnalysisRawH,
		std::vector<std::string>& OutAnalysisRawCPP);
}