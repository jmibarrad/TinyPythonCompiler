#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>

using namespace std;
//extern map<string,int> symbol_table;

class Expression {

	public:
		Expression* nextExpression = NULL;
		virtual void GenerateCode(string& code, string& addr ) = 0;
};

class AddExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		AddExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class SubExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		SubExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class MultExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		MultExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class DivExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		DivExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class GTExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		GTExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class LTExpr : public Expression {
	public:
 		Expression *left_expr = NULL, *right_expr = NULL;

	 	LTExpr(Expression *left_expr, Expression *right_expr) {
			this->left_expr = left_expr; 
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr); 
};

class GTEExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		GTEExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class LTEExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		LTEExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr );
};

class EQExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;

		EQExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class NEQExpr : public Expression {
	public:
		Expression *left_expr = NULL, *right_expr = NULL;
		
		NEQExpr(Expression *left_expr, Expression *right_expr){
			this->left_expr = left_expr;
			this->right_expr = right_expr;
		}

		void GenerateCode(string& code, string &addr);
};

class NumExpr : public Expression {
	public:
		int value;
		NumExpr(int value){
			this->value = value;
		}

		void GenerateCode(string& code, string &addr);
};

class IDExpr : public Expression {
	public:
		string Id;

		IDExpr(string Id){
			this->Id = Id;
		}

		void GenerateCode(string& code, string &addr);
};

class BoolExpr : public Expression {
	public:
		bool value;
		BoolExpr(bool value){
			this->value = value;
		}

		void GenerateCode(string& code, string &addr);
};

/* Statement Section */

class Statement{
	public: 
		Statement* nextStatement = NULL;
		virtual string GenerateCode() = 0;
};

class PrintStatement : public Statement {
	public: 
		Expression* expr;

		PrintStatement(Expression *expr){
			this->expr = expr;
		}

		string GenerateCode();
};

class AssignStatement : public Statement {
	public:
		string id;
		Expression* expr = NULL;

		AssignStatement(string id, Expression* expr){
			this->id = id;
			this->expr = expr;
		}
		
		string GenerateCode();
};

class IfStatement : public Statement {
	public:
		Expression* condition = NULL;
		Statement* trueBlock = NULL;
		Statement* falseBlock = NULL;

		IfStatement(Expression* condition, Statement* trueBlock, Statement* falseBlock) {
			this->condition = condition;
			this->trueBlock = trueBlock;
			this->falseBlock = falseBlock;
		}

		string GenerateCode();
};

class ForStatement : public Statement {
	public:
		Expression* iterator = NULL;
		Expression* condition = NULL;
		Statement* block = NULL;

		ForStatement(Expression* iterator, Expression* condition, Statement* block){
			this->iterator = iterator;
			this->condition = condition;
			this->block = block;
		}

		string GenerateCode();
};

class WhileStatement : public Statement{
	public:
		Expression* condition = NULL;
		Statement* block = NULL;

		WhileStatement(Expression* condition, Statement* block){
			this->condition = condition;
			this->block = block;
		}

		string GenerateCode();
};

class InputStatement : public Statement{
	public:
		string id;
		string str;

		InputStatement(string id, string str){
			this->id = id;
			this->str = str;
		}

		string GenerateCode(){ return ""; };
};

class ContinueStatement : public Statement{
	public:
	
		ContinueStatement(){

		}

		string GenerateCode(){ return ""; };
};

class BreakStatement : public Statement{
	public:
	
		BreakStatement(){

		}

		string GenerateCode();
};

class ReturnStatement : public Statement{
	public:
		Expression* expr = NULL;
		ReturnStatement(Expression* expr){
			this->expr = expr;
		}
		string GenerateCode();
};


/* Global Scope */

class GlobalVariable {
public:
	string id;
	Expression* expr = NULL;

	GlobalVariable* nextVariable = NULL;

	GlobalVariable(string id, Expression* expr){
		this->id = id;
		this->expr = expr;
	}

	string GenerateCode();
};


class Parameter {
	public:
		string type;
		Expression* expr = NULL;
		Parameter* nextParameter;

		Parameter(string type, Expression* expr){
			this->type = type;
			this->expr = expr;
		}

		string GenerateCode();
		int ParamsSize();
};

class DeclaredParameter{
	public:
		DeclaredParameter * nextDeclaredParameter = NULL;
		string type;
		string id;

		DeclaredParameter(string type, string id){
			this->type = type;
			this-> id = id;
		}
};

class Method {
	public:
		string id;
		DeclaredParameter* params = NULL;
		Statement* block = NULL;

		Expression* ret_val = NULL;
		Method* nextMethod = NULL;

		Method(string id, DeclaredParameter* params, Statement* block){
			this->id = id;
			this->params = params;
			this->block = block;
		}

		string GenerateCode();
};

class Class {
	public:
		GlobalVariable* globalVarList = NULL;
		Method* methodList = NULL;
		
		Class(GlobalVariable* globalVarList, Method* methodList){
			this->globalVarList = globalVarList;
			this->methodList = methodList;
		}

		string GenerateCode();
};

class MethodInvoke : public Statement{
	public:
		string id;
		Parameter* params = NULL;

		MethodInvoke(string id, Parameter* params){
			this->id = id;
			this->params = params;
		}

		string GenerateCode();
};


class FunctionInvoke : public Expression{
	public:
		string id;
		Parameter* params = NULL;

		Method * nextMethod = NULL;

		FunctionInvoke(string id, Parameter* params){
			this->id = id;
			this->params = params;
		}

		void GenerateCode(string& code, string &addr);
};