#include "common.h"

#define VAR_HASHTABLE_SIZE	256
#define VAR_MAX			2000

static var_t	variables[VAR_MAX];
static var_t	*hashtable[VAR_HASHTABLE_SIZE];

int	var_hash(const char *key)
{
	int hash;

	hash = 0;
	while (*key != 0) {
		hash ^= (int)*key++;
		hash += (hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
	}
	return hash;
}

var_t	*var_get(const char *name)
{
	int hash;
	var_t *v;

	hash = var_hash(name) % VAR_HASHTABLE_SIZE;
	for (v = hashtable[hash]; v; v = v->next) {
		if (strcmp(v->name, name) == 0) {
			return v;
		}
	}
	return NULL;
}

var_t	*var_allocate()
{
	int i;
	
	for (i = 0; i != VAR_MAX; ++i) {
		if (variables[i].name == NULL)
			return &variables[i];
	}
	return NULL;
}

var_t	*var_set(const char *varname, const char *value)
{
	int hash;
	var_t *v, *w;
		
	hash = var_hash(varname) % VAR_HASHTABLE_SIZE;
	v = hashtable[hash];
	for (w = v; w; w = v->next) {
		if (strcmp(varname, w->name) == 0) {
			break;
		}
	}

	if (w == NULL) {
		w = var_allocate();
		w->name = mem_strdup(varname);
		w->string = mem_strdup(value);
		w->prev = NULL;
		w->next = v;
		if (v) v->prev = w;
		hashtable[hash] = w;
	} else {
		mem_free(w->string);
		w->string = mem_strdup(value);
	}	
		
	w->real = atof(w->string);
	w->integer = atoi(w->string);

	return w;
}

void	var_init()
{
	memset(variables, 0, sizeof(variables));
	memset(hashtable, 0, sizeof(hashtable));
}

void	var_shutdown()
{
	int i;
	var_t *v;
	for (i = 0; i != VAR_HASHTABLE_SIZE; ++i) {
		for (v = hashtable[i]; v; v = v->next) {
			mem_free(v->name);
			mem_free(v->string);
			v->name = NULL;
		}
	}
}

void	var_load(gameVar_t *vars)
{
	for (; vars->var != NULL; ++vars) {
		*vars->var = var_get(vars->name);
		if (*vars->var == NULL) {
			*vars->var = var_set(vars->name, vars->default_value);
		}	
	}
}
