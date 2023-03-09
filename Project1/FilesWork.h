#ifndef _FILESWORK_H_
#define	_FILESWORK_H_
#include <Windows.h>
#include <stdio.h>

WIN32_FIND_DATA* get_files(int* files_count);
char** get_files_names(int count, WIN32_FIND_DATA* datas);

#endif // _FILESWORK_H_