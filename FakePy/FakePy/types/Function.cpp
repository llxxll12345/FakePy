#include "stdafx.h"
#include "Function.h"
#include "../parsing/nodeParse.h"
#include "../parsing/exprUtils.h"
#include "../exceptions/syntaxExceptionh.h"
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
			printf("is return\n");
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
		printf("... ");
		while ((ch = getchar()) != '\n') {
			input += ch;
		}
	}
	return true;
}