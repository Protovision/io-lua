#include "common.h"

static lua_State	*lua;

void	*script_allocator(void *ud, void *ptr, size_t osize, size_t nsize)
{
	if (nsize == 0) {
		mem_free(ptr);
		return NULL;
	}
	return mem_realloc(ptr, nsize);
}

void	script_init()
{
	lua = lua_newstate(script_allocator, NULL);
	if (lua == NULL) {
		FATAL("Failed to initialize Lua");
	}

	luaL_requiref(lua, "_G", luaopen_base, 1);
	luaL_requiref(lua, "package", luaopen_package, 1);
	luaL_requiref(lua, "table", luaopen_table, 1);
	luaL_requiref(lua, "string", luaopen_string, 1);
	luaL_requiref(lua, "bit32", luaopen_bit32, 1);
	luaL_requiref(lua, "math", luaopen_math, 1);
	luaL_requiref(lua, "debug", luaopen_debug, 1);
	lua_pop(lua, 7);

	lua_getglobal(lua, "package");
	lua_pushstring(lua, "path");
	lua_pushstring(lua, gamepath("?.lua"));
	lua_settable(lua, 1);
	lua_settop(lua, 0);
}

void	script_shutdown()
{
	lua_close(lua);
}

void	script_load(const char *luafile)
{
	if (luaL_dofile(lua, luafile)) {
		FATAL(lua_tostring(lua, -1));
	}
}

void	script_register(const char *cmdname, int (*func)(lua_State*))
{
	lua_register(lua, cmdname, func);
}

void	script_call(const char *func, const char *fmt, ...)
{
	int i;
	va_list v;

	if (func == NULL) {
		if (lua_pcall(lua, 0, 0, 0) != 0) {
			FATAL(lua_tostring(lua, -1));
		}
		return;
	}

	lua_getglobal(lua, func);
	if (fmt == NULL) {
		if (lua_pcall(lua, 0, 0, 0) != 0) {
			FATAL(lua_tostring(lua, -1));
		}
		return;
	}
	va_start(v, fmt);
	for (i = 0; *fmt; ++fmt, ++i) {
		switch (*fmt) {
		case 'i':
			lua_pushinteger(lua, va_arg(v, int));
			break;
		case 'n':
			lua_pushnumber(lua, va_arg(v, double));
			break;
		case 's':
			lua_pushstring(lua, va_arg(v, const char*));
			break;
		case 'p':
			lua_pushlightuserdata(lua, va_arg(v, void*));
			break;
		case 'b':
			lua_pushboolean(lua, va_arg(v, int));
			break;
		}
	}
	va_end(v);
	if (lua_pcall(lua, i, 0, 0) != 0) {
		FATAL(lua_tostring(lua, -1));
	}
}
