#pragma once
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"
#include "../../simple_library_macro.h"

_CRT_BEGIN_C_HEADER

void get_path_directory_inline(char *path_buf);
void SIMPLE_LIBRARY_API get_path_directory(char *buf, const char *path_buf);
void SIMPLE_LIBRARY_API normalization_path(char *path_buf);
void SIMPLE_LIBRARY_API get_path_clean_filename(char *buf, const char *path_buf);
void normalization_directory(char *buf, const char *path_buf);

char* get_full_path(char* in_path_buf, int in_buff_len,const char *in_path);

SIMPLE_LIBRARY_API void get_path_clean_filename_w(wchar_t* buf, const wchar_t* path_buf);

_CRT_END_C_HEADER
