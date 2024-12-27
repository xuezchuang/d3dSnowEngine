#pragma once

//#ifndef SIMPLE_LIBRARY_EXPORTS
//#define SIMPLE_LIBRARY_EXPORTS
//#endif

#ifdef SIMPLE_LIBRARY_STATIC // 如果定义了静态库宏
#define SIMPLE_LIBRARY_API
#elif  SIMPLE_LIBRARY_EXPORTS
#define SIMPLE_LIBRARY_API __declspec(dllexport)
#else
#define SIMPLE_LIBRARY_API __declspec(dllimport)
#endif