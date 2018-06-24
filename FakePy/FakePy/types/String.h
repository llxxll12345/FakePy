#pragma once
#include "stdafx.h"
#ifndef __STRING_H__
#define __STRING_H__

#include <vector>
#include "varType.h"

class String : varType {
private:
	string str;
public:
	String();
	String(string str);
	~String() {};
	string getStr();
	void setStr(string str);

	varType* operator[](vector<varType*> index); //get
	void set(varType* idx, varType* val);
	int idxConv(int idx);
	void add(varType* val);

	string toString();
	string toString_();
	bool isPrintable();
	varType* clone();
	int toInt();
	char toChar();

	bool toBool();

	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}

	varType* operator+(varType* other);
	varType* operator*(varType* other);
	varType* operator==(varType* other);
	varType* operator!=(varType* other);
};

#endif
