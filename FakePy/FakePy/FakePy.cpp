
#include "stdafx.h"
#include "utils/stream.h"
#include "regex/regex.h"
#include "types/varType.h"
#include "parsing/nodeParse.h"
#include <iostream>
//#include "syntax.h"
#include "tests/testType.h"
#include "tests/testExt.h"
//#include "treeParse.h"
//#include "variable.h"
//#include "exprEval.h"
//regex engine based 
static bool isEnd = false;


/*bool isAssigning(string input) {
	regex eng;
	eng.compile("([a-z]+)=([0-9]+)(.[0-9]+)?");
	bool res = eng.match(input);
	printf("\nAssign: %d\n", res);
	return res;
	//basicCalc();
}

bool isEquation(string input) {
	for (char ch : input) {
		if (!isdigit(ch) && !isOp(ch) && !isPar(ch))
			return false;
	}
	return true;
}

void myExit(string args) {
	isEnd = true;
	printf("Error: %s", args.c_str());
	getchar();
	exit(1);
}



void preProcess(string input) {
	if (!checkParMatching(input)) {
		myExit("Umatched Parentheses or Bracket in your latest input!!\n");
	}
	removeSpace(input);
	parse(input);
	
	exprNode root;
	root = parseTree(input, &root);
	printf("RES: %.3f\n", evalTree(&root));
	printExprTree(&root);

	
	if (isAssigning(input)) {
		int idx = getIndexOf(input, '=');
		string var = input.substr(0, idx);
		double val = postfixEval(shuttingYard_adv(input.substr(idx+1)));
		printf("val: %.3f\n", val);
		if (varMap.count(var)) {
			varMap[var] = val;
		}
		else {
			varMap.insert(make_pair(var, val));
		}
		printf("Assign value %.3f to %s\n", val, var.c_str());
	}
	else if (isEquation(input)) {
		vector<string> tkLst = shuttingYard_adv(input);
		printf("%.3f\n", postfixEval(tkLst));
	}
}*/


void std_input() {
	printf("FakePy beta 1.0.0 :_: ~_~ ^_^ $_$ @LLX \n STD I/O mode\n");

	string input;
	char ch;
	printf(">>> ");
	while ((ch = getchar()) != '\n') {
		input += ch;
	}

	vector<string> operationStack;
	nodeParser parser;
	int stackId;

	//testExt();
	//isEnd = 1;
	while (input != "exit()") {
		operationStack.push_back(input);
		stackId = operationStack.size();
		try {
			varType* temp = parser.parseStr(input);
			if (temp) {
				if (temp->isPrintable())
					printf("%s\n", temp->toString().c_str());
				if (temp->getTempVar()) {
					delete temp;
				}
			}
		}
		catch (generalException* ex) {
			string err = ex->what();
			printf("Error: %s\n", err.c_str());
		}
		// get new command from user
		input = "";
		printf(">>> ");
		while ((ch = getchar()) != '\n') {
			
			input += ch;
		}
	}

	parser.memFree();

}


void file_input(string fileName) {
	FILE *f = new FILE();
	f = fopen(fileName.c_str(), "r");
	printf("FakePy beta 1.0.0 :_: ~_~ ^_^ $_$ @LLX\n FILE input: %s\n", fileName.c_str());

	string input;
	char ch;
	fprintf(stdout, ">>> ");
	while ((ch = getc(f)) != '\n') {
		input += ch;
	}

	int i = 0;
	nodeParser parser;
	while (input != "EOF") {
		try {
			printf("#%3d %s", ++i, input.c_str());
			varType* temp = parser.parseStr(input);
			if (temp) {
				if (temp->isPrintable())
					printf("%s\n", temp->toString().c_str());
				if (temp->getTempVar()) {
					delete temp;
				}
			}
		}
		catch (generalException* ex) {
			string err = ex->what();
			printf("Error: %s\n", err.c_str());
		}
		// get new command from user
		//fprintf(stdout, ">>> ");
		while ((ch = getc(f)) != '\n') {
			input += ch;
		}
	}

	parser.memFree();
}


int main() {
	string input;
	char ch;
	printf("Input a certain pattern:\n");
	testType();
	std_input();
	nodeParser parser;
	parser.test();
	/*while (input != "EOF") {
		printf(">>> ");
		while ((ch = getchar()) != '\n') {
			input += ch;
		}
		//preProcess(input);
		input = "";
		printf("%s", input.c_str());
		system("pause");
	}*/

	
	//testRegex();
	getchar();
    return 0;
}

