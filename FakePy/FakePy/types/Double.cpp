#include "stdafx.h"
#include "Double.h"


Double::Double(double v) {
	value = v;
	myType = Types::Doubles;
}
double Double::getVal() { return value; }
void Double::setVal(double v) { value = v; }
bool Double::isPrintable() { return true; }
string Double::toString() { return to_string(value); }
varType* Double::clone() { return new Double(value); }

bool Double::toBool() { return value == 0 ? 0 : 1; }
int Double::toInt() { return (int)value; }
int Double::toNInt() { return (int)(value + 0.5); }
char Double::toChar() { return NULL; }	// unsupported
double Double::toDouble() { return value; }