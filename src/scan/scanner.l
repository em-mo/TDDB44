%{

#include <stdlib.h>
#include <string.h>

/* This is where you put #include directives as needed for later labs. */
#include "scanner.hh"

// Note that the order is important
// include ast.hh
// include parser.hh

int column = 0;

extern YYLTYPE yylloc; // Used for position information, see below.

%}

%option yylineno
%option 8bit
%option noyywrap
%option caseless
%option nodefault
%option outfile = "scanner.cc"
/* Your code here.
 * If you want to include any flex declarations, this is where to do it. */
%x c_comment
%x p_comment
LETTER [a-z]
DIGIT [0-9]

INTEGER {DIGIT}+

FLOAT {FLOAT3}|{FLOAT4}
FLOAT1 {DIGIT}+"."{DIGIT}*
FLOAT2 {DIGIT}*"."{DIGIT}+
FLOAT3 ({FLOAT1}|{FLOAT2}){EPART}?
FLOAT4 {DIGIT}*{EPART}
EPART e[\+-]?{DIGIT}*
ID [a-z][a-z0-9]*

STRING '([^'\n]|'')*'
NEWLINESTRING '([^'\n]|'')*\n

/* Position information is entered into the yylloc variable, so we will be
   able to generate good error messages later in later phases. */
/* Your code should be entered below the %%. Expressions to handle the 
   following: Diesel comments, Diesel string constants, Diesel 
   identifiers, integers, reals, and whitespace. */
%%

{FLOAT}           {yylval.rval = atof(yytext);
                 yylloc.first_line=yylineno;
                 yylloc.first_column=column;
                         column += yyleng;
                         return T_REALNUM;
                }

\.				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_DOT;
				}
;				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_SEMICOLON;
				}
=				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_EQ;			    
				}
\:				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_COLON; 
				}
\(				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_LEFTPAR;
				}
\)				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_RIGHTPAR;
				}
\[				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_LEFTBRACKET;   
				}
\]				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_RIGHTBRACKET;    
				}
,				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_COMMA;
				    
				}
\<				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_LESSTHAN;
				}
\>				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_GREATERTHAN;
				}
\+				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_ADD;				    
				}
\-				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_SUB;	
				}
\*				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_MUL;	
				}
\/				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_RDIV;	
				}
":="				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_ASSIGN;	
				}
"<>"				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_NOTEQ;	
				}
of				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_OF;	
				}
if				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_IF;	
				}
do				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_DO;	
				}
or				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_OR;	
				}
var				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_VAR;	
				}
end				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_END;	
				}
and				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_AND;	
				}
div				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_RDIV;	
				}
mod				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_MOD;	
				}
not				{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_NOT;	
				}
then			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_THEN;	
				}
else			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_ELSE;	
				}
const			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_CONST;	
				}
array			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_ARRAY;	
				}
begin			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_BEGIN;	
				}
while			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_WHILE;	
				}
elsif			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_ELSIF;	
				}
return			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_RETURN;	
				}
program			{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_PROGRAM;	
				}
function		{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_FUNCTION;	
				}
procedure		{yylloc.first_line=yylineno;
 				 yylloc.first_column=column;
                         column += yyleng;
                         return T_PROCEDURE;	
				}

{INTEGER}         {yylval.ival = atoi(yytext);
                 yylloc.first_line=yylineno;
                 yylloc.first_column=column;
                         column += yyleng;
                         return T_INTNUM;
                }

{STRING}          {char *fixed_string = sym_tab->fix_string(yytext);
                 yylval.str = sym_tab->pool_install(fixed_string);
                 delete fixed_string;

                 yylloc.first_line=yylineno;
                 yylloc.first_column=column;
                         column += yyleng;
                         return T_STRINGCONST;
                }

{NEWLINESTRING} {column = 0;
                 yyerror("Newline in string");
                }

{ID}              {char *fixed_string = sym_tab->capitalize(yytext);
                 yylval.pool_p = sym_tab->pool_install(fixed_string);
                 delete fixed_string;

                 yylloc.first_line=yylineno;
                 yylloc.first_column=column;
                         column += yyleng;
                         return T_IDENT;
                }

[ \t]           {column += 1;}

\n              {column = 0;}

\/\/.*$				column = 0; /* Skip single-line comment */
"/\*"				{
                                    column += yyleng;
                                    BEGIN(c_comment);
                                }

<c_comment>
{
    "\*/"			{
                                    column += 2;
                                    BEGIN(INITIAL);
                                }
    "/\*"			{
				    column += 2;
				    yyerror("Suspicious comment");
				}
    [^\n]			column++; /* Skip stuff in comments */
    \n				column = 0; 
    <<EOF>>			{
				    yyerror("Unterminated comment");
				    yyterminate();
				}
}

"{"               {
                                    column += yyleng;
                                    BEGIN(p_comment);
                                }

<p_comment>
{
    "}"           {
                                    column += yyleng;
                                    BEGIN(INITIAL);
                                }
    "{"           {
                    column += 1;
                    yyerror("Suspicious comment");
                }
    [^\n]           column++; /* Skip stuff in comments */
    \n              column = 0; 
    <<EOF>>         {
                    yyerror("Unterminated comment");
                    yyterminate();
                }
}

<<EOF>>				yyterminate();
.				yyerror("Illegal character");
