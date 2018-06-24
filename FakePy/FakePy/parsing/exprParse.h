#pragma once
#ifndef __EXPRPARSE_H__
#define __EXPRPARSE_H__

#include "stdafx.h"
#include "exprUtils.h"
#include "../utils/mystack.h"

string doParse(string input, string& last, int& i);

bool otherChar(char ch) {
	return !isLetter(ch) && !isdigit(ch) && ch != '-' && ch != ' ' && ch != '(' && ch != ')';
}

vector<string> parse(string input) {	
	vector<string> res;
	string last = "";
	int i = 0;
	while (i < input.size()) {
		res.push_back(doParse(input, last, i));
		//printf("%s\n", res.back().c_str());
	}
	return res;
}

string doParse(string input, string& last, int& i) {
	if (i > input.size()) 
		return last = "";
	string temp;
	auto isbgminus = [&last, &input](char ch, int i) {
		if (i == input.size() - 1) return false;
		return ch == '-' && isdigit(input[i + 1]) && (last == "(" || last == "" || isOp(last[0]));
	};
	char ch = input[i];
	while (ch == ' ' && i < input.size()) {
		ch = input[++i];
	}
	if (isdigit(ch)) {
		while (isPartNum(ch) && i < input.size()) {
			temp += input[i++];
			ch = i < input.size() ? input[i] : 0;
		}
	}
	else if (isbgminus(ch, i)) {
		temp += '-';
		i++;
		temp += doParse(input, last, i);
	}
		
	else if (ch == '(' || ch == ')') {
		temp += ch;
		i++;
	}
	else if (isLetter(ch)) {
		string funcName = "";
		while (isLetter(ch) && (i < input.length())) {
			temp += ch;
			i++;
			ch = (i < input.length()) ? input[i] : 0;
		}
		temp += funcName;
	}
	else {
		while (otherChar(ch) && (i < input.length())) {
			temp += input[i];
			i++;
			ch = (i < input.length()) ? input[i] : 0;
		}
		if (!isOp(temp[0]) && temp != "=" && temp != "==") {
			printf("Unknown operator: %s", temp.c_str());
		}
	}
	last = temp;
	return temp;
}

bool checkRPN(vector<string> tklist) {
	int cnt = 0;
	for (string str : tklist) {
		if (str == "cos" || str == "sin" || str == "ln") continue;
		if (isOp(str[0])) cnt -= 2;
		if (cnt < 0) return false;		// too many operators
		cnt++;
	}
	return cnt == 1;	// > 1 too many vars, < 1 empty
}


vector<string> shuttingYard_adv(string input) {
	vector<string> tklist = parse(input);
	vector<string> res;
	stack<string> s;
	for (string str : tklist) {
		//printf("ss str: %s\n", str.c_str());
		if (str == "") continue;
		if (isNum(str) || isVariable(str)) {
			//printf("match Var\n");
			res.push_back(str);
		}
		else if (isOp(str[0])) {
			//printf("match OP\n");
			string ntk = s.empty() ? "" : s.top();
			while (ntk != "" && isOp(ntk[0]) && getPrecedence(str[0]) <= getPrecedence(ntk[0])) {
				res.push_back(s.top());
				s.pop();
				ntk = s.empty() ? "" : s.top();
				//nextOP = OPERATORS.get(nextToken);
			}
			s.push(str);
		}
		else if (str == "cos" || str == "sin" || str == "ln") {
			s.push(str);
		}
		else if (str == "(") {
			//printf("match(\n");
			s.push(str);
		}
		else if (str == ")") {
			//printf("match)\n");
			while (!s.empty() && s.top() != "(") {       
				res.push_back(s.top());
				s.pop();
			}
			if (s.empty()) {
				printf("Mismatched parentheses");
				return res;
			}
			s.pop();
			if (!s.empty()) {
				string front = s.top();
				if (front == "cos" || front == "sin" || front == "ln") {
					res.push_back(s.top());
					s.pop();
				}
			}
		}
	}
	while (!s.empty()) {
		string temp = s.top();
		s.pop();
		if (temp == "(" || temp == ")") {
			printf("Mismatched parentheses\n");
			return res;
		}
		if (!isOp(temp[0])) {
			printf("Unknown operator or function: %s\n" , temp.c_str());
		}
		res.push_back(temp);
	}
	printf("Check: %d\n", checkRPN(res));
	return res;
}



#endif // !__EXPRPARSE_H__
