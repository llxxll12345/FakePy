#pragma once
#include "stdafx.h"
#include <cmath>
#include "../utils/mystack.h"
#include "exprUtils.h"
#include "variable.h"
#include "exprUtils.h"

// basic calculator

double postfixEval(vector<string> tokenLst) {
	stack<double> s;
	for (int i = 0; i < tokenLst.size(); i++) {
		if (isPureNum(tokenLst[i].c_str())) {
			s.push(atof(tokenLst[i].c_str()));
		}
		else if (isVariable(tokenLst[i])) {
			s.push(varMap[tokenLst[i]]);
		}
		else if (isOp(tokenLst[i][0])) {
			int val1 = s.top();
			s.pop();
			int val2 = s.top();
			s.pop();
			switch (tokenLst[i][0]) {
			case '+': s.push(val1 + val2); break;
			case '-': s.push(val1 - val2); break;
			case '*': s.push(val1 * val2); break;
			case '/': s.push(val1 / val2); break;
			case '^': s.push(pow(val2, val1));
			default:
				printf("Unsupported operator!!!\n");
			}
		}
		else {
			int val1 = s.top();
			s.pop();
			if (tokenLst[i] == "cos")
				return cos(atof(tokenLst[i].c_str()));
			if (tokenLst[i] == "sin")
				return sin(atof(tokenLst[i].c_str()));
		}
	}
	return s.top();
}
;

string shuttingYard(string expr) {
	string res = "";
	printf("ok\n");
	Stack<int> s(expr.size());
	for (int i = 0; i < expr.size(); i++) {
		if (isdigit(expr[i]))
			res += expr[i];
		else if (expr[i] == '(')
			s.push(expr[i]);
		else if (expr[i] == ')') {
			while (!s.empty() && (char)s.peek() != '(') {
				printf("%d %c\n", i, s.peek());
				res += s.pop();
			}
			printf("empty: %d\n", s.empty());
			if (!s.empty() && s.peek() != '(') {
				std::printf("Unmatched parentheses.\n");
				return "";
			}
			else s.pop();
		}
		else if (isOp(expr[i])) {
			while (!s.empty() && getPrecedence(expr[i]) <= getPrecedence(s.peek())) {
				res += s.pop();
			}
			s.push(expr[i]);
		}
	}
	while (!s.empty())
		res += s.pop();
	//reverse(res.begin(), res.end());
	printf("Postfix: %s\n", res.c_str());
	return res;
}


