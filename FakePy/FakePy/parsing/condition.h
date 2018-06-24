#include "stdafx.h"
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../exceptions/parameterException.h"
#include "exprNode.h"
class ifBlock {
private:
	vector<exprNode*> conditionLine;
	vector<vector<exprNode*>> lines;
	vector<vector<ifBlock*>>  subBlocks;
	unordered_map<string, varType*> localMap;
	bool inFunc;
public:
	ifBlock(exprNode* con) {
		conditionLine.push_back(con);
	}
	ifBlock(vector<exprNode*> cdl) {
		conditionLine = cdl;
	}
	ifBlock(vector<exprNode*> cdl, vector<vector<exprNode*>> l) {
		conditionLine = cdl;
		lines = l;
	}

	~ifBlock() {
		freeVars();
		freeLines();
	}

	vector<vector<exprNode*>> getLines() {
		return lines;
	}
	vector<exprNode*> getBlock(int i) {
		return lines[i];
	}
	void setLines(vector<vector<exprNode*>> l) {
		lines = l;
	}
	void setBlock(int i, vector<exprNode*> l) {
		lines[i] = l;
	}
	
	bool buildIf(string preIndent);

	varType* runIf(unordered_map<string, varType*> *v);

	void freeVars() {
		for (auto p : localMap) {
			delete p.second;
		}
	}

	void freeLines() {
		for (auto line : lines)
			for (auto p : line) {
				if (p)
					delete p;
			}
	}
};


#endif 

