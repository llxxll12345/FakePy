#include "stdafx.h"
#include "nodeParse.h"
#include "../exceptions/syntaxExceptionh.h"
#include "../exceptions/parameterException.h"
#include "cstdio"
#include "../exceptions/IndentException.h"
#include "../types/Bool.h"
#include "../types/Int.h"
#include "../types/None.h"
#include "../types/Double.h"
#include "../types/List.h"
#include "../types/Character.h"
#include "../types/String.h"
#include "exprUtils.h"
#include "condition.h"
#include <map>
#include <algorithm>

unordered_map<string, varType*> varMap;
pair<char, char> pairedToken[] = { { '"', '"' },{ '\'', '\'' },{ '(', ')' },{ '[', ']' },{ '{', '}' } };

string reserved[] = { "return", "def", "True", "False", "import", "from", "for", "while", "in", "assert", "and", "or", "not"};
string binOps[] = { "==", "!=", "<=", ">=", "||", "&&" };
char   sigOps[] = { '*', '/', '%', '+', '-', '>', '<' };
char   complex[] = { '[', ']', '(', ')', '{', '}', ',' };

// func list
// varType* nodenodeParser::parseStr(string str)
// bool checkFunDeclare(string str)
// assignFunc(str)
// str cleanStr(str)
// exprNode buildTree(str)
// runCmd(str)
// findPair(ch, ch, str, int)
// getVarType(str)
// bool isVar(str)
// bool isBasicType(str)
// bool checkReturn(str)
// getVarVal(string &str, unordered_map<string, varType*>* varMap)
// make assignment
// get temp void;
// get default val
// get idx range getRange

// Func 
// parseInto

void nodeParser::test() {
	printf("True: %d\n", isBoolean("Ture"));
	printf("12: %d\n", isInteger("12"));
	printf("12.123: %d\n", isDouble("12.123"));
	printf("asdflerea: %d\n", isString("\"asdflerea\""));
	
	string s = "    asdf   asdf    asdf 1234 ";
	trim(s);
	printf("%s|\n", s.c_str());
	printf("_asdf1234: %d\n", isVar("_asdf1234"));

	printf("Getbasic Type\n");
	printf("%s\n", getBasicType("12.123")->toString().c_str());
	printf("%s\n", getBasicType("True")->toString().c_str());
	printf("%s\n", getBasicType("12")->toString().c_str());
	printf("%s\n", getBasicType("\'asdf\'")->toString().c_str());

	printf("cleanStr: %s\n", cleanStr("  \"asdf  \'asdf\'   asdf\"  a = 123 for i in [1,2,3]:").c_str());
	printf("checkReturn: %d\n", checkReturn("return "));

	varMap.insert(make_pair("asdf", &Int(6)));
	printf("getVal :%s\n", getVarVal("asdf")->toString().c_str());

}

varType* nodeParser::parseStr(string str) {
	if ((str.size() > 0) && str[0] == ' ' || str[0] == '\t') {
		throw new IndentException();
	}
	trim(str);
	varType* ret = nullptr;
	if (!str.size())
		return nullptr;

	if (checkFunDeclare(str)) {
		printf("deal with function\n");
		string funcName = cleanStr(str.substr(4, str.size() - 4));
		printf("Func def: %s\n", funcName.c_str());
		Function* newFunc = NULL;
		try { newFunc = assignFun(funcName); }
		catch(generalException ex){
			varMap.erase(funcName);
			printf("Error is assignFun\n");
		}
		if (newFunc != nullptr) {
			bool parseOk = newFunc->buildFun();
			if (!parseOk) {
				printf("Error in implementation of function\n");
				varMap.erase(funcName);
				throw new syntaxException();
			}
		}
		ret = (varType*) new Void();
		ret->setTempVar(1);
		return ret;
	}
	if (str.substr(0, 2) == "if") {
		string condition = cleanStr(str.substr(2, str.size() - 2));
		printf("if statement\n");
		if (condition[condition.size() - 1] != ':') {
			printf("If statement should end with ':' !\n");
			throw new syntaxException();
		}
		ifBlock* block = assignIf(condition.substr(0, condition.size() - 1));
		varType* ret = block->runIf(&varMap);
		if (ret->myType != NonType) {
			printf("No return statement should appear outside a function.");
			throw new parameterException();
		}
		
		ret = (varType*) new Void();
		ret->setTempVar(1);
		return ret;
	}
	
	if (str.substr(0, 3) == "for") {

	}

	/*
		startConditionBlock
		startIterativeBlock
	*/

	string line = cleanStr(str);
	exprNode* root = buildTree(line);
	root->printT();
	ret = runCmd(root, &varMap);
	printf("TypeVal: %d\n", ret->myType);
	if (!ret->getTempVar()) {   
		ret = ret->clone();
		ret->setTempVar(1);
	}
	return ret;
}


string nodeParser::cleanStr(const string &str) {
	string ret = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == ' ') continue;
		if (str[i] == '"' || str[i] == '\'') {
			int end = findPair(str[i], str[i], str, i + 1);
			if (end == -1) {
				printf("Unpaired \" or \' \n ");
				throw new syntaxException();
			}
			ret += str.substr(i, end - i + 1);
			i = end;
			continue;
		}
		ret += str[i];
	}
	return ret;
}
	

varType* nodeParser::getBasicType(const string &str) {
	varType* ret = nullptr;
	if (isInteger(str)) {
		int val = stoi(str);
		ret = (varType*) new Int(val);
	}
	else if (isBoolean(str)) {
		ret = (varType*) new Bool(str == "True");
	}
	else if (isString(str)) {
		if (str.length() == 3)	//'c'
			ret = (varType*) new Character(str[1]);
		else {					// 'asdf'
			string content = str.substr(1, str.size() - 2);
			ret = (varType*) new String(content);
		}
	}
	else if (isDouble(str)) {
		double val = stod(str);
		ret = (varType*) new Double(val);
	}
	return ret;
}

bool nodeParser::isVar(const string& str) {
	if ((!isLetter(str[0])) && str[0] != '_')
		return 0;

	for (int i = 1; i < str.length(); i++) {
		if ((!isDigit(str[i])) && (!isLetter(str[i])) && str[i] != '_')
			return false;
	}

	for (string res : reserved)		// reserved words
		if (str == res)
			return false;

	return true;
}

bool nodeParser::isBasicType(const string &str) {
	if (!str.size()) return false;
	if (isInteger(str) || isBoolean(str) || isString(str) || isDouble(str)) return true;
	return false;
}

// s-input, a-pattern
int nodeParser::getFirstOccr(const string &s, const string &a) {
	int *lps = (int*)malloc(sizeof(int) * a.size());
	int len = 0;
	lps[0] = 0;
	for (int i = 0; i < a.size(); i++) {
		while (len > 0 && a[len] != a[i]) len = lps[len];
		if (a[len] == a[i]) {
			lps[i] = ++len;
		}
		else lps[i] = 0;
	}
	for (int i = 0, j = 0; i < s.size();) {
		if (s[i] == a[j]) {
			i++;
			j++;
		}
		if (j == a.size()) {
			return j - a.size() + 1;
		}
		else if (i < s.size() && s[i] != a[j]) {
			if (j > 0)
				j = lps[j - 1];
			else 
				i++;
		}
	}
	return -1;
}


// check return format
bool nodeParser::checkReturn(const string &str) {
	//int firstSpace = str.find_first_of(" \t");
	int fSpace = str.find_first_of(" ");
	if (fSpace == string::npos)
		return false;							// No space in this line
	string fWord = str.substr(0, fSpace);
	return fWord == "return";
}



bool nodeParser::checkFunDeclare(const string & str) {
	//int firstSpace = str.find_first_of(" \t");
	int fSpace = str.find_first_of(" ");
	if (fSpace == string::npos)
		return false;							// No space in this line
	string fWord = str.substr(0, fSpace);
	return fWord == "def";
}


varType* nodeParser::getVarVal(const string &str, unordered_map<string, varType*> *v) {
	if (v == nullptr)
		v = &varMap;
	unordered_map<string, varType*>::iterator iter = v->find(str);
	if (iter == v->end()) {
		if (v != &varMap)
			return getVarVal(str, nullptr);
		else
			return nullptr;
	}
	return iter->second;
}


void nodeParser::memFree(unordered_map<string, varType*>* v) {
	if (v == nullptr)
		v = &varMap;
	for (auto p : *v) {
		delete p.second;
	}
}

varType* nodeParser::runCmd(exprNode* root, unordered_map<string, varType*>* v) {
	printf("SIZE: %d\n", v->count("a"));
	if (root == nullptr) {
		printf("An empty root!\n");
		throw new parameterException();
	}
	string line = root->getVal();
	varType  *ret, *asgn, *var, *idx, *lval, *rval;

	exprNode *lchd, *rchd, *schd;
	exprNode *valT, *varT;
	List* varl;
	vector<varType*> idv;

	printf("line val: %s\n", line.c_str());
	switch(line.size()) {
	case 1:
		switch (line[0]) {
		case '=':
			if (root->chdSz() != 2) {
				printf("Attempting to assign with operator '='. Missing l or r value.\n");
				throw new parameterException();
			}
			varT = root->getChd(0);
			valT = root->getChd(1);
			asgn = runCmd(valT, v);
			
			if (varT->getVal() == "[]*") {
				if (varT->chdSz() != 2) {
					printf("Missing list var or index\n");
					throw new syntaxException();
				}
					
				var = runCmd(varT->getChd(0), v);
				if (var->myType != Types::Lists) {
					printf("Not a list! Can't be indexed. \n");
					throw new syntaxException();
				}

				varl = (List*) var;
				if (varl->isTuple) {
					printf("Tuple is not modifiable!\n");
					throw new syntaxException();
				}
				idx = runCmd(varT->getChd(1), v);
				varl->set(idx, asgn);
				return (varType*) getVoid();
			}

			if (isVar(varT->getVal()) && !varT->hasChd()) {
				string varName = varT->getVal();
				assignVar(varName, asgn, v);
				return (varType*)getVoid();
			}
			printf("Unrecognizable r-value.\n");
			throw new parameterException();
			break;
		case '+':
			if (root->chdSz() != 2) {
				printf("Missing a value for +. \n");
				throw new syntaxException();
			}
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval  = runCmd(lchd, v);
			rval  = runCmd(rchd, v);
			printf("+val: %d %d\n", ((Int*) lval)->getVal(), ((Int*) rval)->getVal());
			return (*lval) + rval;
			break;
		case '-':
			if (root->chdSz() != 2)
				throw new syntaxException();
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			printf("-val: %d %d\n", ((Int*)lval)->getVal(), ((Int*)rval)->getVal());
			return (*lval) - rval;
			break;
		case '*':
			if (root->chdSz() != 2)
				throw new syntaxException();
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) * rval;
			break;
		case '/':
			if (root->chdSz() != 2)
				throw new syntaxException();
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) / rval;
			break;
		case '%':
			if (root->chdSz() != 2)
				throw new syntaxException();
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) % rval;
			break;
		case '>':
			if (root->chdSz() != 2)
				throw new syntaxException();
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) > rval;
			break;
		case '<':
			if (root->chdSz() != 2)
				throw new syntaxException();
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) < rval;
			break;
		case '!':
			if (root->chdSz() != 1)
				throw new syntaxException();
			schd = root->getChd(0);
			ret = runCmd(schd, v);
			return !(*ret);
			break;
		default:
			ret = getDefault(line, v);
			if (ret == nullptr) {
				printf("There is no default value for %s.\n", line.c_str());
				throw new syntaxException();
			}
			return ret;
			break;
		}
		break;
	case 2:
		if (line[1] == '=') {
			if (isOp(line[0])) {
				if (root->chdSz() != 2) {
					printf("At 'x=' : Missing l or r value!\n");
					throw new syntaxException();
				}
					
				valT = root->getChd(1);
				varT = root->getChd(0);

				if (varT->getVal() == "[]*") {
					varl = (List*) runCmd(varT->getChd(0), v);
					if (varl->isTuple) {
						printf("Tuple is not modifiable!\n");
						throw new syntaxException();
					}
				}
				asgn = runCmd(varT, v);
				ret  = runCmd(valT, v);

				

				switch (line[0]) {
				case '*':
					asgn = (*asgn) * ret;
					break;
				case '/':
					asgn = (*asgn) / ret;
					break;
				case '+':
					asgn = (*asgn) + ret;
					break;
				case '-':
					asgn = (*asgn) - ret;
					break;
				case '%':
					asgn = (*asgn) % ret;
					break;
				}
				if (varT->getVal() == "[]*") {
					if (varT->chdSz() != 2) {
						printf("At '[]*' : Missing l or r value!\n");
						throw new syntaxException();
					}
					lchd = varT->getChd(0);
					var = runCmd(lchd, v);

					if (var->myType != Types::Lists) {
						printf("Attempting to use [] to get value from a list : Variable is not a list\n");
						throw new syntaxException();
					}

					varl = (List*)var;
					rchd = varT->getChd(1);
					idx = runCmd(rchd, v);
					varl->set(idx, asgn);
					return (varType*) getVoid();
				}
				if (isVar(varT->getVal()) && !varT->hasChd()) {
					assignVar(varT->getVal(), asgn, v);
					return (varType*) getVoid();
				}
				printf("Unrecognizable operator. Must be one of '+=', '-=', '*=', '/=', '%='\n");
				throw new parameterException();
			}
			else if (line[0] == '=' || line[0] == '!' || line[0] == '>' || line[0] == '<') {
				if (root->chdSz() != 2) {
					printf("Attemping to do comparison. Missing l or r value\n");
					throw new parameterException();
				}
				lchd = root->getChd(0);
				rchd = root->getChd(1);
				lval = runCmd(lchd, v);
				rval = runCmd(rchd, v);
				switch (line[0]) {
				case '=':
					return (*lval) == rval;
					break;
				case '!':
					return (*lval) != rval;
					break;
				case '>':
					return (*lval) >= rval;
					break;
				case '<':
					return (*lval) <= rval;
					break;
				}
			}
			else {

				printf("Unrecognizable syntax\n");
				throw new syntaxException();
			}
		}
		else if (line == "||") {
			if (root->chdSz() != 2) {
				printf("Attemping to do '||' comparison. Missing l or r value\n");
				throw new parameterException();
			}
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) || rval;
		}
		else if (line == "&&") {
			if (root->chdSz() != 2) {
				printf("Attemping to do '&&' comparison. Missing l or r value\n");
				throw new parameterException();
			}
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			return (*lval) && rval;
		}
		else if (line == "[]") {
			ret = (varType*) new List();
			for (auto child : root->getChds()) {
				if (child == nullptr) {
					printf("Null memeber in the list!\n");
					throw new syntaxException();
				}
				((List*) ret)->add(runCmd(child, v));
			}
			return ret;
		}
		else if (line == "()") {
			ret = (varType*) new List();
			for (auto child : root->getChds()) {
				if (child == nullptr) {
					printf("Null member in the tuple!\n");
					throw new syntaxException();
				}
				((List*)ret)->add(runCmd(child, v));
			}
			((List*)ret)->isTuple = 1;
			return ret;
		}
		else if (line == "{}") {
			ret = (varType*) new List();
			for (auto child : root->getChds()) {
				if (child == nullptr)
					throw new syntaxException();
				((List*)ret)->add(runCmd(child, v));
			}
			return ret;
		}
		else {
			ret = getDefault(line, v);
			if (ret == nullptr) {
				printf("There is no default value for %s.\n", line.c_str());
				throw new syntaxException();
			}
			return ret;
		}
		break;
	default:
		if (line == "[]*") {
			if (root->chdSz() != 2) {
				printf("Attempt to access a list memver: Missing l-val or r-val.\n");
				throw new syntaxException();
			}

			lchd = root->getChd(0);
			varType*  var  = runCmd(lchd, v);
			if (var->myType != Types::Lists) {
				printf("r-val is not a list of vars\n");
				throw new syntaxException();
			}

			varl = (List*)var;

			rchd = root->getChd(1);
			idv = parseRange(rchd, v);
			return (*varl)[idv];
		} 
		else if (line == "()*") {
			if (root->chdSz() != 2) {
				printf("Unacceptable function declaration.\n");
				throw new parameterException();
			}
			lchd = root->getChd(0);
			rchd = root->getChd(1);
			lval = runCmd(lchd, v);
			rval = runCmd(rchd, v);
			if (lval->myType != Types::Functions) {
				printf("lval->type %d\n", lval->myType);
				printf("l-val is not a callable function!\n");
				throw new syntaxException();
			}
			if (rval->myType != Types::Lists) {
				printf("r-val is not a list of vars\n");
				throw new syntaxException();
			}

			Function* func = (Function*)lval;
			vector<varType*> vars = ((List*)rval)->getList();
			//printf("Vars: %s\n", ((List*)rval)->toString().c_str());
			return func->runFun(vars);
		}
		ret = getDefault(line, v);
		if (ret == nullptr) {
			printf("There is no default value for %s.\n", line.c_str());
			throw new syntaxException();
		}
		return ret;
	}
}

varType* nodeParser::getDefault(const string& str, unordered_map<string, varType*>* v) {
	if (isVar(str))
		return getVarVal(str, v);
	// basic type
	if (isBasicType(str))
		return getBasicType(str);
	return nullptr;
}

vector<varType*> nodeParser::parseRange(exprNode* root, unordered_map<string, varType*>* v) {
	vector<varType*> idv;
	parseRange_until(root, idv, v);
	return idv;
}

void nodeParser::parseRange_until(exprNode* root, vector<varType*>& idv, unordered_map<string, varType*>* v) {
	if (root == nullptr) {
		idv.push_back((varType*) getVoid());
		return;
	}
	exprNode *lchd, *rchd;

	string line = root->getVal();
	if (line == ":") {
		lchd = root->getChd(0);
		rchd = root->getChd(1);
		parseRange_until(lchd, idv, v);
		parseRange_until(rchd, idv, v);
	}
	else if (line == "")
		idv.push_back((varType*) getVoid());
	else
		idv.push_back(runCmd(root, v));
}

Void* nodeParser::getVoid() {
	Void* ret = new Void();
	ret->setTempVar(1);
	return ret;
}

exprNode* nodeParser::buildTree(const string &str) {
	if (str == "")
		return nullptr;
	//printf("str: %s\n", str.c_str());
	if (str[0] == '(' && findPair('(', ')', str, 1) == str.size() - 1) {
		string content = str.substr(1, str.size() - 2);
		return buildClosedTree(content, "()");
	}
	if (str[0] == '[' && findPair('[', ']', str, 1) == str.size() - 1) {
		string content = str.substr(1, str.size() - 2);
		return buildClosedTree(content, "[]"); 
	}
	if (str[0] == '{' && findPair('{', '}', str, 1) == str.size() - 1) {
		string content = str.substr(1, str.size() - 2);
		return buildClosedTree(content, "{}");
	}
	
	auto idr = getPairIndex(str);
	int i, idx;
	// check for assigners and for ':' seprator
	for (i = str.size() - 1, idx = idr.size() - 2; i >= 0; i--) {
		if (str[i] == '=') {
			if (i > 0) {
				if (str[i - 1] == '=' || str[i - 1] == '!' || str[i - 1] == '>' || str[i - 1] == '<') {
					string val = str.substr(i - 1, 2);
					//printf("VAL: %s\n", val.c_str());
					string lexpr = str.substr(0, i - 1);
					string rexpr = str.substr(i + 1, str.size() - i - 1);
					exprNode* lval = buildTree(lexpr);
					exprNode* rval = buildTree(rexpr);
					exprNode* ret = new exprNode(val, lval, rval);
					return ret;
				}
				if (str[i - 1] == '*' || str[i - 1] == '/' || str[i - 1] == '+' || str[i - 1] == '-' || str[i - 1] == '%') {
					string val = str.substr(i - 1, 2);
					//printf("VAL: %s\n", val.c_str());
					string lexpr = str.substr(0, i - 1);
					string rexpr = str.substr(i + 1, str.size() - i - 1);
					exprNode* lval = buildTree(lexpr);
					exprNode* rval = buildTree(rexpr);
					exprNode* ret = new exprNode(val, lval, rval);
					return ret;
				}
			}
			string lexpr = str.substr(0, i);
			string rexpr = str.substr(i + 1, str.size() - i - 1);
			exprNode* lval = buildTree(lexpr);
			exprNode* rval = buildTree(rexpr);
			return new exprNode("=", lval, rval);
		}
		if (str[i] == ':') {
			string lexpr = str.substr(0, i);
			string rexpr = str.substr(i + 1, str.size() - i - 1);
			exprNode* lval = buildTree(lexpr);
			exprNode* rval = buildTree(rexpr);
			return new exprNode(":", lval, rval);
		}
		if (idx >= 0 && idr[idx].second.second == i)
			i = idr[idx--].second.first;
	}
	// check for Comparers
	for (i = str.size() - 1, idx = idr.size() - 1; i > 0; i--) {
		string curOp = str.substr(i - 1, 2);
		for (string op : binOps)
			if (curOp == op) {
				string lexpr = str.substr(0, i - 1);
				string rexpr = str.substr(i + 1, str.size() - i - 1);
				exprNode* lval = buildTree(lexpr);
				exprNode* rval = buildTree(rexpr);
				return new exprNode(curOp, lval, rval);
			}
		if (idx >= 0 && idr[idx].second.second == i)
			i = idr[idx--].second.first;
	}
	// check for + and -
	for (i = str.size() - 1, idx = idr.size() - 1; i >= 0; i--) {
		if (str[i] == '+' || str[i] == '-') {
			if (i != 0) {
				string lexpr = str.substr(0, i);
				string rexpr = str.substr(i + 1, str.size() - i - 1);
				exprNode* lval = buildTree(lexpr);
				exprNode* rval = buildTree(rexpr);
				return new exprNode(string(1, str[i]), lval, rval);
			}
			else
				return new exprNode(string(1, str[0]), new exprNode("0"), buildTree(str.substr(1)));
		}
		if (idx >= 0 && idr[idx].second.second == i)
			i = idr[idx--].second.first;
	}
	for (i = str.size() - 1, idx = idr.size() - 1; i >= 0; i--) {
		if (str[i] == '*' || str[i] == '/' || str[i] == '%') {
			string lexpr = str.substr(0, i);
			string rexpr = str.substr(i + 1, str.size() - i - 1);
			exprNode* lval = buildTree(lexpr);
			exprNode* rval = buildTree(rexpr);
			return new exprNode(string(1, str[i]), lval, rval);
		}
		if (idx >= 0 && idr[idx].second.second == i)
			i = idr[idx--].second.first;
	}
	// check for indexers and function calls
	for (i = idr.size() - 1; i >= 0; i--) {
		auto range = idr[i];
		if (range.second.first != 0) {
			if (range.first == '[') {
				if (range.second.second == str.size() - 1) {
					string lexpr = str.substr(0, range.second.first);
					string rexpr = str.substr(range.second.first + 1, range.second.second - range.second.first - 1);
					exprNode* lval = buildTree(lexpr);
					exprNode* rval = buildTree(rexpr);
					return new exprNode(string("[]*"), lval, rval);
				}
				else
					throw new parameterException();
			}
			if (range.first == '(') {
				if (range.second.second == str.size() - 1) {
					string lexpr = str.substr(0, range.second.first);
					string rexpr = str.substr(range.second.first + 1, range.second.second - range.second.first - 1);
					//printf("l: %s, r: %s\n", lexpr.c_str(), rexpr.c_str());
					exprNode* lval = buildTree(lexpr);
					exprNode* rval = buildClosedTree(rexpr, "()");
					
					return new exprNode(string("()*"), lval, rval);
				}
				else
					throw new parameterException();
			}
			
		}
	}
	
	if (str[0] == '!')
		return new exprNode("!",  buildTree(str.substr(1, str.length() - 1)));
	if (isVar(str) || isBasicType(str))
		return new exprNode(str);

	printf("Unable to parse str |%s|\n, isVar %d\n", str.c_str(), isVar(str));
	throw new parameterException();
}

exprNode* nodeParser::buildClosedTree(string &str, const string &val) {
	exprNode* root = new exprNode(val);
	printf("closed str: %s\n", str.c_str());
	if (str == "")
		return root;
	auto idr = getPairIndex(str);
	int i, idx, last;
	last = 0;
	for (i = 0, idx = 0; i < str.size(); i++) {
		if (str[i] == ',') {
			if (i == last) {
				printf("empty item in a list.\n");
				throw new parameterException();
			}
			root->addChd(buildTree(str.substr(last, i - last)));
			last = i + 1;
		} else if (idx < idr.size() && idr[idx].second.first == i)
			i = idr[idx++].second.second;
	}
	root->addChd(buildTree(str.substr(last, str.size() - last)));
	return root;
}

vector<pair<char, pair<int, int>>> nodeParser::getPairIndex(const string &str) {
	vector<pair<char, pair<int, int>>> idr;
	int ltk, rtk;
	rtk = -1;
	for (int i = 0; i < str.length(); i++) {
		for (int j = 0; j < 5; j++) {
			if (str[i] != pairedToken[j].first)
				continue;

			int rtk = findPair(pairedToken[j], str, i + 1);
			if (rtk == -1) {
				printf("Missing right token of %c\n", pairedToken[j].first);
				throw new syntaxException();
			}
			idr.push_back(make_pair(pairedToken[j].first, make_pair(i, rtk)));

			i = rtk;
			break;
		}
	}
	return idr;
}


int nodeParser::findPair(pair<const char, const char> pairtk, const string &str, int pos) {
	return findPair(pairtk.first, pairtk.second, str, pos);
}


int nodeParser::findPair(char ltk, char rtk, const string &str, int pos) {
	int ltkCnt = 0;
	if (ltk != '\"' && ltk != '\'') {
		char quote = 0;
		for (int i = pos; i < str.length(); i++) {
			if (quote == 0) {
				if (str[i] == rtk) {
					if (!ltkCnt)
						return i;
					ltkCnt--;
					continue;
				}
				else if (str[i] == ltk) {
					ltkCnt++;
				} 
				else if (str[i] == '\'' || str[i] == '"'){
					quote = str[i];
				}
			}
			else {
				if (str[i] == quote)
					quote = 0;
			}
		}
	}
	else {
		for (int i = pos; i < str.length(); i++) {
			if (str[i] == rtk) {
				if (!ltkCnt)
					return i;
				ltkCnt--;
				continue;
			}
			else if (str[i] == ltk) {
				ltkCnt++;
			}
		}
	}
	return -1;
}

void nodeParser::assignVar(const string& varName, varType* asgn, unordered_map<string, varType*>* v){
	varType* Var = getVarVal(varName);
	if (Var == nullptr && v != nullptr)
		(*v)[varName] = asgn;
	else
		varMap[varName] = asgn;
}



Function* nodeParser::assignFun(const string &str) {
	exprNode* root = buildTree(str);
	root->printT();
	if (!root || root->getVal() != ":") {
		printf("Missing ':' \n");
		throw new syntaxException();
	}

	exprNode *lchd, *rchd;

	rchd = root->getChd(1);
	if (rchd && rchd->getVal() != "") {
		printf("Right of ':' should be empty\n");
		throw new syntaxException();
	}

	lchd = root->getChd(0);
	if (lchd == nullptr || lchd->getVal() != "()*") {
		printf("Null expression or expression is not a function declaration. \n");
		throw new syntaxException();
	}

	exprNode* llchd = lchd->getChd(0);
	if (llchd == nullptr || llchd->chdSz() != 0) {
		printf("Invalid function declaration\n");
		throw new syntaxException();
	
	}
		
	string funName = llchd->getVal();
	if (!isVar(funName)) {
		printf("Invalid function name\n");
		throw new syntaxException();
	}
	if (getVarVal(funName) != nullptr) {
		printf("Alrady assigned!\n");
		throw new syntaxException();
	}

	vector<string> localVars;
	exprNode* localT = lchd->getChd(1);
	if ((!localT) || localT->getVal() != "()") {
		printf("Invalid local varaible Tree\n");
		throw new syntaxException();
	}
	for (auto var : localT->getChds()) {
		if (var == nullptr) {
			printf("Invalid var definition\n");
			throw new syntaxException();
		}

		string varName = var->getVal();
		if (!isVar(varName)) {
			printf("Not a varName\n");
			throw new syntaxException();
		}
		localVars.push_back(varName);
	}
	varMap[funName] = (varType*) new Function(localVars);
	return (Function*)(varMap[funName]);
}

varType* Function::runFun(vector<varType*> vars) {
	if (vars.size() != localVars.size()) {
		printf("Impatible parameters to run function!\n");
		throw new parameterException();
	}

	for (int i = 0; i < vars.size(); i++)
		localMap[localVars[i]] = vars[i];

	nodeParser parser;
	int ifIdx = 0;
	for (exprNode* line : lines) {
		try {
			if (line == nullptr)
				break;
			if (line->getVal() == "return") {
				exprNode* schd = line->getChd(0);
				if (line->chdSz() != 1 || schd == nullptr)
					break;
				return parser.runCmd(schd, &localMap);
			}
			else if (line->getVal() == "if") {
				//printf("If in fun\n");
				ifBlock* ib = ifBlocks[ifIdx++];
				varType* ret = ib->runIf(&localMap);
				//printf("ret type: %d\n", ret->myType);
				if (ret->myType != NonType) {
					return ret;
				}
				else {
					continue;
				}
			}
			varType* ret = parser.runCmd(line, &localMap);
			if (ret->isPrintable())
				printf("%s\n", ret->toString());
			if (ret->getTempVar())
				delete ret;
		}
		catch (generalException* ex) {
			string err = ex->what();
			printf("Error: %s\n", err.c_str());
		}
	}

	//parser.memFree(&localMap);
	return (varType*) new Void();
}

bool Function::buildFun(string preIndent) {
	string input;

	printf("... ");
	char ch;
	while ((ch = getchar()) != '\n') {
		input += ch;
	}

	nodeParser parser;

	while (input != "") {
		if (input[0] != ' ' && input[0] != '\t')
			return false;
		trim(input);
		exprNode* line;
		if (parser.checkReturn(input)) {
			string cinput = parser.cleanStr(input.substr(7, input.size() - 7));
			exprNode* sval = parser.buildTree(cinput);
			line = new exprNode("return", sval);
		} if (input.substr(0, 2) == "if") {
			if (input[input.size() - 1] != ':') {
				printf("If statement should end with ':' ");
				throw new syntaxException();
			}
			ifBlock *ib = parser.assignIf(input.substr(2, input.size() - 3));
			ifBlocks.push_back(ib);
			line = new exprNode("if");
		}
		else {
			line = parser.buildTree(parser.cleanStr(input));
		}

		lines.push_back(line);

		input = "";
		printf("... ");
		while ((ch = getchar()) != '\n') {
			input += ch;
		}
	}
	return true;
}

ifBlock* nodeParser::assignIf(const string &cd) {
	string condition = cd;
	trim(condition);
	cleanStr(condition);
	exprNode* conT = buildTree(condition);
	ifBlock *block = new ifBlock(conT);
	if (!block->buildIf("")) {
		printf("Build if block error.\n");
		throw new syntaxException();
	}
	return block;
}

varType* ifBlock::runIf(unordered_map<string, varType*> *v) {
	if (v == nullptr)
		v = &varMap;

	localMap = *v;
	//printf("local for a: %s\n", ((Int*) (*v)["a"])->toString().c_str());
	nodeParser parser;
	int index = 0, subIndex = 0;
	for (auto condition : conditionLine) {
		//printf("SIZE: %d\n", v->size());
		varType* ret = parser.runCmd(condition, v);
		if (ret->myType != Types::Boolean && ret->myType != Types::Integer) {
			printf("The condition line does not return a boolean value.\n");
			throw new parameterException();
		}
		bool verdict = ((Bool*)ret)->getVal();
		//printf("verdict: %d\n", verdict);
		if (verdict) {
			for (auto line : lines[index]) {
				printf("line %s\n", line->getVal().c_str());
				try {
					if (line == nullptr)
						continue;
					if (line->getVal() == "return") {
						exprNode* schd = line->getChd(0);
						if (line->chdSz() != 1 || schd == nullptr)
							return (varType*)&Void();
						return parser.runCmd(schd, &localMap);
					}
					else if (line->getVal() == "if") {
						ifBlock* subB = subBlocks[index][subIndex++];
						varType* ret = subB->runIf(v);
						if (ret->myType != Types::Voids)
							return ret;
					}
					varType* ret = parser.runCmd(line, &localMap);
					if (ret->isPrintable())
						printf("%s\n", ret->toString());
					if (ret->getTempVar())
						delete ret;
				}
				catch (generalException* ex) {
					string err = ex->what();
					printf("Error: %s\n", err.c_str());
				}
			}
			return (varType*)&None();
		}
		index++;
	}

	parser.memFree(&localMap);
	//printf("Run fin\n");
	return (varType*) &None();
}

bool ifBlock::buildIf(string preIndent) {
	string input;
	string condition;
	printf("... %s", preIndent.c_str());
	char ch;
	while ((ch = getchar()) != '\n') {
		input += ch;
	}

	vector<exprNode*> l;
	vector<ifBlock*>  subBlock;
	int setLen = preIndent.size();

	nodeParser parser;
	while (input != "") {
		//printf("input: %s\n", input.c_str());
		if (input.substr(setLen, 4) == "elif") {
			lines.push_back(l);
			subBlocks.push_back(subBlock);
			l.clear();
			subBlock.clear();
			
			input = parser.cleanStr(input.substr(setLen + 4, input.size() - setLen - 4));
			
			//printf("elif input: %s\n", input.c_str());
			if (input[input.size() - 1] != ':') {
				printf("elif should end with :");
				throw new syntaxException();
			}
			exprNode* conT;
			conT = parser.buildTree(input.substr(0, input.size() - 1));
			conditionLine.push_back(conT);
		}
		else if (input.substr(setLen, 4) == "else") {
			lines.push_back(l);
			subBlocks.push_back(subBlock);
			l.clear();
			subBlock.clear();

			exprNode *conT = parser.buildTree("True");
			conditionLine.push_back(conT);
		}
		else {
			input = input.substr(setLen, input.size() - setLen);
			if (input[0] != '\t' && input[0] != ' ')
				return false;
			trim(input);
			exprNode* line;
			//printf("input: %s\n", input.c_str());
			if (parser.checkReturn(input)) {
				string cinput = parser.cleanStr(input.substr(7, input.size() - 7));
				exprNode* sval = parser.buildTree(cinput);
				line = new exprNode("return", sval);
			}
			else if (input.substr(0, 2) == "if"){
				string subCon = input.substr(2, input.size() - 2);
				exprNode* conT = parser.buildTree(subCon);
				ifBlock* subB = new ifBlock(conT);
				try {
					subB->buildIf(preIndent + '\t');
				}
				catch (generalException ex) {
					return false;
				}
				line = new exprNode("if");
			}
			else {
				line = parser.buildTree(parser.cleanStr(input));
			}
			l.push_back(line);
		}	
		input = "";
		printf("... %s", preIndent.c_str());
		while ((ch = getchar()) != '\n') {
			input += ch;
		}
	}
	lines.push_back(l);
	subBlocks.push_back(subBlock);
}

