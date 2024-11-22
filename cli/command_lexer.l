%{
#include "command_parser.tab.h" // Generated Bison header
%}

%%

// Define keywords
"Raag"            return RAAG;
"Taal"            return TAAL;
"Bilambit"|"Madhya"|"Drut"  { yylval.str = strdup(yytext); return TEMPO_NAME; }

// Match words (names)
[a-zA-Z]+         { yylval.str = strdup(yytext); return WORD; }

// Ignore whitespace
[ \t\n]+          /* ignore */;

// Handle invalid characters
.                 { return yytext[0]; }

%%
