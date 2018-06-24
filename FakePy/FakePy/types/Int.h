#pragma once
#include "stdafx.h"
#ifndef __INT_H__
#define __INT_H__


#include "varType.h"
#include "Bool.h"

class Int : public varType
{
private:
	int value;
public:
	Int(int value);
	~Int() {}
	int getVal();
	void setVal(int v);
	bool isPrintable();
	string toString();
	varType* clone();

	bool toBool();
	int toInt();
	char toChar();
	double toDouble();

	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}

	varType* operator+ (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value + temp->getVal());
	}

	varType* operator- (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value - temp->getVal());
	}

	varType* operator* (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value + temp->getVal());
	}

	varType* operator/ (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value / temp->getVal());
	}

	varType* operator== (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value == temp->getVal());
	}

	varType* operator> (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value > temp->getVal());
	}

	varType* operator< (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value < temp->getVal());
	}

	varType* operator>= (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value >= temp->getVal());
	}

	varType* operator<= (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value <= temp->getVal());
	}

	varType* operator!= (varType* other) const {
		Int* temp = (Int*)other;
		return new Int(value != temp->getVal());
	}

};
#endif