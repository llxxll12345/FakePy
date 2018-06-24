#pragma once
#include "stdafx.h"
#ifndef __NONE_H__
#define __NONE_H__

#include "varType.h"
#include "Bool.h"

class None : public varType {
public:
	None() {
		myType = Types::NonType;
	}
	~None() {}
	bool isPrintable() { return false; }
	int toInt() { return 0; }
	string toString() {
		return "";
	};
	char toChar() {
		return NULL;
	}
	void setTempVar(bool v) {
		isTempVar = v;
	}
	bool getTempVar() {
		return isTempVar;
	}
	None* clone() { return new None(); }
};
#endif
#pragma once
