#pragma once
#include <stddef.h>
#define LAST_KEYWORD RETURN
#define FIRST_KEYWORD FUNCTION

enum TokenType
{
	ILLEGAL, 

	eof,

	IDENT, 
	
	// Literals
	INT,

	// Mathematical OPERATORS
	ASSIGN, PLUS, MINUS, BANG, NEQUAL, EQUAL, ASTERISK, SLASH, LT, GT,

	// Keywords
	FUNCTION, LET, TRUE, FALSE, IF, ELSE, RETURN,

	// punctuation
	COMMA, SEMICOLON, LPAREN, RPAREN, LBRACE, RBRACE,

};

typedef struct opaqueToken Token;

enum TokenType FetchTokenType(Token* t); 