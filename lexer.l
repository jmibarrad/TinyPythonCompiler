%{
	
	#include <cstdio>
	#include <cstdlib>
	#include <iostream>
	#include <stack>
	#include "ast.h"
	#include "tokens.h"

	stack <int> tab_stack;
	int lines = 1;
	int detab_count = 0;
%}

%option noyywrap
%x NEWLINE

ID [a-zA-Z_]*[a-zA-Z][a-zA-Z0-9_]*
STRING_LITERAL ["\"".*"\""]

%%
	
%{
	if(detab_count > 0) {
		detab_count--;
		//tab_stack.pop();
		return TK_DEDENT;
	}
%}

<NEWLINE>^[    ]+ {
	BEGIN(INITIAL);
	if (yyleng > tab_stack.top()) {
        tab_stack.push(yyleng);
        return TK_INDENT;
    } else if (yyleng < tab_stack.top()) {
        tab_stack.pop();
        while(yyleng < tab_stack.top()){
        	detab_count++;
        	tab_stack.pop();
        }
        return TK_DEDENT;
    }
}


<NEWLINE>. {	
	unput(yytext[0]); 
	BEGIN(INITIAL);
	if (tab_stack.top() != 0) {	
        tab_stack.pop();
        while(tab_stack.top() != 0){
        	detab_count++;
        	tab_stack.pop();
        }
        return TK_DEDENT;
    }
}

<NEWLINE><<EOF>> {
	BEGIN(INITIAL);
	if (tab_stack.top() != 0) {	
        tab_stack.pop();
        while(tab_stack.top() != 0){
        	detab_count++;
        	tab_stack.pop();
        }
        return TK_DEDENT;
    }
}

	[ ] /* Do Nothing */
	\t  /* Do Nothing */
	\n 	{
			lines++;
			BEGIN(NEWLINE);
			//return TK_NEWLINE;
		}
	":" {return ':';}
	"," {return ',';}

	"+" {return '+';}
	"-" {return '-';}
	"*" {return '*';}
	"/" {return '/';}
	"(" {return '(';}
	")" {return ')';}

	">"  { return '>'; }
	"<"  { return '<'; }
	">=" { return OP_GREATEREQUAL; }
	"<=" { return OP_LESSEQUAL; }

	"=" { return '='; }

	"and" 	{ return OP_AND; }
	"or" 	{ return OP_OR; }
	"not" 	{ return OP_NOT; }

	"==" { return OP_EQUAL; }
	"!=" { return OP_NOTEQUAL; }

	"def" 		{ return KW_DEF; }
	"break" 	{ return KW_BREAK; }
	"continue" 	{ return KW_CONTINUE; }
	"class" 	{ return KW_CLASS; }
	"print" 	{ return KW_PRINT; }
	"if" 		{ return KW_IF; }
	"elif" 		{ return KW_ELIF; }
	"else" 		{ return KW_ELSE; }
	"for" 		{ return KW_FOR; }
	"while" 	{ return KW_WHILE; }
	"return" 	{ return KW_RETURN; }
	"input" 	{ return KW_INPUT; }
	"true" 		{ return KW_TRUE; }
	"false" 	{ return KW_FALSE; }


{ID} 				{ yylval.str_t = strdup(yytext); return TK_ID;}
[0-9]+ 				{ yylval.num_t = atoi(yytext); return TK_NUM; }
{STRING_LITERAL} 	{ yylval.str_t = strdup(yytext); return STRING_LITERAL; }

. { printf("[Error] Not a valid character: %s line: %d\n", yytext, lines); }

%%	

