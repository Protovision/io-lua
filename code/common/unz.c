#include "common.h"
#include "unzip.h"

static unzFile	basezip, gamezip;

void	unz_init()
{
	basezip = unzOpen(c_basezip->string);
	if (basezip == NULL)
		FATAL("Failed to open zip file: %s", c_basezip->string);

	gamezip = unzOpen(c_gamezip->string);
	if (gamezip == NULL)	
		FATAL("Failed to open zip file: %s", c_gamezip->string);

}

void	unz_shutdown()
{
	unzClose(gamezip);
	unzClose(basezip);
}

static	unzFile	foundzip;

int	unz_exists(const char *filename)
{
	if (unzLocateFile(gamezip, filename, 1) == UNZ_OK) {
		foundzip = gamezip;
		return 1;
	} else if (unzLocateFile(basezip, filename, 1) == UNZ_OK) {
		foundzip = basezip;
		return 1;
	}
	return 0;
}

static void	unz_extract(void **ptr, size_t *size, const char *filename)
{
	unz_file_info info;
	char *data, *data_end;
	int got;

	if (!unz_exists(filename))
		FATAL("Failed to locate packaged file: %s", filename);

	if (unzOpenCurrentFile(foundzip) != UNZ_OK)
		FATAL("Failed to unpack %s", filename);

	unzGetCurrentFileInfo(foundzip, &info, NULL, 0, NULL, 0, NULL, 0);
	data = (char*)malloc(info.uncompressed_size+1);
	*ptr = data;
	*size = info.uncompressed_size;
	data_end = data + info.uncompressed_size;
	*data_end = 0;

	while ((got = unzReadCurrentFile(foundzip, data, data_end - data)) > 0) {
		data = data + got;	
	}
	unzCloseCurrentFile(foundzip);
}

static	int unz_close(SDL_RWops *ops)
{
	free(ops->hidden.mem.base);
	SDL_FreeRW(ops);
	return 0;
}

SDL_RWops *unz_open(const char *filename)
{
	void *data;
	size_t size;
	SDL_RWops *ops;
	
	unz_extract(&data, &size, filename);
	ops = SDL_RWFromMem(data, size);
	if (ops) ops->close = unz_close;
	return ops;	
}

char	*unz_load(const char *filename)
{
	void *data;
	size_t size;
	
	unz_extract(&data, &size, filename);
	return (char*)data;
}

