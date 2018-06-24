#include "stdafx.h"
#ifndef __BOOL_H__
#define __BOOL_H__

#include "varType.h"

class Bool : varType {
private:
	bool value;
public:
	Bool(bool value);
	~Bool() {}
	bool getVal() ;
	void setVal(int v);
	bool isPrintable();
	string toString();
	varType* clone();

	bool toBool();
	int toInt();
	double toDouble();
	char toChar();

	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}

	varType* operator!() const {
		return (varType*) new Bool(!value);
	}

	varType* operator&&(varType* other) const {
		Bool* temp = (Bool*)other;
		return (varType*)new Bool(temp->getVal() && value);
	}

	varType* operator||(varType* other) const {
		Bool* temp = (Bool*)other;
		return (varType*)new Bool(temp->getVal() || value);
	}
};


#endif // __BOOLTYPE_H__