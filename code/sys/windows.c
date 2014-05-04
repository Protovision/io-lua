#ifdef _WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>

void	sys_copy(const char *from, const char *to)
{
	DWORD attr;
	const char *p;

	int i;
	static char cmd[1024];
	static char src[256];
	static char dst[256];

	for (i = 0; from[i] != 0; ++i) {
		if (from[i] == '/') src[i] = '\\';
		else src[i] = from[i];
	}
	src[i] = 0;

	attr = GetFileAttributesA(from);
	if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		p = to + strlen(to) - 1;
		if (*p == '\\') {
			sprintf(dst, "%s%s", to, strrchr(src, '\\')+1);
		} else {
			sprintf(dst, "%s%s", to, strrchr(src, '\\'));
		}
		sprintf(cmd, "@md \"%s\" >nul", dst);
		system(cmd);
		sprintf(cmd, "@robocopy \"%s\" \"%s\" /E >nul", src, dst);
	} else {
		sprintf(cmd, "@copy \"%s\" \"%s\" >nul", src, to);
	}
	system(cmd);
}

int	sys_exists(const char *file)
{
	static char path[256];
	int i;

	for (i = 0; file[i] != 0; ++i) {
		if (file[i] == '/') path[i] = '\\';
		else path[i] = file[i];
	}
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

#endif
