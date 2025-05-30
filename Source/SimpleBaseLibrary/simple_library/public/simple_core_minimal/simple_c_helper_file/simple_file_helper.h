#pragma once

#include "simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"
#include "../../simple_library_macro.h"

typedef struct
{
	int index;
	char paths[2048][512];//MAX_PATH
}def_c_paths;

typedef struct
{
	int index;
	wchar_t paths[2048][512];//MAX_PATH
}def_c_paths_w;

//v2版本 路径自适应
typedef struct
{
	int index;//表示当前偏移
	int num;//表示数量
	char* paths;//MAX_PATH
}def_c_paths_v2;

typedef struct
{
	int index;//表示当前偏移
	int num;//表示数量
	wchar_t* paths;//MAX_PATH
}def_c_paths_w_v2;

_CRT_BEGIN_C_HEADER
#define _number_of_successful_conversions(a) a
#define _out_pram(a) a
#define _in_pram(a) a

void SIMPLE_LIBRARY_API init_def_c_paths(def_c_paths *c_paths);
void init_def_c_paths_w(def_c_paths_w* c_paths);

int copy_file(char *Src, char *Dest);

//移除目录下所有文件
void SIMPLE_LIBRARY_API remove_dir_all_files(const char* file_dir);

//递归移除文件路径 使用该API确保路径下已经没有文件 只有文件夹
SIMPLE_LIBRARY_API void remove_directory_all(const char* file_dir);

void SIMPLE_LIBRARY_API find_files(char const *in_path, def_c_paths *str,bool b_recursion,bool b_include_folder);

bool is_file_exists(char const* filename);

bool create_file(char const *filename);
 
//创建路径
SIMPLE_LIBRARY_API bool create_file_directory(char const *in_path);

//打开地址
bool open_url(const char* url);

//通过参数打开url
bool open_url_by_param(const char* url, const char* param);

//通过操作打开某个东西
bool open_by_operation(const char* in_operation, const char* url, const char* param);

//打开一个文件夹
bool open_explore(const char* url);

//使用该接口 一定要初始化buf
SIMPLE_LIBRARY_API bool get_file_buf(const char *path,char *buf);

bool save_file_buff(const char* path, char* buf);

bool add_file_buf(const char *path, char *buf);

//这个函数是以字符串的方式存储，如果数据中有0 自动截断，建议用二进制存储
bool add_new_file_buf(const char *path, char *buf);

//这个是以二进制方式读取
bool load_data_from_disk(const char* path, char* buf);

SIMPLE_LIBRARY_API unsigned int get_file_size_by_filename(const char *filename);

unsigned int get_file_size(FILE *file_handle);

//这个是以二进制方式存储，不会遇到像0自动截断的情况
bool save_data_to_disk(const char* path, char* buf, size_t buf_size);
//宽字符和窄字符
// 
//宽字符转窄字符
SIMPLE_LIBRARY_API _number_of_successful_conversions(size_t) wchar_t_to_char(
	_out_pram(char*) dst_char,
	size_t char_size,
	_in_pram(wchar_t const*) _Src);

//窄字符转宽字符
SIMPLE_LIBRARY_API _number_of_successful_conversions(size_t) char_to_wchar_t(
	_out_pram(wchar_t*) dst_wchar_t,
	size_t wchar_t_size,
	_in_pram(char const*) _Src);

//宽字符
//////////////////////////////////////////////
//这个函数是以字符串的方式存储，如果数据中有0 自动截断，建议用二进制存储
bool add_new_file_buf_w(const wchar_t* path, char* buf);

bool get_file_buf_w(const wchar_t* path, char* buf);

//这个是以二进制方式存储，不会遇到像0自动截断的情况
bool save_data_to_disk_w(const wchar_t* path, char* buf,int buf_size);

//这个是以二进制方式读取 buf的大小要比实际大小+1 因为最后一位留给/0
bool load_data_from_disk_w(const wchar_t* path, char* buf);

bool is_file_exists_w(const wchar_t *filename);

//打开地址
bool open_url_w(const wchar_t* url);

bool open_url_by_param_w(const wchar_t* url,const wchar_t *param);

bool open_by_operation_w(const wchar_t *in_operation, const wchar_t* url, const wchar_t* param);

//打开一个文件夹
bool open_explore_w(const wchar_t* url);

unsigned int get_file_size_by_filename_w(const wchar_t* filename);

void SIMPLE_LIBRARY_API init_def_c_paths_v2(def_c_paths_v2* c_paths);
void init_def_c_paths_w_v2(def_c_paths_w_v2* c_paths);

void SIMPLE_LIBRARY_API find_files_v2(char const* in_path, def_c_paths_v2* str, bool b_recursion, bool b_include_folder);

int SIMPLE_LIBRARY_API get_def_c_offset(const char* str);
int get_def_c_offset_w(const wchar_t* str);

void add_def_c_paths(def_c_paths_v2* c_paths,const char *str);
void add_def_c_paths_w(def_c_paths_w_v2* c_paths, const wchar_t* str);

int get_def_c_paths_offset_by_index(def_c_paths_v2* c_paths, int index);
int get_def_c_paths_offset_by_index_w(def_c_paths_w_v2* c_paths, int index);

SIMPLE_LIBRARY_API char* get_def_c_paths_by_offset(def_c_paths_v2* c_paths, int in_offset);
wchar_t* get_def_c_paths_by_offset_w(def_c_paths_w_v2* c_paths, int in_offset);

char* get_def_c_paths_by_index(def_c_paths_v2* c_paths,int index);
wchar_t *get_def_c_paths_by_index_w(def_c_paths_w_v2* c_paths, int index);

void SIMPLE_LIBRARY_API destroy_def_c_paths_v2(def_c_paths_v2* c_paths);
void destroy_def_c_paths_w_v2(def_c_paths_w_v2* c_paths);
_CRT_END_C_HEADER