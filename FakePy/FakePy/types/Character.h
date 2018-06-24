#pragma once
#include "stdafx.h"
#ifndef __CHARACTER_H__
#define __CHARACTER_H__


#include "varType.h"
#include "Bool.h"

class Character : public varType {
private:
	char value;
public:
	Character(char ch);
	~Character() {}
	char getVal();
	void setVal(char v);
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
		Character* temp = (Character*)other;
		return new Character(value + temp->getVal());
	}
	
	varType* operator- (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value - temp->getVal());
	}

	varType* operator== (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value == temp->getVal());
	}

	varType* operator> (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value > temp->getVal());
	}

	varType* operator< (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value < temp->getVal());
	}

	varType* operator>= (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value >= temp->getVal());
	}

	varType* operator<= (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value <= temp->getVal());
	}

	varType* operator!= (varType* other) const {
		Character* temp = (Character*)other;
		return new Character(value != temp->getVal());
	}
	
};
#endif