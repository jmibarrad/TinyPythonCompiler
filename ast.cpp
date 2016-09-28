#include <sstream>
#include <string>
#include "ast.h"

map<string, int> symbol_table;
map<string, string> gbl_symbol_table;
vector<map<string, string>> $scopes;

int tmp_offset = 4;
int gbl_offset = 4;
int lbl_count = 0;
int base_addr = 10000000;

void resetOffset(){
	tmp_offset = 4;
}

string newLabel(){
	lbl_count++;
	return "lbl_" + to_string(lbl_count);
}

string newTemp(){
	string temp = "dword[ebp - " + to_string(tmp_offset) + "]";
	tmp_offset += 4;
	return temp;
}

string resolveId(string id){

	int scopes = $scopes.size() - 1;
	for(int i = scopes; i >= 0; i--) {
		if($scopes[i].find(id) != $scopes[i].end()){
			return "dword[ebp " + $scopes[i][id] + "]";
		}
	}

	if(gbl_symbol_table.find(id) != gbl_symbol_table.end()){
	  return  gbl_symbol_table[id];
	}

	$scopes[scopes][id] = " - " + to_string(tmp_offset);
	string addr = "dword[ebp - " + to_string(tmp_offset) + "]";
	tmp_offset += 4;
	return addr;
}


/* Arithmetic Nodes */

void AddExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"mov eax, " + addr1 + "\n" +
		"add eax, " + addr2 + "\n" +
		"mov " +addr + ", eax \n";
}

void SubExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"mov eax, " + addr1 + "\n" +
		"sub eax, " + addr2 + "\n" +
		"mov " +addr + ", eax \n";
}

void MultExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"mov eax, " + addr1 + "\n" +
		"imul eax, " + addr2 + "\n" +
		"mov " +addr + ", eax \n";
}

void DivExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"mov eax, " + addr1 + "\n" +
		"xor edx, edx \n" +
		"idiv " + addr2 + "\n" +
		"mov " +addr + ", eax \n";
}

/* Relational Nodes */

void GTExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + addr1 + "\n" +
		"cmp eax, " + addr2 + "\n" +
		"setg bl \n" +
		"mov " +addr + ", ebx \n";
}

void LTExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + addr1 + "\n" +
		"cmp eax, " + addr2 + "\n" +
		"setl bl \n" +
		"mov " +addr + ", ebx \n";
}

void GTEExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + addr1 + "\n" +
		"cmp eax, " + addr2 + "\n" +
		"setge bl \n" +
		"mov " +addr + ", ebx \n";
}

void LTEExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + addr1 + "\n" +
		"cmp eax, " + addr2 + "\n" +
		"setle bl \n" +
		"mov " +addr + ", ebx \n";
}

void EQExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + addr1 + "\n" +
		"cmp eax, " + addr2 + "\n" +
		"sete bl \n" +
		"mov " +addr + ", ebx \n";
}

void NEQExpr::GenerateCode(string& code, string &addr)
{
	string code1, addr1;
	string code2, addr2;
	
	addr = newTemp();

	left_expr->GenerateCode(code1, addr1);
	right_expr->GenerateCode(code2, addr2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + addr1 + "\n" +
		"cmp eax, " + addr2 + "\n" +
		"setne bl \n" +
		"mov " +addr + ", ebx \n";
}

void IDExpr::GenerateCode(string& code, string &addr)
{
	code = "";
	addr = resolveId(Id);
}

void BoolExpr::GenerateCode(string& code, string &addr){
	code = "";
	addr = value ? "1" : "0";
}

void NumExpr::GenerateCode(string& code, string &addr)
{
	code = "";
	addr = to_string(value);
}

string AssignStatement::GenerateCode()
{
	string code, addr;

	expr->GenerateCode(code, addr);
	stringstream ss;
	string new_addr = resolveId(this->id);
	ss << code << endl
	<< "mov eax, " << addr << endl
	<< "mov " << new_addr << ", eax" << endl;

	if(nextStatement != NULL)
		ss << nextStatement->GenerateCode();

	return ss.str();
}

string BreakStatement::GenerateCode()
{
      
      return "nop";
}

string PrintStatement::GenerateCode()
{
	string code, addr;

	expr->GenerateCode(code,addr);
	stringstream ss;
	ss << code << endl
	<< "#show " << addr << endl; 
	/*      << "push " << addr << endl
	<< "push format_string" << endl //TODO: replace this for easy asm code
	<< "call @libc.printf" << endl
	<< "add esp, 8" << endl;*/

	if(nextStatement != NULL)
		ss << nextStatement->GenerateCode();

	return ss.str();
}

string ReturnStatement::GenerateCode(){
      
	stringstream ss;

	if(expr != NULL){
		string code, addr;
		expr -> GenerateCode(code, addr);
		ss << code << endl;
		ss << "mov eax, " << addr << endl;
	}

	ss << "leave" << endl
	<< "ret" << endl;

	return ss.str();
}

string IfStatement::GenerateCode()
{
	$scopes.push_back(map<string, string>());

	string lbl_else = newLabel();
	string lbl_endif = newLabel();
	string code, addr;

	condition->GenerateCode(code, addr);
	stringstream ss;
	ss << code << endl
	<< "cmp " << addr << ", 0" << endl
	<< "je " << lbl_else << endl;
/*
	while(trueBlock != NULL)
	{*/
	if(trueBlock != NULL)
		ss << trueBlock->GenerateCode() << endl;
/*		trueBlock = trueBlock->nextStatement;
	}*/

	ss << "jmp " << lbl_endif << endl
	<< lbl_else << ":" << endl;
	if(falseBlock != NULL)
	{
		/*while(falseBlock != NULL)
		{*/
			ss << falseBlock->GenerateCode() << endl;
		/*	falseBlock = falseBlock->nextStatement;
		}*/
	}

	ss << lbl_endif << ":" << endl;
	
	if(nextStatement != NULL)
		ss << nextStatement->GenerateCode() << endl;
	$scopes.pop_back();

	return ss.str();
}

string WhileStatement::GenerateCode(){
	
	$scopes.push_back(map<string, string>());

	string lbl_while = newLabel();
	string lbl_endwhile = newLabel();
	string code, addr;

	condition->GenerateCode(code, addr);
	stringstream ss;
	ss << lbl_while << ":" << endl
	<< code << endl
	<< "cmp " << addr << ", 0" << endl
	<< "je " << lbl_endwhile << endl;
	
	//while(block != NULL)
	//{
		ss << block->GenerateCode() << endl;
		//block = block->nextStatement;
	//}

	ss << "jmp " << lbl_while << endl
	<< lbl_endwhile << ":" << endl;

	if(nextStatement != NULL)
		ss << nextStatement->GenerateCode() << endl;
	$scopes.pop_back();

	return ss.str();
}

string ForStatement::GenerateCode(){
	
	$scopes.push_back(map<string, string>());

	string code_i, addr_i;
	string code_c, addr_c;
	iterator->GenerateCode(code_i, addr_i);
	condition->GenerateCode(code_c, addr_c);
	
	string lbl_for = newLabel();
	string lbl_endfor = newLabel();
	string tmp_for = newTemp();
	
	stringstream ss;
	ss << code_i << endl 
	<< code_c << endl
	<< "mov " << tmp_for << ", " << addr_i << endl
	<< lbl_for << ":" << endl
	<< "cmp "<< tmp_for << ", " << addr_c << endl
	<< "je " << lbl_endfor << endl
	<< block->GenerateCode() << endl
	<< "inc " << tmp_for << endl
	<< "jmp " << lbl_for << endl
	<< lbl_endfor << ":" << endl;

	if(nextStatement != NULL)
		ss << nextStatement->GenerateCode() << endl;
	
	$scopes.pop_back();
	return ss.str();
}

string Method::GenerateCode(){

	stringstream ss;
	map <string, string> methodParams;
	int paramOffset = 8;

	while(params != NULL){
		methodParams[params->id] = "+ " + to_string(paramOffset);
		paramOffset += 4;

		if(params->nextDeclaredParameter != NULL)
			params = params->nextDeclaredParameter;
		else 
			break;
	}

	$scopes.push_back(methodParams);
	ss << id << ":" << endl 
	<< "push ebp" << endl 
	<< "mov ebp, esp" << endl;
	string methodBlock;
	if(block != NULL) methodBlock = block->GenerateCode();
	string espStack = "sub esp, " + to_string(tmp_offset - 4);
	if(tmp_offset > 4) ss << espStack << endl;
	ss << methodBlock << endl;
	resetOffset();
	$scopes.pop_back();
	if(nextMethod != NULL) ss << endl << nextMethod->GenerateCode();

	return ss.str();
}

string Parameter::GenerateCode() {

	stringstream ss;
	string code, addr;

	if(nextParameter != NULL) ss << nextParameter->GenerateCode();
	expr->GenerateCode(code, addr);
	ss << code << "push " << addr << endl;

	return ss.str();
}


int Parameter::ParamsSize(){
	if(nextParameter != NULL){
		return 1 + nextParameter->ParamsSize();
	}
	return 1;
}

void FunctionInvoke::GenerateCode(string& code, string &addr){

	string tmp_func = newTemp();
	string param_code = "";

	if(params != NULL)	param_code = params->GenerateCode();
	code = param_code + "call " + id + "\n";
	if(params != NULL) code += "add esp, " + to_string(params->ParamsSize() * 4) + "\n";

	code += "mov " + tmp_func + ", eax \n";
	addr = tmp_func;
}

string MethodInvoke::GenerateCode(){
	stringstream ss;

	if(params != NULL) ss << params->GenerateCode();
	ss << "call " << id << endl;
	if(params != NULL) ss << "add esp, " << to_string(params->ParamsSize() * 4) << endl;

	if(nextStatement != NULL)
		ss << nextStatement->GenerateCode() << endl;
	
	return ss.str();
}

string Class::GenerateCode(){//

	stringstream ss;
	if(globalVarList != NULL)
		ss << globalVarList-> GenerateCode();
	
	resetOffset();
	ss << "call main" << endl 
	<< "#stop" << endl
	<< methodList->GenerateCode() << endl;

	 return ss.str();
}

string GlobalVariable::GenerateCode(){

	stringstream ss;
	string code, addr;
	expr->GenerateCode(code, addr);
	string gblAddr = "dword[ 0x" + to_string(base_addr) + "]";
	gbl_symbol_table[id] = gblAddr;
	base_addr += 4;
	ss << code 
	<< "mov eax, " << addr << endl
	<< "mov " << gblAddr << ", eax" << endl;

	if(nextVariable != NULL)
		ss << nextVariable->GenerateCode();

	return ss.str();
}
