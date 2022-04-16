#include "FilesWork.h"

char** get_files_names(int* files_count)
{
	char** files_names = (char**)malloc(sizeof(char*)), ** fn;
	*files_count = 0;

	WIN32_FIND_DATA f;
	HANDLE hf = FindFirstFile("./*", &f);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			++* files_count;
			fn = (char**)realloc(files_names, sizeof(char*) * *files_count);
			if (!fn)
			{
				fprintf_s(stderr, "Ошибка выделения памяти под названия файлов.\n");
				return;
			}
			files_names = fn;
			files_names[*files_count - 1] = (char*)malloc(sizeof(char) * FILENAME_MAX);
			if (files_names[*files_count - 1])
				strcpy_s(files_names[*files_count - 1], FILENAME_MAX * sizeof(char), f.cFileName);
			else
			{
				fprintf_s(stderr, "Ошибка выделения памяти под название файла.\n");
				return NULL;
			}
			//files_names[files_count-1] = f.cFileName;
		} while (FindNextFile(hf, &f));
	}
	else
	{
		fprintf_s(stderr, "Неверная директория.\n");
		return NULL;
	}
	return files_names;
}