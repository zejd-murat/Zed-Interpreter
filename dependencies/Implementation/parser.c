#include <stdlib.h>
#include <stdio.h>
#include "../Interfaces/parser.h"

static const char* Map[] = {"ILLEGAL", "eof", "IDENT", "INT", "ASSIGN", "PLUS", "MINUS", "BANG", "NEQUAL", "EQUAL", "ASTERISK", "SLASH", "LT", "GT", "FUNCTION", "LET", "TRUE", "FALSE", "IF", "ELSE", "RETURN", "COMMA",
	"SEMICOLON", "LPAREN", "RPAREN", "LBRACE", "RBRACE"};

typedef struct opaqueParser {
	Lexer* l;
	Token* curToken;
	Token* peekToken;
	string** errors;
	uint32_t errcap;
	uint32_t errsize;
} parser;

typedef struct opaqueExpr { // How to implement this in C?
	const char* src;
	void* Value;
} Expression;

typedef struct opaqueLetSt {
	Token* t; // token.LET in the book
	char* identifier;
	Expression* Value;
} LetStatement;

typedef struct opaqueReturnSt {
	Token* t; // 'return' token
	Expression* ReturnValue;
} ReturnStatement;

typedef struct opaqueExprSt {
	Token* t; // first token of the expression
	Expression* expr;
} ExpressionSt;

typedef struct opaqueStatement {
	enum StatementType StType;
	void* St;
} Statement;

typedef struct opaqueBlockSt {
	Token* t; // the { token
	Statement** statements;
} BlockSt;

typedef struct opaqueProgram {
	Statement** statements;
	uint32_t nStatements;
} Program;


bool curTokenIs(parser* p, enum TokenType t) {
	return t == FetchTokenType(p->curToken);
}

bool peekTokenIs(parser* p, enum TokenType t) {
	return FetchTokenType(p->peekToken) ==t;
}

uint32_t GetErrSize(parser* p) {
	return p->errsize;
}

string** GetErrors(parser* p) {
	return p->errors;
}

void peekError(parser* p, enum TokenType t) {
	string* s = constructStr();
	push_str(s, 5, "expected next token to be ", Map[t], ", got ", Map[FetchTokenType(p->peekToken)], " instead.");

	if (p->errsize < p->errcap) 
		p->errors[p->errsize++] = s;

	else {
		string** arr = calloc(p->errsize * 2, sizeof(string*));

		for (uint32_t i = 0; i < p->errsize; i++)
			arr[i] = p->errors[i];
		
		arr[p->errsize++] = s;

		for (uint32_t i = 0; i < p->errcap; i++)
			free(p->errors[i]);
		
		free(p->errors);

		p->errors = arr;
		p->errcap = p->errsize * 2;
	}	
}

bool expectPeek(parser* p, enum TokenType t) {
	if (peekTokenIs(p, t))
	{
		nextToken(p);
		return true;
	}

	else
	{
		peekError(p, t);
		return false; // Soon this will have to change, because I want debugging to be easier
	}
}
bool expectCur(parser* p, enum TokenType t) {
	if (curTokenIs(p, t))
	{
		nextToken(p);
		return true;
	}

	else
	{
		peekError(p, t);
		return false; // Soon this will have to change, because I want debugging to be easier
	}
}

Token* curParserToken(parser* p) {
	return p->curToken;
}

Token* peekParserToken(parser* p) {
	return p->peekToken;
}

void nextToken(parser* p) {
		p->curToken = p->peekToken;
		pair** pr = initTable();
		p->peekToken = NextToken(p->l, pr);
		size_t s = ComputeSize();
	for (size_t i = 0; i < s; i++)
	{
		free(FetchPairKey(pr[i]));
		free(pr[i]); 
	}
	free(pr);

}

parser* newParser(Lexer* l) {
	parser* p = malloc(sizeof(parser));

	p->l = l;
	p->curToken = NULL;
	p->peekToken = p->curToken;
	p->errcap = 3;
	p->errors = calloc(p->errcap, sizeof(char*));
	p->errsize = 0;
	nextToken(p);
	nextToken(p);

	return p;

}

/* 
Below functions are not fully implemented yet,
I just want the compiler to stop bitching.
*/
Statement* parseLetStatement(parser* p) {
	Statement* s = malloc(sizeof(Statement));

	s->StType = LetSt;
	s->St = malloc(sizeof(LetStatement));
	((LetStatement*)s->St)->t = p->curToken;
	if (!expectPeek(p, IDENT)) {
		free(((LetStatement*)s->St)->t);
		free(s);
		return NULL;
	}


	((LetStatement*)s->St)->identifier = FetchTokenValue(p->curToken);
	
	if (!expectPeek(p, ASSIGN)) {
		free(((LetStatement*)s->St)->t);
		free(((LetStatement*)s->St)->identifier);
		free(s->St);
		free(s);
		return NULL;
	}

	for ( ; !curTokenIs(p, SEMICOLON) && !curTokenIs(p, eof); ) // Jumping over expressions for now
		nextToken(p);

	if (!expectCur(p, SEMICOLON)) {
		free(((LetStatement*)s->St)->t);
		free(((LetStatement*)s->St)->identifier);
		free(s->St);
		free(s);
		return NULL;
	}
	return s;
}

Statement* parseReturnStatement(parser* p) {
	Statement* s = malloc(sizeof(Statement));

	s->StType = ReturnSt;
	s->St = malloc(sizeof(ReturnStatement));
	((ReturnStatement*)s->St)->t = p->curToken;

	nextToken(p);
	if (!curTokenIs(p, SEMICOLON) && !curTokenIs(p, eof)) { // Jumping over expressions for now
		nextToken(p);
	}
	if (!expectCur(p, SEMICOLON)) {
		free(((ReturnStatement*)s->St)->t); // No expression to FREE for now.
		free(s->St);
		free(s);
		return NULL;
	}

	return s;
}

Statement* parseExpressionStatement(parser* p) {
	return NULL; // Not parsing expressions for now
}
/* */

Statement* parseStatement(parser* p) {
	switch (FetchTokenType(p->curToken)) {
		case LET:
			return parseLetStatement(p);
		case RETURN:
			return parseReturnStatement(p);
		default:
			return parseExpressionStatement(p);
	};
}


void checkParserErrors(parser* p) { // This function doesn't have any use yet

	if (p->errsize == 0) {
		printf("\nNo errors.\n");
	}

	else {
		printf("Parser has %u errors", p->errsize);

		for (uint32_t i = 0; i < p->errsize; i++)
			printf("%s\n", GetStr(p->errors[i]));

		// A lot of code to deallocate should be here
		exit(1);
	}
}
Program* ParseProgram(parser* p) {
	Program* prog = malloc(sizeof(Program));

	prog->statements = calloc(GetTokens(p->l), sizeof(Statement*));

	prog->nStatements = 0;

	Statement* st = NULL;
	for (size_t i = 0; ; i++) {
		st = parseStatement(p);

		if (st != NULL) {
			prog->statements[i] = st;
			prog->nStatements += 1;
		}
	
		if (FetchTokenType(p->peekToken) == eof)
			break;
		

		nextToken(p);
	}
	return prog;
}

