#include "common.h"

#define MAX_SMALLVAR	512
#define	MAX_SMALLPOOL	2048
#define MAX_BIGVAR	(1024*1024)
#define MAX_BIGPOOL	63

static char	smallpool[MAX_SMALLVAR*MAX_SMALLPOOL];
static char	bigpool[MAX_BIGVAR*MAX_BIGPOOL];

void	mem_init()
{
	memset(smallpool, 0, sizeof(smallpool));
	memset(bigpool, 0, sizeof(bigpool));
}

void	mem_shutdown() {}

void	*mem_pool_alloc(size_t size, void *pool, size_t poolsize, size_t varsize)
{
	size_t *p;
	size_t *pool_end;

	pool_end = (size_t*)((char*)pool + poolsize*varsize);
	for (p = (size_t*)pool; *p; p = (size_t*)((char*)p+varsize)) {
		if (p == (size_t*)pool_end) {
			return NULL;
		}
	}
	*p = size;
	return ++p;
}

void	*mem_alloc(size_t size)
{
	void *ptr;

	if (size+sizeof(size_t) < MAX_SMALLVAR) {
		ptr = mem_pool_alloc(size, smallpool, MAX_SMALLPOOL, MAX_SMALLVAR);
	} else {
		ptr = mem_pool_alloc(size, bigpool, MAX_BIGPOOL, MAX_BIGVAR);
	}

	if (ptr == NULL) {
		ERROR("Out of game memory", size);
	}
	return ptr;
}

void	*mem_realloc(void *ptr, size_t size)
{
	void *newptr;

	if (ptr == NULL) return mem_alloc(size);

	if ((char*)ptr - smallpool < MAX_SMALLPOOL*MAX_SMALLVAR) {
		if (size+sizeof(size_t) < MAX_SMALLVAR) {
			((size_t*)ptr)[-1] = size;
			return ptr;
		} else {
			newptr = mem_alloc(size);
			memcpy(newptr, ptr, ((size_t*)ptr)[-1]);
			return newptr;
		}	
	} else if ((char*)ptr - bigpool < MAX_BIGPOOL*MAX_BIGVAR) {
		if (size+sizeof(size_t) < MAX_BIGVAR) {
			((size_t*)ptr)[-1] = size;
			return ptr;
		} else {
			ERROR("Out of game memory", size);
		}
	}

	return NULL;
}

char	*mem_strdup(const char *s)
{
	char *p;
	p = (char*)mem_alloc(strlen(s)+1);
	strcpy(p, s);
	return p;
}

void	mem_free(void *ptr)
{
	if (ptr == NULL) return;
	((size_t*)ptr)[-1] = 0;
}


