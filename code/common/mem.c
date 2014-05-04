#include "common.h"

static void	*smallpool, *bigpool;
static void	*smallpool_end, *bigpool_end;
static int	smallpool_chunks, smallpool_size, bigpool_chunks, bigpool_size;

void	mem_init(
	int s_chunks,
	int s_size,
	int b_chunks,
	int b_size)
{
	smallpool_chunks = s_chunks;
	smallpool_size = s_size;
	bigpool_chunks = b_chunks;
	bigpool_size = b_size;
	smallpool = calloc(smallpool_chunks, smallpool_size);
	smallpool_end = (char*)smallpool + (smallpool_chunks*smallpool_size);
	bigpool = calloc(bigpool_chunks, bigpool_size);
	bigpool_end = (char*)bigpool + (bigpool_chunks*bigpool_size);
}

void	mem_shutdown()
{
	if (smallpool) {
		free(smallpool);
		smallpool = NULL;
	}
	if (bigpool) {
		free(bigpool);
		bigpool = NULL;
	}
}

void	*mem_real_alloc(size_t size, void *pool, void *pool_end, size_t chunksize)
{
	size_t *p;
	for (p = (size_t*)pool; *p; p = (size_t*)((char*)p+chunksize)) {
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
	if (size+sizeof(size_t) < smallpool_size) {
		return mem_real_alloc(size, smallpool, smallpool_end, smallpool_size);
	}
	ptr = mem_real_alloc(size, bigpool, bigpool_end, bigpool_size);
	if (ptr == NULL) {
		ERROR("Failed to allocate %zd bytes", size);
	}
	return ptr;
}

void	*mem_realloc(void *ptr, size_t size)
{
	void *newptr;
	if (ptr == NULL) return mem_alloc(size);
	if ((char*)ptr - (char*)smallpool < smallpool_chunks*smallpool_size) {
		if (size+sizeof(size_t) < smallpool_size) {
			((size_t*)ptr)[-1] = size;
			return ptr;
		} else {
			newptr = mem_alloc(size);
			if (newptr == NULL) {
				ERROR("Failed to allocate %zd bytes", size);
			}
			memcpy(newptr, ptr, ((size_t*)ptr)[-1]);
			return newptr;
		}	
	} else if ((char*)ptr - (char*)bigpool < bigpool_chunks*bigpool_size) {
		if (size+sizeof(size_t) < bigpool_size) {
			((size_t*)ptr)[-1] = size;
			return ptr;
		} else {
			ERROR("Failed to allocate %zd", size);
		}
	}
	ERROR("Failed to allocate %zd bytes", size);
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


