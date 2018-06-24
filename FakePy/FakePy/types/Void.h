#pragma once
#include "stdafx.h"
#ifndef __VOID_H__
#define __VOID_H__

#include "varType.h"
#include "Bool.h"

class Void : varType
{
public:
	Void() {
		myType = Types::Voids;
	}
	~Void() {}
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
	Void* clone() { return new Void(); }
};
#endif
