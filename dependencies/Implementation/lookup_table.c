#include "../Interfaces/lookup_table.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct opaquePair {
	char* key;
	enum TokenType token;
} pair;

size_t ComputeSize() { 
	enum TokenType i = FIRST_KEYWORD; // any new KEYWORD enum MUST be added after whatever enum this macro is
	size_t x = 1;
	for (; i < LAST_KEYWORD; i++) // any new KEYWORD enum MUST be added before whatever enum this macro is
	{
		++x;
	}
	
	return x;
}
// Why `ComputeSize()` and not just a macro or a const? Desperate attempt to automate this shit


char* FetchPairKey(pair* p) { // THIS was needed ONLY to feed it to the `free()` function
	return p->key;
}

size_t hash(const char* key, size_t s) {

	size_t length = strlen(key);

	size_t hash_value = 0;

	for (size_t i = 0; i < length; i++)
	{
		hash_value += (size_t)key[i];
		hash_value = (hash_value  * (size_t)key[i]) % s;
	}	

	return hash_value;
}

pair** initTable() {
    const char* keywords[] = {"fn", "let", "true", "false", "if", "else", "return"};

	size_t size = ComputeSize();

	pair** p = calloc(size, sizeof(pair*));
	enum TokenType t = FIRST_KEYWORD;
    for (size_t i = 0; i < size; i++) {

		size_t temp = hash(keywords[i], size);
		if (p[temp] == NULL)
		{
			p[temp] = malloc(sizeof(pair));
  	      	p[temp]->key = strdup(keywords[i]);
		    p[temp]->token = t;
		}

		else
		{
			for (size_t x = 0; x < size; x++)
			{
				if (p[x] == NULL) {
					p[x] = malloc(sizeof(pair));
					p[x]->key = strdup(keywords[i]);
					p[x]->token = t;
					break;
				}
			}	
		}
		++t;
    }
	return p;
}

enum TokenType FetchPairType(pair** p, const char* key) {
	size_t s = ComputeSize();
	size_t hashed = hash(key, s);

	if (strcmp(p[hashed]->key, key) == 0) {
		return p[hashed]->token;
	}

	else {
		for (size_t x = 0; x < s; x++) {
			if (strcmp(p[x]->key, key) == 0) { // This used to be "p[x]->key == key" lmfao
				return p[x]->token;
			}
		}

		return IDENT;
	}
}

