#pragma once
#include "../Interfaces/token.h"
#include <stdint.h>

typedef struct opaquePair pair;

pair** initTable();

size_t ComputeSize();

char* FetchPairKey(pair* p);

enum TokenType FetchPairType(pair** p, const char* key);
