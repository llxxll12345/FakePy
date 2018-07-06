#include "stdafx.h"
#include "nodeParse.h"
#include "exprUtils.h"
#include "condition.h"
#include "iteration.h"
#include "../types/None.h"
#include "../types/List.h"
#include "../types/String.h"
#include "../exceptions/syntaxExceptionh.h"
#include "../exceptions/parameterException.h"


forBlock::forBlock(String* i, exprNode* v) {
	iterVar = i;
	varList = v;
}

bool whileBlock::buildWhile(string preIndent) {
	string input;
	printf("... %s", preIndent.c_str());
	char ch;
	while ((ch = getchar()) != '\n') {
		input += ch;
	}
	int setLen = preIndent.size();

	nodeParser parser;
	while (input != "") {
		//printf("input: %s\n", input.c_str());
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
		else if (input.substr(0, 2) == "if") {
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
		printf("... %s", preIndent.c_str());
		while ((ch = getchar()) != '\n') {
			input += ch;
		}
	}
	return true;
}


varType* whileBlock::runWhile(unordered_map<string, varType*> *v) {
	if (v == nullptr)
		return NULL;

	localMap = *v;
	//printf("local for a: %s\n", ((Int*) (*v)["a"])->toString().c_str());
	nodeParser parser;
	int index = 0, subIndex = 0;
	int ifIdx = 0;
	while (1) {
		//printf("SIZE: %d\n", v->size());
		varType* ret = parser.runCmd(condition, v);
		if (ret->myType != Types::Boolean && ret->myType != Types::Integer) {
			printf("The condition line does not return a boolean value.\n");
			throw new parameterException();
		}
		bool verdict = ((Bool*)ret)->getVal();
		//printf("verdict: %d\n", verdict);
		if (verdict)
			break;

		ifIdx = 0;
		for (auto line : lines) {
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
	}
	parser.memFree(&localMap);
	//printf("Run fin\n");
	return (varType*)&None();
}


bool forBlock::buildFor(string preIndent) {
	string input;
	printf("... %s", preIndent.c_str());
	char ch;
	while ((ch = getchar()) != '\n') {
		input += ch;
	}
	int setLen = preIndent.size();

	nodeParser parser;
	while (input != "") {
		//printf("input: %s\n", input.c_str());
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
		else if (input.substr(0, 2) == "if") {
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
		printf("... %s", preIndent.c_str());
		while ((ch = getchar()) != '\n') {
			input += ch;
		}
	}
	return true;
}


varType* forBlock::runFor(unordered_map<string, varType*> *v) {
	if (v == nullptr)
		return NULL;

	localMap = *v;
	//printf("local for a: %s\n", ((Int*) (*v)["a"])->toString().c_str());
	nodeParser parser;
	int ifIdx = 0;
	varType* retList = parser.runCmd(varList, v);
	if (retList->myType != Types::Lists) {
		printf("Iteration base should be of type list!");
		throw new syntaxException();
	}
	
	vector<varType*> iterList = ((List*)retList)->getList();
	string varName = iterVar->getStr();
	localMap.insert(make_pair(varName, iterList[0]));

	vector<varType*>::iterator iter = iterList.begin();
	for (; iter != iterList.end(); iter++) {
		localMap[varName] = *iter;
		ifIdx = 0;
		for (auto line : lines) {
			printf("line %s\n", line->getVal().c_str());
			if (line->getVal() == "continue") {
				break;
			}
			if (line->getVal() == "break") {
				parser.memFree(&localMap);
				return (varType*)&None();
			}
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
	}
	parser.memFree(&localMap);
	//printf("Run fin\n");
	return (varType*)&None();
}

