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
DIGIT [0-9]
/* {DIGIT}+"."{DIGIT}*
 ID [a-z][a-z0-9]* */
/* Position information is entered into the yylloc variable, so we will be
   able to generate good error messages later in later phases. */
/* Your code should be entered below the %%. Expressions to handle the 
   following: Diesel comments, Diesel string constants, Diesel 
   identifiers, integers, reals, and whitespace. */
%%

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
of				{
				    
				}
if				{return(T_IF);
				    
				}
do				{
				    
				}
or				{
				    
				}
var				{
				    
				}
end				{
				    
				}
and				{
				    
				}
div				{
				    
				}
mod				{
				    
				}
not				{
				    
				}
then				{
				    
				}
else				{
				    
				}
const				{
				    
				}
array				{
				    
				}
begin				{
				    
				}
while				{
				    
				}
elsif				{
				    
				}
return				{
				    
				}
program				{
				    
				}
function			{
				    
				}
procedure			{
				    
				}


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

<<EOF>>				yyterminate();
.				yyerror("Illegal character");
