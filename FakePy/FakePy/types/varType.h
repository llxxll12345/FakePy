#pragma once
#include "stdafx.h"
#ifndef __VARTYPE_H__
#define __VARTYPE_H__


enum Types {
	Boolean,
	Integer,
	Characters,
	Doubles,
	Lists,
	Strings,
	Voids,
	Functions,
	Operand,
	BigNum,
	NonType
};

class varType {
private:
	bool  canClone = true;
	
public:
	Types myType = Types::Voids;
	bool  isTempVar;
	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}
	varType() {}
	varType(Types tpname) {
		myType = tpname;
	}
	void setCanClone(bool c) { 
		canClone = c; 
	}
	bool getCanClone() {
		return canClone;
	}

	virtual bool isPrintable() = 0;
	virtual string toString() = 0;
	virtual int toInt() = 0;
	virtual varType* clone() = 0;

	virtual char toChar() = 0;

	virtual varType* operator+(varType* other) const { return NULL; }
	virtual varType* operator-(varType* other) const { return NULL; }
	virtual varType* operator*(varType* other) const { return NULL; }
	virtual varType* operator/(varType* other) const { return NULL; }
	virtual varType* operator%(varType* other) const { return NULL; }
	virtual varType* operator>(varType* other) const { return NULL; }
	virtual varType* operator<(varType* other) const { return NULL; }
	virtual varType* operator==(varType* other) const { return NULL; }
	virtual varType* operator>=(varType* other) const { return NULL; }
	virtual varType* operator<=(varType* other) const { return NULL; }
	virtual varType* operator!=(varType* other) const { return NULL; }
	virtual varType* operator!() const { return NULL; }
	virtual varType* operator||(varType* other) const { return NULL; }
	virtual varType* operator&&(varType* other) const { return NULL; }
	virtual varType* operator<<(varType* other) const { return NULL; }
	virtual varType* operator>>(varType* other) const { return NULL; }
	virtual varType* operator+=(varType* other) const { return NULL; }
	virtual varType* operator-=(varType* other) const { return NULL; }
	virtual varType* operator/=(varType* other) const { return NULL; }
	virtual varType* operator*=(varType* other) const { return NULL; }
	virtual varType* operator%=(varType* other) const { return NULL; }

};


#endif //TYPE_H