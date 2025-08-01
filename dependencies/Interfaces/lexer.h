#pragma once
#include "./token.h"
#include "./string.h"
#include "./lookup_table.h"

typedef struct opaqueLexer Lexer;

Lexer* newLexer(const char* input);

Token* NextToken(Lexer* l, pair** p);

size_t GetLexerPos(Lexer* l);

void* FetchTokenValue(Token* t);

enum TokenType FetchTokenType(Token* t);

size_t GetTokens(Lexer* l);
