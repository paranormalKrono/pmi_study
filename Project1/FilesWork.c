#include "FilesWork.h"

char** get_files_names(int count, WIN32_FIND_DATA* datas)
{
	char** files_names = (char**)malloc(sizeof(char*) * count);

	if (!files_names)
	{
		fprintf_s(stderr, "Ошибка выделения памяти под названия файлов.\n");
		return;
	}

	for (int i = 0; i < count; ++i) 
	{
		files_names[i] = (char*)malloc(sizeof(char) * FILENAME_MAX);
	}

	for (int i = 0; i < count; ++i) 
	{
		if (files_names[i])
		{
			strcpy_s(files_names[i], FILENAME_MAX * sizeof(char), datas[i].cFileName);
		}
		else
		{
			fprintf_s(stderr, "Ошибка выделения памяти под название файла.\n");
			return NULL;
		}
	}
	return files_names;
}

WIN32_FIND_DATA* get_files(int* files_count)
{
	*files_count = 0;

	WIN32_FIND_DATA* files = malloc(sizeof(WIN32_FIND_DATA)), * fn, cur_file;
	HANDLE hf = FindFirstFile("./*", &cur_file);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			++* files_count;
			fn = realloc(files, sizeof(WIN32_FIND_DATA) * *files_count);
			if (!fn)
			{
				fprintf_s(stderr, "Ошибка выделения памяти под информацию о файлах.\n");
				return;
			}
			files = fn;
			files[*files_count - 1] = cur_file;
		} while (FindNextFile(hf, &cur_file));
	}
	else
	{
		fprintf_s(stderr, "Неверная директория.\n");
		return NULL;
	}
	return files;
}