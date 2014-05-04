#ifdef _WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>

void	sys_copy(const char *from, const char *to)
{
	DWORD attr;
	const char *p;
	static char cmd[1024];
	static char dst[256];
	
	attr = GetFileAttributesA(from);
	if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		p = to + strlen(to) - 1;
		if (*p == '\\') {
			sprintf(dst, "%s%s", to, strrchr(from, '\\')+1);
		} else {
			sprintf(dst, "%s%s", to, strrchr(from, '\\'));
		}
		sprintf(cmd, "@md \"%s\" >nul", dst);
		system(cmd);
		sprintf(cmd, "@robocopy \"%s\" \"%s\" /E >nul", from, dst);
	} else {
		sprintf(cmd, "@copy \"%s\" \"%s\" >nul", from, to);
	}
	system(cmd);
}

int	sys_exists(const char *file)
{
	return GetFileAttributesA(file) != INVALID_FILE_ATTRIBUTES;
}

#endif
