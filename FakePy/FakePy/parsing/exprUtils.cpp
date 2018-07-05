#include "stdafx.h"
#include "exprUtils.h"
const char* opList = "*^+-/%";
const char* parList = "{}[]()";
int prelist[5] = { 2, 3, 1, 1, 2 };
bool isOp(char ch) {
	for (int i = 0; i < strlen(opList); i++)
		if (opList[i] == ch)
			return true;
	return false;
}

int getPrecedence(char ch) {
	for (int i = 0; i < 5; i++) {
		if (ch == opList[i])
			return prelist[i];
	}
	return -1;
}

bool isdigit(char ch) {
	return ch >= '0' && ch <= '9';
}

bool isPar(char ch) {
	for (int i = 0; i < 6; i++) {
		if (ch == parList[i])
			return true;
	}
	return false;
}

bool isPureNum(string str) {
	for (int i = 0; i < str.length(); i++)
		if (str[i] > '9' || str[i] < '0')
			return false;
	return true;
}

bool isPartNum(char ch) {
	return ch == '.' || isdigit(ch) || ch == 'E';
}

bool isNumber(char* ch) {
	for (int i = 0; i < strlen(ch); i++) {
		if (!isdigit(ch[i])) {
			if (i == 0 && ch[i] != '-' && ch[i] != '+')
				return 0;
			else if (i != 0 && (ch[i] == '.' || ch[i] == 'E' || ch[i] == 'e') && !isdigit(ch[i - 1]) && !isdigit(ch[i + 1]))
				return 0;
			else if (i != 0 && ch[i] != '.' && ch[i] != 'E' && ch[i] != 'e' && ch[i] != '+' && ch[i] != '-')
				return 0;
		}
	}
	return 1;
}

int parseNumber(string ch) {
	return 0;
}

bool isNum(string ch) {
	//printf("Eng match %s : %d\n", ch.c_str(), eng.match(ch));
	regex eng;
	eng.compile("[0-9]+(.[0-9]+)?");
	return eng.match(ch);
}

bool isLetter(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isVariable(string ch) {
	for (int i = 0; i < ch.size(); i++) {
		if (ch[i] != '_' && !isLetter(ch[i])) {
			if (i == 0) return false;
			if (i != 0 && !isdigit(ch[i])) return false;
		}
	}
	printf("var test %s\n", ch.c_str());
	return true;
}


void removeSpace(string &input) {
	string temp;
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ')
			temp += input[i];
	}
	input = temp;
}

int getIndexOf(string input, char c) {
	for (int i = 0; i < input.size(); i++)
		if (input[i] == c)
			return i;
	return -1;
}

bool isDigit(char c) {
	return (c >= '0' && c <= '9');
}

bool isDouble(const string &s) {
	bool dot = false;
	for (int i = 0; i < s.size(); i++) {
		if (((!isDigit(s[i])) && s[i] != '.') || (s[i] == '.' && dot))
			return false;
		if (s[i] == '.')
			dot = true;

	}
	return true;
}

bool isInteger(const string& s) {
	for (int i = 0; i < s.size(); i++) {
		if (!isDigit(s[i])) {
			return false;
		}
	}

	return true;
}

bool isBoolean(const string& s) {
	return (s == "True" || s == "False");
}


bool isString(const string& s) {
	int end = s.size() - 1;

	if (s[0] == '\"' && end == s.find('\"', 1)) {
		return true;
	}
	if (s[0] == '\'' && end == s.find('\'', 1)) {
		return true;
	}
	return false;
}

void ltrim(string &str) {
	int pos = 0;
	while (pos < str.size() && (str[pos] == ' ' || str[pos] == '\t')) {
		pos++;
	}
	if (pos != str.size())
		str = str.substr(pos, str.size() - pos);
	else
		str = "";
}

void rtrim(string &str) {
	int pos = str.size() - 1;
	while (pos >= 0 && (str[pos] == ' ' || str[pos] == '\t')) {
		pos--;
	}
	if (pos != -1)
		str = str.substr(0, pos + 1);
	else
		str = "";
}

void trim(string &str) {
	rtrim(str);
	ltrim(str);
}



void removeZeros(string &str) {
	int pos = 0;
	while (pos < str.size() && (str[pos] == '0')) {
		pos++;
	}
	if (pos != str.size())
		str = str.substr(pos, str.size() - pos);
	else
		str = "";
}