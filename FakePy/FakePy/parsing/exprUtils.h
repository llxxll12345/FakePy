#include "stdafx.h"
#ifndef __EXPRUTILS_H__
#define __EXPRUTILS_H__

#include "../regex/regex.h"


bool isOp(char ch);

int getPrecedence(char ch);

bool isdigit(char ch);

bool isPar(char ch);

bool isPureNum(string str);

bool isPartNum(char ch);

bool isNumber(char* ch);

int parseNumber(string ch);

bool isNum(string ch);

bool isLetter(char ch);

bool isVariable(string ch);

void removeSpace(string &input);

int getIndexOf(string input, char c);

bool isDigit(char c);

bool isDouble(const string &s); 

bool isInteger(const string& s);

bool isBoolean(const string& s);

bool isString(const string& s);

void ltrim(string &str);

void rtrim(string &str);

void trim(string &str);

void removeZeros(string &str);

#endif


