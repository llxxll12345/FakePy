#pragma once
#ifndef __TREEPARSE_H__
#define __TREEPARSE_H__

#include "stdafx.h"
#include "exprUtils.h"
#include "exprParse.h"
#include "variable.h"

struct exprNode;
struct func;
double evalTree(exprNode* root);

const string CONST = "const";

struct exprNode {
	exprNode *left, *right;
	string variable, expression;
	double value;
	exprNode *subTree;
	exprNode() :
		variable(""), expression(""), value(0.0), left(NULL), right(NULL), subTree(NULL) {};
	exprNode(string var, string expr, double val) : 
		variable(var), expression(expr), value(val), left(NULL), right(NULL), subTree(NULL) {};
	exprNode(string var) :
		variable(var), expression(""), value(0.0), left(NULL), right(NULL), subTree(NULL) {};
	double evalFunc() {
		if (subTree)
			return subTree->evalFunc();
		return value;
	};
	exprNode* operator=(const exprNode *other) {
		this->variable = other->variable;
		this->expression = other->expression;
		this->value = other->value;
		this->left = other->left;
		this->right = other->right;
		return this;
	}
};

void printFunc(exprNode* node) {
	if (node != NULL)
		printf("Function var: %s expr: %s value: %.3f\n", node->variable.c_str(), node->expression.c_str(), node->value);
}

bool printUntil(exprNode *root, int n, int N) {
	if (root == NULL) return false;
	if (n == N) {
		printFunc(root);
		return true;
	}
	else {
		bool b1 = printUntil(root->left, n + 1, N);
		bool b2 = printUntil(root->right, n + 1, N);
		if (b1 || b2) return true;
		return false;
	}
}

void printExprTree(exprNode *root) {
	int l = 0;
	printf("level: %d\n", l);
	while (printUntil(root, 0, l++)) {
		printf("level: %d\n", l);
	}
}

exprNode* simplifyNode(exprNode* root) {
	if (root == NULL)
		return root;
	if (root->expression == "*") {
		if (root->left->expression == "const" && root->left->value == 1.0)
			return root->right;
		if (root->right->expression == "const")
			if (root->right->value = 1.0)
				return root->left;
	}

	if (root->expression == "^") {
		if (root->right->expression == "const" && root->right->value == 1.0)
			return root->left;
	}

	if (root->expression == "+" || root->expression == "-") {
		if (root->left->expression == "const" && root->left->value == 0.0)
			return root->right;
		if (root->right->expression == "const")
			if (root->right->value = 0.0)
				return root->left;
	}
	return root;
}

exprNode* simplifyExpr(exprNode *root) {
	if (root == NULL)
		return root;
	root->left = simplifyNode(root->left);
	root->right = simplifyNode(root->right);
	return simplifyNode(root);
}


exprNode* build(exprNode* a, exprNode* b, string op) {
	exprNode* res = new exprNode(op, "", 0.0);
	res->left = a;
	res->right = b;
	return res;
}


double evalVal(string op, double a, double b) {
	printf("op: %s, %.3f, %.3f\n", op.c_str(), a, b);
	switch (op[0]) {
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	case '^': return pow(a, b);
	default: return -1;
	}
}

double evalUntil(exprNode* root) {
	printFunc(root);
	if (root) {
		double left = evalUntil(root->left);
		double right = evalUntil(root->right);
		if (!isOp(root->variable[0])) {
			return root->value;
		}
		else {
			printf("%.3f\n", evalVal(root->variable, left, right));
			return evalVal(root->variable, left, right);
		}
	}
}

double evalTree(exprNode* root) {
	if (!root) return 0;
	stack<exprNode*> s;
	return evalUntil(root);
}

string deparse(exprNode *root) {
	string result = "";
	if (!root) return "";
	string expr = root->expression;
	if (expr == "const") 
		return to_string(root->value);
	//if (valueList.containsKey(expr)) return expr;
	if (expr == "cos" || expr == "sin" || expr == "ln") {
		return expr + "(" + deparse(root->subTree) + ")";
	}
	if ((isOp(expr[0]) && (root->left == NULL || root->right == NULL)) || !isOp(expr[0])) {
		printf("Incomplete or Unsupported expression tree.");
		return "";
	}

	
	string leftExpr = root->left->expression;
	string rightExpr = root->right->expression;

	if (isOp(leftExpr[0]) && getPrecedence(expr[0]) > getPrecedence(leftExpr[0])) {
		result += "(" + deparse(root->left) + ")";
	}
	else {
		result += deparse(root->left);
	}
	result += expr;
	if (isOp(rightExpr[0]) && getPrecedence(expr[0]) > getPrecedence(rightExpr[0])) {
		result += "(" + deparse(root->right) + ")";
	}
	else {
		result += deparse(root->right);
	}

	return  result;
}

exprNode* parseUntil(vector<string> &tklist, int i, exprNode* root) {
	if (i < 0 || i >= tklist.size())
		return NULL;
	printf("l: %d, str: %s\n", i, tklist[i].c_str());
	if (root == NULL) {
		if (isOp(tklist[i][0]))
			root = new exprNode(tklist[i], "", 0.0);
		else if (isNum(tklist[i]))
			root = new exprNode(CONST, "", atof(tklist[i].c_str()));
		else {
			root = new exprNode(tklist[i], "~", 0.0);
			root->subTree = parseUntil(tklist, i + 1, root);
			return root;
		}
	}
	root->left = parseUntil(tklist, i * 2 + 1, root->left);
	root->right = parseUntil(tklist, i * 2 + 2, root->right);
	return root;
}

exprNode* giveValue(exprNode* root, exprNode* res) {
	if (res == NULL) return NULL;
	if (root == NULL) {
		return root = res;
	}
	root->left = giveValue(root->left, res);
	root->right = giveValue(root->right, res);
	return root;
}

exprNode* parseTree(string input, exprNode *root) {
	stack<exprNode*> s;
	vector<string> tklist = shuttingYard_adv(input);
	for (string token : tklist) {
		printf("build: %s\n", token.c_str());
		if (isOp(token[0])) {
			exprNode* argLeft =  s.top();
			s.pop();
			exprNode* argRight = s.top();
			s.pop();
			switch (token[0]) {
				case '+': s.push(build(argLeft, argRight, "+")); break;
				case '-': s.push(build(argLeft, argRight, "-")); break;
				case '*': s.push(build(argLeft, argRight, "*")); break;
				case '/': s.push(build(argLeft, argRight, "/")); break;
				case '^': s.push(build(argLeft, argRight, "^")); break;
				default:  printf("ERR: Unidentified operator.\n");
			}
		}
		else if (token == "cos" || token == "sin" || token == "ln") {
			exprNode* temp = new exprNode(token, "~", 0.0);
			temp->subTree = s.top();
			s.pop();
			s.push(temp);
		}
		else {
			//if (valueList.containsKey(token))
				//stack.push(new exprNode(new func(token, token, valueList.get(token))));
			if (isNum(token)) {
				exprNode* temp = new exprNode(CONST, "/", atof(token.c_str()));
				s.push(temp);
			}
			else if (isVariable(token)) {
				exprNode* temp = new exprNode(CONST, "/", varMap[token]);
				s.push(temp);
			}
		}
	}
	if (!s.empty()) {
		exprNode* res = s.top();
		root = giveValue(root, res);
		printExprTree(s.top());
		s.pop();
		return root->left;
	}
	return NULL;
}

#endif // !__TREEPARSE_H_
