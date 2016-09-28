%{
	#include <iostream>
	#include <cstdio>
	#include <fstream>
	#include <map>
	#include <string>
	#include "ast.h"
//	#include <stack>

	#define YYERROR_VERBOSE 1;

	using namespace std;

	int yylex();	
	void yyerror(const char * str);

	extern stack <int> tab_stack;
	extern int detab_count;
	extern int lines;
%}

%union {
	
	Class* class_t;
	GlobalVariable* globalVariable_t;
	Parameter* param_t;
	DeclaredParameter* dparam_t;
	Method* method_t;

	char* str_t;
	int num_t;

	Expression* expression_t;
	Statement* statement_t;

}

/* Key Words */
	%token KW_INPUT
	%token KW_TRUE
	%token KW_FALSE
	%token KW_BREAK
	%token KW_CONTINUE
	%token KW_CLASS
	%token KW_DEF
	%token KW_PRINT
	%token KW_IF
	%token KW_ELIF
	%token KW_ELSE
	%token KW_FOR
	%token KW_WHILE
	%token KW_RETURN

/* Tokens */
	%token TK_INDENT
	%token TK_DEDENT
	%token <num_t> TK_NUM
	%token <str_t> TK_ID
	%token <str_t> STRING_LITERAL

/*          Types        */
	%type <class_t> CLASS

	%type <globalVariable_t> VARIABLE_LIST
	%type <globalVariable_t> OP_VARIABLE_LIST
	%type <globalVariable_t> VARIABLE

	%type <param_t> PARAMETER
	%type <param_t> PARAMETER_LIST
	%type <param_t> OP_PARAMETER_LIST

	%type <dparam_t> DECLARED_PARAMETER
	%type <dparam_t> DECLARED_PARAMETER_LIST
	%type <dparam_t> OP_DECLARED_PARAMETER_LIST

	%type <method_t> METHOD
	%type <method_t> METHOD_LIST
	
	%type <str_t> OP_STRING_LITERAL

/*        Expressions     */
	%type <expression_t> e
	%type <expression_t> OPTIONAL_VALUE

/*          Statements     */
	%type<statement_t> METHOD_INVOKE
	%type<statement_t> STATEMENT
	%type<statement_t> STATEMENT_LIST
	%type<statement_t> PRINT
	%type<statement_t> BLOCK
	%type<statement_t> IF
	%type<statement_t> ELIF
	%type<statement_t> ELSE
	%type<statement_t> FOR
	%type<statement_t> WHILE
	%type<statement_t> ASSIGN
	%type<statement_t> LOOP_BLOCK
	%type<statement_t> LOOP_STATEMENT
	%type<statement_t> LOOP_STATEMENT_LIST
	%type<statement_t> INPUT


	%nonassoc '>' '<' OP_GREATEREQUAL OP_LESSEQUAL OP_EQUAL OP_NOTEQUAL OP_NOT OP_OR OP_AND

	%left '+' '-' '*' '/'

%%

input: 	CLASS {
			string code = $1->GenerateCode();
			ofstream output;
			output.open("py.asm");
			output << code;
			output.close();

			delete $1;
		}
;

CLASS: KW_CLASS TK_ID ':'  TK_INDENT OP_VARIABLE_LIST METHOD_LIST TK_DEDENT { $$ = new Class($5, $6); free($2); }
;

OP_VARIABLE_LIST: VARIABLE_LIST {$$ = $1;}
					| {$$ = NULL;}
;

VARIABLE_LIST: VARIABLE VARIABLE_LIST { $1->nextVariable = $2; $$ = $1; }
				| VARIABLE { $$ = $1; }
;

VARIABLE: TK_ID '=' e  { $$ = new GlobalVariable($1, $3); free($1); }
;

METHOD_LIST: METHOD METHOD_LIST { $1->nextMethod = $2; $$ = $1; }
			| METHOD {$$ = $1; }
;

METHOD: KW_DEF TK_ID '(' OP_DECLARED_PARAMETER_LIST ')' ':'  BLOCK {string str($2); $$ = new Method(str, $4, $7); free($2);}
;

OP_DECLARED_PARAMETER_LIST: DECLARED_PARAMETER_LIST {$$ = $1;}
							|{$$ = NULL;}
;

DECLARED_PARAMETER_LIST: DECLARED_PARAMETER ',' DECLARED_PARAMETER_LIST { $1-> nextDeclaredParameter = $3;}
						|DECLARED_PARAMETER { $$ = $1; }
;

DECLARED_PARAMETER: TK_ID ':' TK_ID { string str($1); string str2($3); $$ = new DeclaredParameter(str, str2); free($1); free($3);}
;

METHOD_INVOKE: TK_ID '(' OP_PARAMETER_LIST ')'  	{string str($1); $$ = new MethodInvoke(str, $3); free($1);}
;

OP_PARAMETER_LIST:  PARAMETER_LIST { $$ = $1; }
					| { $$ = NULL; }
;

PARAMETER_LIST: PARAMETER ',' PARAMETER_LIST { $1->nextParameter = $3; $$ = $1; }
				| PARAMETER { $$ = $1; }
;

PARAMETER: TK_ID ':' e { string str($1); $$ = new Parameter(str, $3); free($1); }
;


STATEMENT_LIST: STATEMENT STATEMENT_LIST { $1->nextStatement = $2; $$ = $1;}
				| STATEMENT { $$ = $1; }
;

STATEMENT: 	PRINT
			|INPUT
			|ASSIGN
			|IF 
			|KW_RETURN OPTIONAL_VALUE {$$ = new ReturnStatement($2); }
			|WHILE
			|FOR
			|METHOD_INVOKE
;

BLOCK: 	TK_INDENT STATEMENT_LIST TK_DEDENT { $$ = $2; }
;

LOOP_BLOCK: TK_INDENT LOOP_STATEMENT_LIST TK_DEDENT { $$ = $2;}
;

LOOP_STATEMENT_LIST: LOOP_STATEMENT LOOP_STATEMENT_LIST {$1->nextStatement = $2; $$ = $1;}
					| LOOP_STATEMENT {$$ = $1;}
;

LOOP_STATEMENT: STATEMENT { $$ = $1; }
				| KW_CONTINUE { $$ = new ContinueStatement(); }
				| KW_BREAK { $$ = new BreakStatement(); }
;


OP_STRING_LITERAL: 	STRING_LITERAL { $$ = $1; }
					| { $$ = NULL; }
;

OPTIONAL_VALUE: e {$$ = $1;}
				| {$$ = NULL;}
;

INPUT: TK_ID '=' KW_INPUT '(' OP_STRING_LITERAL ')' { string str2($1); string str($5); $$ = new InputStatement(str2, str); free($1); free($5);}
;


ASSIGN: TK_ID '=' e { $$ = new AssignStatement($1, $3); free($1);}
//		| TK_ID '=' ARRAY {} 
;

IF: KW_IF e ':'  LOOP_BLOCK ELIF { $$ = new IfStatement($2, $4, $5); }
;

ELIF: 	KW_ELIF e ':'  LOOP_BLOCK ELIF { $$ = new IfStatement($2, $4, $5);}
		| ELSE {$$ = $1;}
;

ELSE: 	KW_ELSE ':'  LOOP_BLOCK { $$ = $3; }
		| { $$ = NULL; }
;

WHILE: KW_WHILE e ':'  LOOP_BLOCK { $$ = new WhileStatement($2, $4); }
;

FOR: KW_FOR '('e','e')' ':'  LOOP_BLOCK { $$ = new ForStatement($3, $5, $8); }
;


PRINT: 	KW_PRINT '('e')'  { $$ = new PrintStatement($3);}
		//| KW_PRINT '(' STRING_LITERAL ')'  { $$ = new PrintStatement($3); }
;


/*EXPRESSION_LIST: 	e ',' EXPRESSION_LIST { $1->nextExpression = $2; $$ = $1; }
			    	|e { $$ = $1; }
;

OP_EXPRESSION_LIST: EXPRESSION_LIST { $$ = $1; }
					|{ $$ = NULL; }
;

ARRAY: '[' OP_EXPRESSION_LIST ']' { $$ = $2; }
;*/



e:	e '+' e 										{ $$ = new AddExpr($1,$3); } 
	|e '-' e 										{ $$ = new SubExpr($1,$3); } 
	|e '*' e   										{ $$ = new MultExpr($1,$3); }
	|e '/' e   										{ $$ = new DivExpr($1,$3); }
	|e '<' e 										{ $$ = new LTExpr($1,$3); }
	|e '>' e 										{ $$ = new GTExpr($1,$3); }
	|e OP_GREATEREQUAL e 							{ $$ = new GTEExpr($1,$3); }
	|e OP_LESSEQUAL e 								{ $$ = new LTEExpr($1,$3); }
	|e OP_EQUAL e 									{ $$ = new EQExpr($1,$3); }
	|e OP_NOTEQUAL e 								{ $$ = new NEQExpr($1,$3); }
	|'(' e ')'       								{ $$ = $2; }
    |TK_NUM        									{ $$ = new NumExpr($1); }
    |TK_ID 											{ string str($1); $$ = new IDExpr(str); free($1); }
    |TK_ID '(' OP_PARAMETER_LIST ')' 			 	{ string str($1); $$ = new FunctionInvoke(str, $3); free($1);}
    |KW_TRUE										{ $$ = new BoolExpr(true); }
    |KW_FALSE										{ $$ = new BoolExpr(false); }

;

%%

int main(){

	tab_stack.push(0);
	//detab_count = 0;
	//lines = 0;
	yyparse();
	return 0;
}

void yyerror(const char *str){
	printf("At line %d: %s\n", lines, str);
}