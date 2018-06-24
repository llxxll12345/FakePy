#include "stdafx.h"
#include "Bool.h"


Bool::Bool(bool v) {
	value = v;
	myType = Types::Boolean;
}
bool Bool::getVal() { return value; }
void Bool::setVal(int v) { value = v; }
bool Bool::isPrintable() { return true; }
string Bool::toString() { return value == 0 ? "false" : "true"; }
varType* Bool::clone() { return new Bool(value); }

bool Bool::toBool() { return value; }
int Bool::toInt() { return value ? 1 : 0; }
double Bool::toDouble() { return value ? 1.0 : 0.0; }
char Bool::toChar() { return value ? 't' : 'f'; }
