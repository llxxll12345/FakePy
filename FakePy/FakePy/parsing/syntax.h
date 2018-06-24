#pragma once
#ifndef __SYNTAX_H__
#define __SYNTAX_H__
#include "stdafx.h"
#include "../utils/mystack.h"
#include <string>
using namespace std;
vector<string> remComments(vector<string> input) {
	vector<string> ans;
	bool isBlock = false;
	string line;
	string mark;
	for (string str : input) {
		for (int i = 0; i< str.length(); ) {
			if (!isBlock) {
				if (str[i] != '#') {
					if (i < str.length() - 3) {
						mark = str.substr(i, 3);
						if (mark == "'''")
							isBlock = 1, i += 3;
					}
					else
						line += str[i++];
				}
				else break;
			}
			else {
				if (i < str.length() - 3) {
					string st = str.substr(i, 3);
					if (st == "'''") {
						isBlock = 0;
						i += 3;
					}
					else
						i++;
				}
			}
		}
		if (line.size() && !isBlock) {
			ans.push_back(line);
			line = "";      // clear temp only when a valid result appears (line broken by multiple lines of comments)
		}
	}
	if (isBlock) {
		printf("unmatched block comment notations at the end of the file! Compilation stops.\n");
		ans.clear();
		return ans;
	}
	return ans;

}

bool checkParMatching(string input) {
	Stack<char> s(input.size());
	for (int i = 0; i < input.size(); i++) {
		if (input[i] == '(') {
			s.push(')');
		}
		else if (input[i] == '[') {
			s.push(']');
		}
		else if (input[i] == '{') {
			s.push('}');
		}
		else if (input[i] == ')' || input[i] == ']' || input[i] == '}'){
			if (s.empty() || s.peek() != input[i]) {
				printf("Unmatched %c at %d.\n", input[i], i);
				return false;
			}
			else s.pop();
		}
	}
	return s.empty();
} 




#endif // !__SYNTAX_H__
