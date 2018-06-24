#pragma once
#pragma once
#include "stdafx.h"
#ifndef __DOUBLE_H__
#define __DOUBLE_H__

#include "varType.h"
#include "Bool.h"

class Double : public varType {
private:
	double value;
public:
	Double(double value);
	~Double() {}
	double getVal();
	void setVal(double v);
	bool isPrintable();
	string toString();
	varType* clone();

	bool toBool();
	int toInt();
	int toNInt();
	char toChar();
	double toDouble();

	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}

	varType* operator+ (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value + temp->getVal());
	}

	varType* operator- (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value - temp->getVal());
	}

	varType* operator* (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value + temp->getVal());
	}

	varType* operator/ (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value - temp->getVal());
	}

	varType* operator== (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value == temp->getVal());
	}

	varType* operator> (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value > temp->getVal());
	}

	varType* operator< (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value < temp->getVal());
	}

	varType* operator>= (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value >= temp->getVal());
	}

	varType* operator<= (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value <= temp->getVal());
	}

	varType* operator!= (varType* other) const {
		Double* temp = (Double*)other;
		return new Double(value != temp->getVal());
	}

};

#endif