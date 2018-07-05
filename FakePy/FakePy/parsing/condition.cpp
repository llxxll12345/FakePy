#include "stdafx.h"
#include "nodeParse.h"
#include "exprUtils.h"
#include "condition.h"
#include "../types/None.h"
#include "../exceptions/syntaxExceptionh.h"

varType* ifBlock::runIf(unordered_map<string, varType*> *v) {
	if (v == nullptr)
		return NULL;

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
	return (varType*)&None();
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
			else if (input.substr(0, 2) == "if") {
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