#include "../Interfaces/REPL.h"
#include <stddef.h>
#include <string.h>
#include "../Interfaces/parser.h"


void Start() {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

   printf("HEYA! Welcome to the Zed programming language, " aryan "z# " RESET
         "is our command "
         "prompt. Have fun EXPERIMENTING (;"
         "\033[38;5;117m"
         "\nz# " RESET);
	pair** p = initTable();
	Lexer* lexer = NULL;
	parser* Parser = NULL;
	Program* program = NULL;	
  while ((read = getline(&line, &len, stdin)) != -1) {
		line[strlen(line) - 1] = '\0';
		if (strlen(line) == 0) {
			printf("No input. Repeat.\n");
			    printf("\033[38;5;117m"
           "z# " RESET);
			continue;
		}
		lexer = newLexer(line);
		Parser = newParser(lexer);
		program = ParseProgram(Parser);
		if (GetErrSize(Parser) > 0) {
			printf("You have %u errors: ", GetErrSize(Parser));

			for (size_t i = 0; i < GetErrSize(Parser); i++)
				printf("%s\n\n", GetStr(GetErrors(Parser)[i]));
		}	


	
	for (size_t i = 0; i < GetErrSize(Parser); i++)
		free(GetStr(GetErrors(Parser)[i]));

	free(GetErrors(Parser));

	free(lexer);
	
	// Free the parser and its tokens

	if (GetErrSize(Parser) > 0) {
		free(peekParserToken(Parser));
	}

	else {
		free(curParserToken(Parser));
		free(peekParserToken(Parser));
	}

	free(Parser);
	
    printf("\033[38;5;117m"
           "z# " RESET);
  }


	free(line);
	size_t s = ComputeSize();
	for (size_t i = 0; i < s; i++)
	{
		free(FetchPairKey(p[i]));
		free(p[i]); 
	}
	free(p);


}

