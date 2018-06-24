#pragma once
#include "stdafx.h"
#ifndef __LIST_H__
#define __LIST_H__

#include "varType.h"

class List :  varType {
private:
	vector<varType*> mylist;
public:
	bool isTuple;
	bool isDictionary;
	List();
	List(vector<varType*> l);
	~List();
	vector<varType*> getList();
	void setList(vector<varType*> value);

	varType* operator[](vector<varType*> index); //get
	void set(varType* idx, varType* val);
	int idxConv(int idx);
	void add(varType* val);

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

	varType* operator+(varType* other);
	varType* operator*(varType* other);
};

#endif