#pragma once
#include "Core/simple_array_c_macro.h"

typedef struct
{
	wchar_t data[8196];
}wstr_node;
SIMPLE_ARRAY_C_STRUCT(simple_c_wstring, wstr_node)

typedef struct
{
	char data[8196];
}str_node;
SIMPLE_ARRAY_C_STRUCT(simple_c_string,str_node)

_CRT_BEGIN_C_HEADER
//խ�ַ�
 void init_string(simple_c_string *array_c);//��ʼ�� �����ڴ�

 void destroy_string(simple_c_string *array_c);

 void add_string(char const *  in_data,simple_c_string *array_c);

 void printf_string(simple_c_string *array_c);

 char*get_string(int in_index,simple_c_string *array_c);

 void dismantling_string(const char *in_data, const char *str_sub, simple_c_string *array_c);

 char*dismantling_string_by_index(int index, const char *split_str, const char *sub_str, char *buf);

 bool is_exist(char const *  in_data, simple_c_string *array_c);
 
 //���ַ�
 void dismantling_wstring(const wchar_t* in_data, const wchar_t* str_sub, simple_c_wstring* array_c);
 
 void init_wstring(simple_c_wstring* array_c);//��ʼ�� �����ڴ�

 void destroy_wstring(simple_c_wstring* array_c);

 void add_wstring(wchar_t const* in_data, simple_c_wstring* array_c);

 wchar_t* get_wstring(int in_index, simple_c_wstring* array_c);
 _CRT_END_C_HEADER