#pragma once
#include "stdafx.h"
#ifndef __NODEPARSE_H__
#define __NODEPARSE_H__


#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include "../types/Function.h"
#include "condition.h"
#include "iteration.h"
#include "String.h"
#include "../types/Void.h"
#include <vector>
#include "exprNode.h"



class nodeParser
{
	friend class Function;
	friend class ifBlock;
public:
	varType* parseStr(string str);
	void memFree( unordered_map< string, varType*>* varMap = nullptr);
	exprNode* buildTree(const string &str);
	void test();


	varType* getBasicType(const string &str);

	bool checkFunDeclare(const string & str);
	bool isVar(const string& str);
	bool isBasicType(const string& str);
	bool checkReturn(const string& str);

	string cleanStr(const string &str);

	int getFirstOccr(const string &s, const string &a);

	varType* getVarVal(const  string &str,  unordered_map< string, varType*>* varMap = nullptr);

	
	exprNode* buildClosedTree(string &str, const string &val);
	
	int findPair( pair<const char, const char> openerAndCloser, const string &str, int pos = 0);
	int findPair(char opener, char closer, const  string &str, int pos = 0);

	vector<pair<char, pair<int, int>>> getPairIndex(const string &str);

	varType* runCmd(exprNode* commandTree,  unordered_map< string, varType*>* varMap);

	vector<varType*> parseRange(exprNode* indexTree,  unordered_map< string, varType*>* varMap = nullptr);
	void parseRange_until(exprNode* indexTree,  vector<varType*>& indexVec,  unordered_map< string, varType*>* varMap = nullptr);

	varType* getDefault(const string& str,  unordered_map< string, varType*>* varMap = nullptr);

	void assignVar(const string& varName, varType* assignVal,  unordered_map< string, varType*>* localVarMap = nullptr);

	Function* assignFun(const string &funName);
	void assignIter(const string &iters, varType* iterList, unordered_map<string, varType*>* vaarMap = nullptr);
	ifBlock* assignIf(const string &condition);
	
	forBlock* assignFor(const string &iterName, const string &Lists);
	whileBlock* assignWhile(const string &condition);

	Void* getVoid();
};


#endif //PARSER_H