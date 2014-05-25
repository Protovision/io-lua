#include "common.h"
#include "video/video.h"
#include "audio/audio.h"

void	trap_args(lua_State *s, const char *funcname, const char *fmt, ...)
{
	int i;
	va_list v;
	const char *where;
	
	va_start(v, fmt);

	for (i = 1; *fmt; ++fmt, ++i) {
		if (lua_isnil(s, i)) break;
		switch (*fmt) {
		case 'i':
			*va_arg(v, int*) = lua_tointeger(s, i);
			break;
		case 'u':
			*va_arg(v, unsigned int*) = lua_tounsigned(s, i);
			break;
		case 'n':
			*va_arg(v, double*) = lua_tonumber(s, i);
			break;
		case 's':
			*va_arg(v, const char**) = lua_tostring(s, i);
			break;
		case 'p':
			*va_arg(v, void**) = lua_touserdata(s, i);
			break;
		case 'b':
			*va_arg(v, int*) = lua_toboolean(s, i);
			break;
		}
	}
	if (*fmt) {
		luaL_where(s, 1);
		where = lua_tostring(s, -1);
		FATAL("%s Invalid call to %s", where, funcname);
	}
	va_end(v);	
}

int	trap_Get(lua_State *s)
{
	const char *varname;
	var_t *var;

	trap_args(s, "Get", "s", &varname);
	var = var_get(varname);
	if (var == NULL) {
		lua_pushnil(s);
		return 1;
	}
	lua_pushstring(s, var->string);
	return 1;	
}

int	trap_Set(lua_State *s)
{
	const char *varname, *value;
	
	trap_args(s, "Set", "ss", &varname, &value);
	var_set(varname, value);
	return 0;	
}

int	trap_LoadFont(lua_State *s)
{
	FONT *font;
	const char *fontname;
	int fontsize;

	trap_args(s, "LoadFont", "si", &fontname, &fontsize);
	font = font_load(gamepath(fontname), fontsize);
	if (font == NULL) {
		lua_pushnil(s);
		return 1;
	}
	lua_pushlightuserdata(s, font);
	return 1;
}

int	trap_LoadImage(lua_State *s)
{
	IMAGE *image;
	const char *imgfile;
	
	trap_args(s, "LoadImage", "s", &imgfile);
	image = image_load(gamepath(imgfile));
	if (image == NULL) {
		lua_pushnil(s);
		return 1;
	}
	lua_pushlightuserdata(s, image);
	return 1;
}

int	trap_DrawText(lua_State *s)
{
	int x, y;
	unsigned int color;
	const char *text;
	FONT *font;

	trap_args(s, "DrawText", "iisip", &x, &y, &text, &color, &font);

	if (color == 0) {
		sscanf(c_fgcolor->string, "%x", &color);
	}
	if (font == NULL) {
		font = (FONT*)script_import_pointer(c_fontfamily->string);
	}

	video_drawText(x, y, text, font, color);
	return 0;
}

int	trap_DrawBackground(lua_State *s)
{
	IMAGE *image;
	
	trap_args(s, "DrawBackground", "p", &image);
	video_drawBackground(image);
	return 0;
}

int	trap_Quit(lua_State *s)
{
	quit(0);
	return 0;
}

int	trap_FreeFont(lua_State *s)
{
	FONT *f;

	trap_args(s, "FreeFont", "p", &f);
	font_free(f);
	return 0;	
}

int	trap_FreeImage(lua_State *s)
{
	IMAGE *image;

	trap_args(s, "FreeImage", "p", &image);
	image_free(image);
	return 0;
}

int	trap_DrawImage(lua_State *s)
{
	IMAGE *image;
	int x, y, w, h;
	
	trap_args(s, "DrawImage", "iipii", &x, &y, &image, &w, &h);
	video_drawImage(x, y, image, w, h);
	return 0;
}

int	trap_SetColor(lua_State *s)
{
	unsigned long int color;

	trap_args(s, "SetColor", "u", &color);
	video_setColor(color);
	return 0;
}

int	trap_Clear(lua_State *s)
{
	video_clear();
	return 0;
}

int	trap_DrawLine(lua_State *s)
{
	int x1, y1, x2, y2;
	
	trap_args(s, "DrawLine", "iiii", &x1, &y1, &x2, &y2);
	video_drawLine(x1, y1, x2, y2);
	return 0;
}

int	trap_DrawRect(lua_State *s)
{
	int x, y, h, w;

	trap_args(s, "DrawRect", "iiii", &x, &y, &w, &h);
	video_drawRect(x, y, w, h);
	return 0;
}

int	trap_FillRect(lua_State *s)
{
	int x, y, h, w;
	
	trap_args(s, "FillRect", "iiii", &x, &y, &w, &h);
	video_fillRect(x, y, w, h);
	return 0;
}

int	trap_GetPlatform(lua_State *s)
{
	const char *platform;

	platform = SDL_GetPlatform();
	lua_pushstring(s, platform);
	return 1;
}

int	trap_OpenFile(lua_State *s)
{
	const char *file, *mode, *fullpath;
	FILE *fp;
	
	trap_args(s, "OpenFile", "ss", &file, &mode);
	fullpath = datapath(file);
	fp = fopen(fullpath, mode);
	if (fp == NULL) FATAL("Failed to open file: %s", fullpath);
	lua_pushlightuserdata(s, fp);
	return 1;
}

int	trap_ReadFile(lua_State *s)
{
	FILE *f;
	size_t n;
	static char line[MAX_STRING];

	trap_args(s, "ReadFile", "p", &f);
	line[0] = 0;
	fgets(line, MAX_STRING, f);
	n = strlen(line);
	if (n) {
		--n;
		if (line[n] == '\n' || line[n] == '\r') line[n] = 0;
		if (n != 0 && line[--n] == '\r') line[n] = 0;
	}
	lua_pushstring(s, line);
	return 1;	
}

int	trap_WriteFile(lua_State *s)
{
	FILE *f;
	const char *string;

	trap_args(s, "WriteFile", "ps", &f, &string);
	fputs(string, f);
	return 0;
}

int	trap_SeekFile(lua_State *s)
{
	FILE *f;
	int whence, whence2;
	long offset;

	trap_args(s, "SeekFile", "pii", &f, &offset, &whence);
	switch (whence) {
	case FILESEEK_SET:	whence2 = SEEK_SET; break;
	case FILESEEK_CUR:	whence2 = SEEK_CUR; break;
	case FILESEEK_END:	whence2 = SEEK_END; break;
	}

	if (fseek(f, offset, whence2) < 0) {
		FATAL("Failed to seek file")
	}
	lua_pushinteger(s, ftell(f));
	return 1;		
}

int	trap_TellFile(lua_State *s)
{
	FILE *f;

	trap_args(s, "TellFile", "p", &f);
	lua_pushinteger(s, ftell(f));
	return 1;
}

int	trap_CloseFile(lua_State *s)
{
	FILE *f;
	
	trap_args(s, "CloseFile", "p", &f);
	fclose(f);
	return 0;
}

int	trap_RenameFile(lua_State *s)
{
	const char *oldname, *newname;

	trap_args(s, "RenameFile", "ss", &oldname, &newname);
	oldname = datapath(oldname);
	newname = datapath(newname);
	if (rename(oldname, newname) < 0) {
		FATAL("Failed to rename file from \"%s\" to \"%s\"", oldname, newname);
	}
	return 0;
}

int	trap_RemoveFile(lua_State *s)
{
	const char *file;

	trap_args(s, "RemoveFile", "s", &file);
	file = datapath(file);
	if (remove(file) < 0) {
		FATAL("Failed to remove file: %s", file);
	}
	return 0;
}

int	trap_MakeDirectory(lua_State *s)
{
	const char *dir;

	trap_args(s, "MakeDirectory", "s", &dir);
	dir = datapath(dir);
	if (MKDIR(dir)) {
		FATAL("Failed to make directory: %s", dir);
	}
	return 0;
}

int	trap_RemoveDirectory(lua_State *s)
{
	const char *dir;

	trap_args(s, "RemoveDirectory", "s", &dir);
	dir = datapath(dir);
	if (RMDIR(dir) < 0) {
		FATAL("Failed to remove directory: %s", dir);
	}
	return 0;
}

int	trap_DrawPoint(lua_State *s)
{
	int x, y;
	
	trap_args(s, "DrawPoint", "ii", &x, &y);
	video_drawPoint(x, y);
	return 0;
}

int	trap_GetWindowSize(lua_State *s)
{
	int x, y;

	video_getWindowSize(&x, &y);
	lua_pushinteger(s, x);
	lua_pushinteger(s, y);
	return 2;			
}

int	trap_LoadSound(lua_State *s)
{
	const char *filename;
	sound_t *w;

	trap_args(s, "LoadSound", "s", &filename);
	w = sound_load(gamepath(filename));
	lua_pushlightuserdata(s, w);
	return 1;
}

int	trap_PlaySound(lua_State *s)
{
	sound_t *w;
	channel_t *c;	

	trap_args(s, "PlaySound", "p", &w);
	c = sound_play(w, 0);
	lua_pushlightuserdata(s, c);
	return 1;
}

int	trap_LoopSound(lua_State *s)
{
	sound_t *w;
	channel_t *c;

	trap_args(s, "LoopSound", "p", &w);
	c = sound_play(w, 1);
	lua_pushlightuserdata(s, c);
	return 1;
}

int	trap_FreeSound(lua_State *s)
{
	sound_t *w;
	
	trap_args(s, "FreeSound", "p", &w);
	sound_free(w);
	return 0;
}

int	trap_PauseSound(lua_State *s)
{
	channel_t *c;

	trap_args(s, "PauseSound", "p", &c);
	sound_pause(c);
	return 0;
}

int	trap_ResumeSound(lua_State *s)
{
	channel_t *c;

	trap_args(s, "ResumeSound", "p", &c);
	sound_resume(c);
	return 0;
}

int	trap_StopSound(lua_State *s)
{
	channel_t *c;

	trap_args(s, "StopSound", "p", &c);
	sound_stop(c);
	return 0;
}

int	trap_GetTicks(lua_State *s)
{
	lua_pushinteger(s, SDL_GetTicks());
	return 1;
}

char	*load_file(const char *path)
{
	FILE *f;
	static char data[MAX_FILE];
	size_t n;

	f = fopen(path, "rb");
	if (f == NULL) FATAL("Could not open file: %s", path);
	n = fread(data, 1, MAX_FILE, f);
	fclose(f);

	data[n] = 0;
	return data;
}

int	trap_ReadGameFile(lua_State *s)
{
	const char *path;

	trap_args(s, "ReadGameFile", "s", &path);
	lua_pushstring(s, load_file(gamepath(path)));
	return 1;
}

int	trap_ReadDataFile(lua_State *s)
{
	const char *path;
		
	trap_args(s, "ReadDataFile", "s", &path);
	lua_pushstring(s, load_file(datapath(path)));
	return 1;
}

int	trap_MessageBox(lua_State *s)
{
	const char *msg;
	extern SDL_Window *v_window;

	trap_args(s, "MessageBox", "s", &msg);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "", msg, v_window);
	return 0;
}

int	trap_PauseAudio(lua_State *s)
{
	audio_pause();
	return 0;
}

int	trap_ResumeAudio(lua_State *s)
{
	audio_resume();
	return 0;
}

int	trap_StopAudio(lua_State *s)
{
	audio_stop();
	return 0;
}

int	trap_SetVolume(lua_State *s)
{
	int vol;

	trap_args(s, "SetVolume", "i", &vol);
	audio_set_volume(vol);
	return 0;
}

int	trap_GetVolume(lua_State *s)
{
	lua_pushinteger(s, audio_get_volume());
	return 1;
}

int	trap_MuteAudio(lua_State *s)
{
	audio_mute();
	return 0;
}

int	trap_UnmuteAudio(lua_State *s)
{
	audio_unmute();
	return 0;
}

int	trap_WriteDataFile(lua_State *s)
{
	FILE *f;
	int len;
	const char *path, *data, *fullpath;

	trap_args(s, "WriteDataFile", "ssi", &path, &data, &len);

	fullpath = datapath(path);
	f = fopen(fullpath, "wb");
	if (f == NULL) FATAL("Failed to write to data file: %s", fullpath);
	if (len == 0) len = strlen(data);
	fwrite(data, 1, len, f);
	fclose(f);

	return 0;
}

int	trap_CheckDataFile(lua_State *s)
{
	const char *path;

	trap_args(s, "CheckDataFile", "s", &path);
	lua_pushboolean(s, sys_exists(datapath(path)));
	return 1;
}

int	trap_Gamepath(lua_State *s)
{
	const char *path;

	trap_args(s, "Gamepath", "s", &path);
	lua_pushstring(s, gamepath(path));
	return 1;
}

int	trap_Datapath(lua_State *s)
{
	const char *path;

	trap_args(s, "Datapath", "s", &path);
	lua_pushstring(s, datapath(path));
	return 1;
}

int	trap_AppendDataFile(lua_State *s)
{
	FILE *f;
	int len;
	const char *path, *fullpath, *data;

	trap_args(s, "AppendDataFile", "ssi", &path, &data, &len);
	fullpath = datapath(path);
	f = fopen(fullpath, "ab");
	if (f == NULL) FATAL("Failed to append to data file: %s", fullpath);
	if (len == 0) len = strlen(data);
	fwrite(data, 1, len, f);
	fclose(f);

	return 0;
}

int	trap_IsFile(lua_State *s)
{
	const char *path;

	trap_args(s, "IsFile", "s", &path);
	lua_pushboolean(s, sys_isfile(PATH(path)));
	return 1;
}

int	trap_IsDirectory(lua_State *s)
{
	const char *path;

	trap_args(s, "IsDirectory", "s", &path);
	lua_pushboolean(s, sys_isdir(PATH(path)));
	return 1;
}

int	trap_ReadDirectory(lua_State *s)
{
	directory_t *dir;
	direntry_t *ent;
	const char *path;
	int i;

	trap_args(s, "ReadDirectory", "s", &path);

	path = PATH(path);
	dir = OPENDIR(path);
	if (dir == NULL) FATAL("Failed to open directory: %s", path);

	lua_newtable(s);
	i = 1;
	for (ent = READDIR(dir); ent != NULL; ent = READDIR(dir)) {
		path = DIRENT_NAME(ent);
		if (strcmp(path, ".") == 0) continue;
		if (strcmp(path, "..") == 0) continue;

		lua_pushstring(s, va("%d", i));
		lua_pushstring(s, DIRENT_NAME(ent));
		lua_settable(s, -3);	
		++i;
	}
	CLOSEDIR(dir);

	return 1;
}

typedef struct {
	const char *name;
	int (*func)(lua_State*);
} trap_t;

trap_t syscalls[] = {
	{ "Get", trap_Get },
	{ "Set", trap_Set },
	{ "GetPlatform", trap_GetPlatform },

	{ "LoadFont", trap_LoadFont },
	{ "LoadImage", trap_LoadImage },
	{ "DrawText", trap_DrawText },
	{ "DrawBackground", trap_DrawBackground },
	{ "DrawImage", trap_DrawImage },
	{ "Quit", trap_Quit },
	{ "FreeFont",	trap_FreeFont },
	{ "FreeImage", trap_FreeImage },
	{ "SetColor", trap_SetColor },
	{ "Clear", trap_Clear },
	{ "DrawLine", trap_DrawLine },
	{ "DrawRect", trap_DrawRect },
	{ "FillRect", trap_FillRect },
	{ "DrawPoint", trap_DrawPoint },

	{ "OpenFile", trap_OpenFile },
	{ "ReadFile", trap_ReadFile },
	{ "WriteFile", trap_WriteFile },
	{ "SeekFile", trap_SeekFile },
	{ "CloseFile", trap_CloseFile },
	{ "RenameFile", trap_RenameFile },
	{ "RemoveFile", trap_RemoveFile },
	{ "MakeDirectory", trap_MakeDirectory },
	{ "RemoveDirectory", trap_RemoveDirectory },

	{ "GetWindowSize", trap_GetWindowSize },

	{ "LoadSound", trap_LoadSound },
	{ "PlaySound", trap_PlaySound },
	{ "LoopSound", trap_LoopSound },
	{ "FreeSound", trap_FreeSound },
	{ "PauseSound", trap_PauseSound },
	{ "ResumeSound", trap_ResumeSound },
	{ "StopSound", trap_StopSound },

	{ "GetTicks", trap_GetTicks },

	{ "ReadGameFile", trap_ReadGameFile },
	{ "ReadDataFile", trap_ReadDataFile },
	
	{ "MessageBox", trap_MessageBox },

	{ "PauseAudio", trap_PauseAudio },
	{ "ResumeAudio", trap_ResumeAudio },
	{ "StopAudio", trap_StopAudio },

	{ "GetVolume", trap_GetVolume },
	{ "SetVolume", trap_SetVolume },

	{ "MuteAudio", trap_MuteAudio },
	{ "UnmuteAudio", trap_UnmuteAudio },

	{ "WriteDataFile", trap_WriteDataFile },

	{ "TellFile", trap_TellFile },

	{ "CheckDataFile", trap_CheckDataFile },
	{ "Gamepath", trap_Gamepath },
	{ "Datapath", trap_Datapath },
	{ "AppendDataFile", trap_AppendDataFile },
	{ "IsFile", trap_IsFile },
	{ "IsDirectory", trap_IsDirectory },
	{ "ReadDirectory", trap_ReadDirectory },
	
	{ NULL, NULL }
};

void	trap_init()
{
	trap_t *trap;
	for (trap = syscalls; trap->name != NULL; ++trap) {
		script_register(trap->name, trap->func);
	}
}

void	trap_shutdown() {}


