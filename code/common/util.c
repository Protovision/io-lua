#include "common.h"

char	*va(const char *fmt, ...)
{
	va_list v;
	static int index = 0;
	static char strings[2][MAX_STRING];
	char *buf;
		
	buf = strings[index & 1];
	++index;
	
	va_start(v, fmt);
	vsnprintf(buf, MAX_STRING, fmt, v);
	va_end(v);

	return buf;
}

char	*gamepath(const char *p)
{
	char last;
	char *s;
	static int index = 0;
	static char buf[4][MAX_PATH];
	char *path;

	path = buf[index];
	if (index == 3) index = 0;
	else ++index;

	last = 0;
#ifdef _WINDOWS
	sprintf(path, "%s%s\\", base_path, c_game->string);
#else
	sprintf(path, "%s%s/", base_path, c_game->string);
#endif
	for (s = path + strlen(path); *p; ++p, ++s) {
		if (last == '.' && *p == '.') {
			--s;
			continue;
		}
#ifdef _WINDOWS
		if (*p == '/') last = *s = '\\';
		else
#endif
		last = *s = *p;
	}

	*s = 0;
	return path;
		
}

char	*basepath(const char *p)
{
	char last;
	char *s;
	static int index = 0;
	static char buf[2][MAX_PATH];
	char *path;

	path = buf[index & 1];
	++index;

	last = 0;

	strcpy(path, base_path);

	for (s = path + strlen(path); *p; ++p, ++s) {
		if (last == '.' && *p == '.') {
			--s;
			continue;
		}
#ifdef _WINDOWS
		if (*p == '/') last = *s = '\\';
		else
#endif
		last = *s = *p;
	}

	*s = 0;
	return path;
		
}

