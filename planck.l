%{
    #include "token.hpp"
    int lineno = 0, colno = 0, commentFlag = 0;
%}

DIGIT                   [0-9]
LETTER                  [a-zA-Z]
ID                      {LETTER}({LETTER}|{DIGIT})*(("."|"#"|"$"|"_")?({LETTER}|{DIGIT})+)?
NUM                     ({DIGIT}+|{DIGIT}+"."{DIGIT}*)((E|e)("+"|"-")?{DIGIT}+)?
NOTNUM                  ({DIGIT}+|{DIGIT}+"."{DIGIT}*)((E|e)("+"|"-")?[^{DIGIT}|\n]+)?

ADDOP                   "+"|"-"
MULOP                   "*"|"/"
LT                      "<"
LTE                     "<="
BT                      ">"
BTE                     ">="
EQ                      "=="
NEQ                     "!="
ASSIGN                  "="

GET                     "get"
PERCENT_SIGN            "%"
DOUBLE_ASTERISK         "**"
QUESTION_MARK           "?"
DOLLAR_SIGN             "$"
BLOCK_START             "->"


%x C_COMMENT
%option noyywrap

%%
[ \t]+                  { colno += yyleng; }
[ \t]+$                 /* Ignore */

"#".*                   { /* Ignore single-line comments starting with # */ }

"+"                     { colno += yyleng; return token::terminal::PLUS; }
"-"                     { colno += yyleng; return token::terminal::MINUS; }
"*"                     { colno += yyleng; return token::terminal::STAR; }
"/"                     { colno += yyleng; return token::terminal::SLASH; }
"<="                    { colno += yyleng; return token::terminal::LTE; }
"<"                     { colno += yyleng; return token::terminal::LESS_THAN; }
">="                    { colno += yyleng; return token::terminal::GTE; }
"=="                    { colno += yyleng; return token::terminal::EQ; }
"!="                    { colno += yyleng; return token::terminal::NEQ; }
"="                     { colno += yyleng; return token::terminal::EQUAL; }
"->"                    { colno += yyleng; return token::terminal::BLOCK_START; }

{GET}                   { colno += yyleng; return token::terminal::GET; }
{PERCENT_SIGN}          { colno += yyleng; return token::terminal::PERCENT_SIGN; }
{DOUBLE_ASTERISK}       { colno += yyleng; return token::terminal::DOUBLE_ASTERISK; }
{QUESTION_MARK}         { colno += yyleng; return token::terminal::QUESTION_MARK; }
{DOLLAR_SIGN}           { colno += yyleng; return token::terminal::DOLLAR_SIGN; }

{ID}                    { colno += yyleng; return token::terminal::IDENTIFIER; }
{NUM}                   { colno += yyleng; return token::terminal::NUMBER; }
{NOTNUM}                { colno += yyleng; return token::terminal::UNKNOWN; }

"/*"                    { BEGIN(C_COMMENT); commentFlag = 1; }
<C_COMMENT>"*/"         { BEGIN(INITIAL); commentFlag = 0; }
<C_COMMENT>\n           { lineno++; }
<C_COMMENT>.            { /* ignore comment body */ }

<<EOF>>                 { if (commentFlag == 1) printf("^ Invalid_Comment %s %d %d\n", "<eof>", lineno + 1, colno); return 0; }

.                       { colno += yyleng; return token::terminal::UNKNOWN; }
\n                      { lineno++; colno = 0; }
%%


//
//int main(int argc, char *argv[]) {
//    // Arguments Validation
//    if (argc < 2) {
//        printf("\nInvalid arguments! Expected 1 arg: inputFilePath\n");
//        return -1;
//    }
//
//    // Input file init
//    FILE *inFile;
//
//    inFile = fopen(argv[1], "r");
//    if (!inFile) {
//        printf("Error: Cannot open file %s\n", argv[1]);
//        return -1;
//    }
//    yyin = inFile;
//
//    yylex();
//    return 0;
//}
