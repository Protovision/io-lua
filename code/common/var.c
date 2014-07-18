#include "common.h"

#define VAR_HASHTABLE_SIZE	64	
#define VAR_MAX			128

static int	free_var;
static var_t	variables[VAR_MAX];
static var_t	*hashtable[VAR_HASHTABLE_SIZE];

unsigned int	var_hash(const char *key)
{
	unsigned int hash;

	hash = 0;
	while (*key != 0) {
		hash ^= (int)*key++;
		hash += (hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
	}
	return hash;
}

var_t	*var_get(const char *name)
{
	unsigned int hash;
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
/*
	int i;
	
	for (i = 0; i != VAR_MAX; ++i) {
		if (variables[i].name == NULL)
			return &variables[i];
	}
	return NULL;
*/

	if (free_var == VAR_MAX) return NULL;
	return &variables[free_var++];
}

var_t	*var_set(const char *varname, const char *value)
{
	unsigned int hash;
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
		w->name = stralloc(varname);
		w->string = stralloc(value);
		w->prev = NULL;
		w->next = v;
		if (v) v->prev = w;
		hashtable[hash] = w;
	} else {
		free(w->string);
		w->string = stralloc(value);
	}	
		
	w->real = atof(w->string);
	w->integer = atoi(w->string);

	return w;
}

void	var_init()
{
	free_var = 0;
	memset(variables, 0, sizeof(variables));
	memset(hashtable, 0, sizeof(hashtable));
}

void	var_shutdown()
{
	int i;
	var_t *v;
	for (i = 0; i != VAR_HASHTABLE_SIZE; ++i) {
		for (v = hashtable[i]; v; v = v->next) {
			free(v->name);
			free(v->string);
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
