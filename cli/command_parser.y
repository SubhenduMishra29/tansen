%{
#include <iostream>
#include <string>
#include <stdexcept>

// External declarations for parsed values
std::string raagName;
std::string taalName;
std::string tempoName;

extern int yylex();
extern int yyparse();
extern void yyerror(const char *s);
%}

%union {
    char* str;
}

%token <str> RAAG TAAL TEMPO_NAME WORD
%type <str> command

%%

// Grammar rules
command:
    RAAG WORD TAAL WORD TEMPO_NAME {
        raagName = $2;
        taalName = $4;
        tempoName = $5;
    }
    | RAAG WORD TAAL WORD {
        raagName = $2;
        taalName = $4;
        tempoName = "Drut"; // Default tempo
    }
    ;

%%
