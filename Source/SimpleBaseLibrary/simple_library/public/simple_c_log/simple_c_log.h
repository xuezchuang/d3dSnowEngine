// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"
#include "../simple_library_macro.h"

_CRT_BEGIN_C_HEADER
enum e_error
{
	SIMPLE_C_SUCCESS = 0,
	SIMPLE_C_LOG,
	SIMPLE_C_WARNING,
	SIMPLE_C_ERROR,
};

const char *get_log_filename();
const char *get_log_path();

SIMPLE_LIBRARY_API void init_log_system(const char *path);

//log
SIMPLE_LIBRARY_API bool log_wirte(enum e_error error, char *format, ...);
SIMPLE_LIBRARY_API char* get_error_str(enum e_error error, char* buff);
int get_log_str(enum e_error error, char* buff,const char *content_buff);

#define log_system(type,format,...) \
{ \
	char tmp_log_format[] = format; \
	log_wirte(type, tmp_log_format, __VA_ARGS__); \
}

#define log_success(format,...) log_system(SIMPLE_C_SUCCESS,format,__VA_ARGS__)
#define log_log(format,...) log_system(SIMPLE_C_LOG,format,__VA_ARGS__)
#define log_error(format,...) log_system(SIMPLE_C_ERROR,format,__VA_ARGS__)
#define log_warning(format,...) log_system(SIMPLE_C_WARNING,format,__VA_ARGS__)

_CRT_END_C_HEADER