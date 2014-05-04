#ifdef __APPLE__

#include <TargetConditionals.h>

#include <unistd.h>
#include <copyfile.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if  TARGET_OS_IPHONE == 1

#include <MobileCoreServices/MobileCoreServices.h>

void	sys_copy(const char *from, const char *to)
{
	struct stat st;
	const char *p;
	static char src[256];
	static char dst[256];
	CFBundleRef bundle;
	CFURLRef url;

	bundle = CFBundleGetMainBundle();
	url = CFBundleCopyBundleURL(bundle);
	CFURLGetFileSystemRepresentation(url, TRUE, (Uint8*)src, 256);

	sprintf(src+strlen(src), "/%s", from);
    
	p = to + strlen(to) - 1;
	if (*p == '/') {
		sprintf(dst, "%s%s", to, strrchr(src, '/')+1);
	} else {
		sprintf(dst, "%s%s", to, strrchr(src, '/'));
	}
    
	stat(src, &st);
	if (st.st_mode & S_IFDIR) {
		copyfile(src, dst, NULL, COPYFILE_RECURSIVE|COPYFILE_ALL);
		return;
	}
	
	copyfile(src, dst, NULL, COPYFILE_ALL);
    
}

#else

void	sys_copy(const char *from, const char *to)
{
	struct stat st;
	const char *p;
	static char dst[256];

	p = to + strlen(to) - 1;
	if (*p == '/') {
	sprintf(dst, "%s%s", to, strrchr(from, '/')+1);
	} else {
	sprintf(dst, "%s%s", to, strrchr(from, '/'));
	}
	stat(from, &st);
	if (st.st_mode & S_IFDIR) {
		copyfile(from, dst, NULL, COPYFILE_RECURSIVE|COPYFILE_ALL);
		return;
	}
	copyfile(from, dst, NULL, COPYFILE_ALL);
}

#endif

int	sys_exists(const char *path)
{
	struct stat st;

	return stat(path, &st) == 0;
}

#endif
