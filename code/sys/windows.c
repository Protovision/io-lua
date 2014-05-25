#ifdef _WINDOWS

#include "common/common.h"

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

char	*convert_path(const char *path)
{
	static char realpath[MAX_PATHNAME];
	int i;

	for (i = 0; path[i] != 0; ++i) {
		if (path[i] == '/') realpath[i] = '\\';
		else realpath[i] = path[i];
	}
	realpath[i] = 0;
	return realpath;	
}

int	sys_exists(const char *file)
{
	return GetFileAttributesA(file) != INVALID_FILE_ATTRIBUTES;
}

int	sys_isfile(const char *file)
{
	return GetFileAttributesA(file) & FILE_ATTRIBUTE_NORMAL;
}

int	sys_isdir(const char *file)
{
	return GetFileAttributesA(file) & FILE_ATTRIBUTE_DIRECTORY;
}

directory_t	*win_opendir(const char *path)
{
	static int i = 0;
	static directory_t dir[2];
	directory_t *d;

	d = &dir[i & 1];
	++i;

	d->dirh = FindFirstFileA(va("%s\\*", path), &d->data);
	if (d->dirh == INVALID_HANDLE_VALUE) return NULL;
	d->first_time = 1;

	return d;
}

direntry_t	*win_readdir(directory_t *dir)
{
	if (dir->first_time) {
		dir->first_time = 0;
		return &dir->data;
	}
	
	if (FindNextFileA(dir->dirh, &dir->data) == 0) {
		return NULL;
	}

	return &dir->data;	
}

void	win_closedir(directory_t *dir)
{
	FindClose(dir->dirh);
}

#endif
