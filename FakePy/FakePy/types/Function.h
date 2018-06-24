#include "stdafx.h"
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "varType.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../exceptions/parameterException.h"
#include "../parsing/exprNode.h"
class Function : public varType {
	friend class ifBlock;
private:
	vector<exprNode*> lines;
	vector<string>    localVars;
	unordered_map<string, varType*> localMap;
	vector<ifBlock*>  ifBlocks;

public:
	Function(vector<string> p) : localVars(p) {
		myType = Types::Functions;
	}
	Function(vector<string> p, vector<exprNode*> l) : localVars(p), lines(l) {
		myType = Types::Functions;
	}
	~Function() {};

	vector<exprNode*> getLines() {
		return lines;
	}
	void setLines(vector<exprNode*> l) {
		lines = l;
	}
	vector<string> getVars() {
		return localVars;
	}
	void setVars(vector<string> p) {
		localVars = p;
	}
	bool buildFun(string preIndent = "");

	varType* runFun(vector<varType*> parameters);

	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}

	void freeVars() {
		for (auto p : localMap) {
			delete p.second;
		}
	}

	void freeLines() {
		for (auto p : lines) {
			if (p)
				delete p;
		}
	}

	int toInt() {
		return NULL;
	}

	bool isPrintable() { 
		return false; 
	}
	string toString() { 
		return ""; 
	}

	char toChar() {
		return NULL;
	}
	varType* clone() {
		Function* fun = new Function(this->localVars, this->lines);
		fun->localMap = this->localMap;
		return (varType*)fun;
	}
};


#endif // FUNCTION_H#pragma once
