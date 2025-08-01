#pragma once
#include "./lexer.h"
#include "./ast.h"
#include <stdbool.h>

typedef struct opaqueParser parser;

parser* newParser(Lexer* l);

void nextToken(parser* p);

Program* ParseProgram(parser* p);

uint32_t GetErrSize(parser* p);

string** GetErrors(parser* p);

Token* curParserToken(parser* p);

Token* peekParserToken(parser* p);