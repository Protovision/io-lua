#ifdef __APPLE__

#include "common.h"

#include <TargetConditionals.h>

#include <copyfile.h>

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
	CFURLGetFileSystemRepresentation(url, TRUE, (UInt8*)src, 256);

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

int	sys_isfile(const char *path)
{
	struct stat st;

	if (stat(path, &st)) return 0;
	return (st.st_mode & S_IFREG);
}

int	sys_isdir(const char *path)
{
	struct stat st;

	if (stat(path, &st)) return 0;
	return (st.st_mode & S_IFDIR);
}

#endif
