#include "stdafx.h"
#ifndef __ITERATION_H__
#define __ITERATION_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../exceptions/parameterException.h"
#include "exprNode.h"
#include "../types/List.h"
#include "../types/String.h"

class whileBlock {
private:
	exprNode *condition;
	vector<exprNode*> lines;
	unordered_map<string, varType*> localMap;
	vector<ifBlock*>  ifBlocks;
	bool inFunc;
public:
	whileBlock(exprNode* c) {
		condition = c;
	}
	whileBlock(exprNode* c, vector<exprNode*> l) {
		condition = c;
		lines = l;
	}
	~whileBlock() {
		freeLines();
		freeVars();
	}
	void freeVars() {
		for (auto p : localMap) {
			delete p.second;
		}
	}

	void freeLines() {
		for (auto line : lines)
			if (line)
				delete line;
	}

	bool buildWhile(string preIndent);
	varType* runWhile(unordered_map<string, varType*> *v);
};

class forBlock {
private:
	String* iterVar;
	exprNode* varList;
	vector<exprNode*> lines;
	vector<ifBlock*>  ifBlocks;
	unordered_map<string, varType*> localMap;
	
public:
	forBlock(String* i, exprNode* v);
	bool buildFor(string preIndent);
	varType* runFor(unordered_map<string, varType*> *v);

	void freeVars() {
		for (auto p : localMap) {
			delete p.second;
		}
	}

	void freeLines() {
		for (auto line : lines)
			if (line)
				delete line;
	}
};


#endif 

