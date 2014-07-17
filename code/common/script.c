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
}

void	script_shutdown()
{
	lua_close(lua);
}

int	script_load(const char *luafile)
{
	char *data;
	int top1, top2;

	data = unz_load(luafile);
	top1 = lua_gettop(lua);
	if (luaL_dostring(lua, data)) {
		//luaL_where(lua, 1);
		//FATAL("%s %s", lua_tostring(lua, -1), lua_tostring(lua, -2));
		FATAL("%s %s", luafile, lua_tostring(lua, -1));
	}
	top2 = lua_gettop(lua);
	free(data);
	return top2-top1;
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
			luaL_where(lua, 1);
			FATAL("%s %s", lua_tostring(lua, -1), lua_tostring(lua, -2));
		}
		return;
	}

	lua_getglobal(lua, func);
	if (lua_isnil(lua, -1)) {
		lua_pop(lua, 1);
		return;
	}

	if (fmt == NULL) {
		if (lua_pcall(lua, 0, 0, 0) != 0) {
			luaL_where(lua, 1);
			FATAL("%s %s", lua_tostring(lua, -1), lua_tostring(lua, -2));
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
		luaL_where(lua, 1);
		FATAL("%s %s", lua_tostring(lua, -1), lua_tostring(lua, -2));
	}
}

void	script_export_integer(const char *name, int value)
{
	lua_pushinteger(lua, value);
	lua_setglobal(lua, name);
}

void	script_export_number(const char *name, double value)
{
	lua_pushnumber(lua, value);
	lua_setglobal(lua, name);
}

void	script_export_string(const char *name, const char *value)
{
	lua_pushstring(lua, value);
	lua_setglobal(lua, name);	
}

void	script_export_pointer(const char *name, void *value)
{
	lua_pushlightuserdata(lua, value);
	lua_setglobal(lua, name);
}

void	script_export_boolean(const char *name, int value)
{
	lua_pushboolean(lua, value);
	lua_setglobal(lua, name);
}

int	script_import_integer(const char *name)
{
	int value;
	lua_getglobal(lua, name);
	value = lua_tointeger(lua, -1);
	lua_pop(lua, 1);
	return value;
}

double	script_import_number(const char *name)
{
	double value;
	lua_getglobal(lua, name);
	value = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	return value;
}

const char *script_import_string(const char *name)
{
	const char *value;
	lua_getglobal(lua, name);
	value = lua_tostring(lua, -1);
	lua_pop(lua, 1);
	return value;
}

void	*script_import_pointer(const char *name)
{
	void *value;
	lua_getglobal(lua, name);
	value = (void*)lua_topointer(lua, -1);
	lua_pop(lua, 1);
	return value;
}

int	script_import_boolean(const char *name)
{
	int value;
	lua_getglobal(lua, name);
	value = lua_toboolean(lua, -1);
	lua_pop(lua, 1);
	return value;
}
