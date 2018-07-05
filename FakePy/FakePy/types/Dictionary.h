#pragma once
#include "stdafx.h"

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__
#include <unordered_map>
#include "varType.h"

class Dictionary : varType {
private:
	unordered_map<varType*, varType*> dict;
public:
	Dictionary();
	Dictionary(unordered_map<varType*, varType*> d);
	~Dictionary();
	unordered_map<varType*, varType*> getDict();
	void setDict(unordered_map<varType*, varType*> d);

	void set(varType* Key, varType* val);
	void add(varType* Key, varType* val);
	void add(pair<varType*, varType*> p);

	string toString();
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

	varType* operator+(pair<varType*, varType*> p);
	varType* operator[](varType* Key);
};

#endif