#include "../Interfaces/lexer.h"
#include "../Interfaces/token.h"
#include "../Interfaces/lookup_table.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef  struct opaqueLexer {
	const char* input;
	size_t position; 
	size_t readPosition;
	size_t tokens;
	char ch;
} Lexer;

typedef struct opaqueToken 
{
	enum TokenType Type;
	void* Value;
} Token;

size_t GetTokens(Lexer* l) {
	return l->tokens;
}

size_t GetLexerPos(Lexer* l) {
	return l->position;
}

void readChar(Lexer* l)
{
	if (l->readPosition >= strlen(l->input))
		l->ch = '\0';

	else
		l->ch = l->input[l->readPosition];
	
	l->position = l->readPosition;
	l->readPosition += 1;
}
Lexer* newLexer(const char* input)
{
	Lexer* l = (Lexer*)malloc(sizeof(Lexer));
	l->input = input;
	l->position = 0;
	l->readPosition = 0;
	l->tokens = 0;
	readChar(l);	
	return l;
}

 Token* newToken( Lexer* l, enum TokenType type, void* data, size_t size)
{
	 Token* token = ( Token*)malloc(sizeof( Token));
	token->Type = type;
	token->Value = malloc(size);
	memcpy(token->Value, data, size);
	l->tokens += 1;
	return token;
}

string* identify( Lexer* l)
{
	string* str = constructStr();

	bool isNumeric = l->ch >= '0' && l->ch <= '9';

	if (!isNumeric)
	{
		for ( ; (l->ch >= 'a' && l->ch <= 'z') || (l->ch >= 'A' && l->ch <= 'Z') || l->ch == '_'; )
		{
			push_back(str, l->ch);
			readChar(l);
		}
	}

	else
	{
		for ( ; l->ch >= '0' && l->ch <= '9'; )
		{
			push_back(str, l->ch);
			readChar(l);
		}

		SetBool(str, isNumeric);
	}
	return str;
} 

char peekChar(Lexer* l) {
	if (l->readPosition >= strlen(l->input))
		return '\0';
	
	else
		return l->input[l->readPosition];
}

Token* NextToken( Lexer* l, pair** p) {
	 Token* token = NULL;

	if (l->position > strlen(l->input) - 1) {
			token = newToken(l, eof, &l->ch, sizeof(char));
			return token;
	}
	switch (l->ch) {
		case '-':
			token = newToken(l, MINUS, &l->ch, sizeof(char));
			readChar(l);
			break;
		
		case '!':
			if (peekChar(l) != '=')
				token = newToken(l, BANG, &l->ch, sizeof(char));
			
			else
			{
				char str[]= "!=\0";
				token = newToken(l, NEQUAL, str, strlen(str) + 1);
			}
			readChar(l);
			break;
		
		case '*':
			token = newToken(l, ASTERISK, &l->ch, sizeof(char));
			readChar(l);
			break;
		
		case '/':
			token = newToken(l, SLASH, &l->ch, sizeof(char));
			readChar(l);
			break;
		
		case '>':
			token = newToken(l, GT, &l->ch, sizeof(char));
			readChar(l);
			break;
		
		case '<':
			token = newToken(l, LT, &l->ch, sizeof(char));
			readChar(l);
			break;

		case '=':
			if (peekChar(l) != '=')
				token = newToken(l, ASSIGN, &l->ch, sizeof(char));
			
			else
			{
				char str[]= "==\0";
				token = newToken(l,  EQUAL, str,strlen(str) + 1);
			}
			readChar(l);
			break;

		case ';':
			token = newToken(l, SEMICOLON, &l->ch, sizeof(char));
				readChar(l);
			break;
		
		case '(':
			token = newToken(l, LPAREN, &l->ch, sizeof(char));
				readChar(l);
			break;

		case ')':
			token = newToken(l, RPAREN, &l->ch, sizeof(char));
				readChar(l);
			break;

		case ',':
			token = newToken(l, COMMA, &l->ch, sizeof(char));
				readChar(l);
			break;

		case '+':
			token = newToken(l, PLUS, &l->ch, sizeof(char));
				readChar(l);
			break;

		case '{':
			token = newToken(l, LBRACE, &l->ch, sizeof(char));
				readChar(l);
			break;

		case '}':
			token = newToken(l, RBRACE, &l->ch, sizeof(char));
				readChar(l);
			break;

		default: ;
			string* str = identify(l);
			
			if (GetSize(str) > 0 && !isNumeric(str))
				token = newToken(l, FetchPairType(p, GetStr(str)), GetStr(str),strlen(GetStr(str)) + 1);

			else if (GetSize(str) > 0 && isNumeric(str))
			{
				int num = 0;

				for (size_t i = 0; i < strlen(GetStr(str)); i++)
				{
					num += (int)at(str, i) - 48;
					num *= 10;
				}

				num /= 10;
				token = newToken(l, INT, &num, sizeof(int));
			}

			else
			{
				token = newToken(l, ILLEGAL, &l->ch, sizeof(char));
				readChar(l);
			}
			free(GetStr(str));
			free(str);
	}

	for ( ; l->ch == ' ' || l->ch == '\n'; )
		readChar(l);

	return token;
}

void* FetchTokenValue(Token* t) {
	return t->Value;
}

void SetTokenType(Token* t, enum TokenType Type) {
	t->Type = Type; 
}
enum TokenType FetchTokenType(Token* t) {
	return t->Type;
}
