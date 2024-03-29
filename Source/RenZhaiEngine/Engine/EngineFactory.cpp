#include "EngineFactory.h"
#if defined(_WIN32)
#include "Platform/Windows/WindowsEngine.h"
#endif

FEngineFactory::FEngineFactory()
{

}

CEngine* FEngineFactory::Instance()
{
#if defined(_WIN32)
	static CWindowsEngine instance;
	return &instance;
#elif defined(__linux__)

#endif // DEBUG
}
