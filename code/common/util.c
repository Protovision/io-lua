#include "common/common.h"

char	*va(const char *fmt, ...)
{
	va_list v;
	char *buf;

	static int index = 0;
	static char strings[2][MAX_STRING];
		
	buf = strings[index & 1];
	++index;
	
	va_start(v, fmt);
	vsnprintf(buf, MAX_STRING, fmt, v);
	va_end(v);

	return buf;
}

char	*pathjoin(const char *base, const char *p)
{
	char last;
	char *s, *path;

	static int index = 0;
	static char buf[2][MAX_PATHNAME];

	path = buf[index & 1];
	++index;

	last = 0;

#ifdef _WINDOWS
	sprintf(path, "%s\\", base);
#else
	sprintf(path, "%s/", base);
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
