#ifndef POOL_H
#define POOL_H

#include "common.h"

struct block_header {
	size_t	size;
	struct block_header *next;
	char	data[0];
};

#define _POOL_BLKHDR(P)		((struct block_header*)((char*)(P) - sizeof(struct block_header)))

static char			*pool, *pool_end;
static struct block_header	*freeblock;
static size_t blksize;

void	pool_init(size_t count, size_t size)
{
	pool = (char*)calloc(count, size);
	if (pool == NULL)
		FATAL("Failed to initialize memory pool.");		
	pool_end = pool + count * size;
	blksize = size;
	freeblock = (struct block_header*)pool;
}

void	pool_shutdown()
{
	if (pool) {
		free(pool);
		pool = NULL;
	}
}

void	*pool_alloc(size_t size)
{
	struct block_header *blk;

	if (size > blksize - sizeof(struct block_header))
		FATAL("Memory pool block capacity exceeded.");

	blk = freeblock;
	if ((char*)blk == pool_end) 
		FATAL("Memory pool capacity exceeded.");

	if (blk->next == NULL) {
		freeblock = (struct block_header*)((char*)blk + blksize);
	} else {
		freeblock = blk->next;
	}

	blk->size = size;	
	return blk->data;
}

void	*pool_realloc(void *ptr, size_t size)
{
	if (ptr == NULL) return pool_alloc(size);

	if (size > blksize - sizeof(struct block_header))
		FATAL("Memory pool block capacity exceeded.");

	_POOL_BLKHDR(ptr)->size = size;
	return ptr;
}

void	pool_free(void *ptr)
{
	struct block_header *b;

	if (ptr == NULL) return;	
	b = _POOL_BLKHDR(ptr);
	b->size = 0;
	b->next = freeblock;	
	freeblock = b;
}

#endif
