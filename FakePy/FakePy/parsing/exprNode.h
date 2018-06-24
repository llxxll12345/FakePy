#pragma once
#include "stdafx.h"
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "../types/varType.h"
#include <vector>

class exprNode {
private:
	vector<exprNode*> children;
	string val;

public:
	exprNode(string v) : val(v) {};
	exprNode(string v, exprNode* subTree) : val(v) {
		children.push_back(subTree);
	};
	exprNode(string v, exprNode* left, exprNode* right) : val(v) {
		children.push_back(left);
		children.push_back(right);
	};
	exprNode(string v, vector<exprNode*> child) : val(v) {
		children = child;
	};
	exprNode(const exprNode& other){
		this->val = other.val;
		this->children = other.children;
	};
	~exprNode() {
		for (exprNode* node : children)
			if (node)
				delete node;
	};

	bool hasChd() {
		return !children.empty();
	}
	int chdSz() {
		return children.size();
	}

	string getVal() {
		return val;
	}
	void setVal(string v) {
		val = v;
	}

	vector<exprNode*> getChds() {
		return children;
	}
	exprNode* getChd(int i) {
		return children[i];
	}

	void setChds(vector<exprNode*> child) {
		children = child;
	}
	void setChd(int i, exprNode* val) {
		children[i] = val;
	}

	void addChd(exprNode* child) {
		children.push_back(child);
	}

	void revChd() {
		reverse(children.begin(), children.end());
	}

	void printT() {
		int i;
		if (chdSz() == 0) {
			if (val == "")
				printf("VOID");
			else
				printf("%s", val.c_str());
		}
		else if (val == "[]" || val == "{}" || val == "()") {
			printf("%c", val[0]);
			for (i = 0; i < chdSz()-1; i++) {
				if (!children[i])
					printf("VOID");
				else
					children[i]->printT();
				printf(", ");
			}
			if (!children[i]) 
				printf("VOID");
			else 
				children[i]->printT();
			printf("%c", val[1]);
		}
		else if (chdSz() == 1) {
			printf("%s ", val.c_str());
			if (!children[0])
				printf("VOID");
			else
				children[0]->printT();
		}
		else if (chdSz() == 2)
		{
			if (children[0] == nullptr)
				printf("VOID");
			else
				children[0]->printT();
			if (val == "[]*" || val == "{}" || val == "()") {
				printf("%c", val[0]);
				if (!children[1])
					printf("VOID");
				else
					children[1]->printT();
				printf("%c", val[1]);
			}
			else {
				printf("%c", val.c_str());
				if (!children[1])
					printf("VOID");
				else
					children[1]->printT();
			}
		}
		printf("\n");
	}
};

#endif // TREE_NODE_H