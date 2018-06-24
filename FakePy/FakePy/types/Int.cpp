#include "stdafx.h"
#include "Int.h"

Int::Int(int v) {
	value = v;
	myType = Types::Integer;
}
int Int::getVal() { return value; }
void Int::setVal(int v) { value = v; }
bool Int::isPrintable() { return true; }
string Int::toString() { return to_string(value); }
varType* Int::clone() { return new Int(value); }
char Int::toChar() { return NULL; } // unsupported method

bool Int::toBool() { return value == 0 ? 0 : 1; }
int Int::toInt() { return value; }
double Int::toDouble() { return value * 1.0; }
