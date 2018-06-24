#include "stdafx.h"
#include "Character.h"

Character::Character(char v) {
	value = v;
	myType = Types::Characters;
}
char Character::getVal() { return value; }
void Character::setVal(char v) { value = v; }
bool Character::isPrintable() { return true; }
string Character::toString() { return to_string(value); }
varType* Character::clone() { return new Character(value); }

bool Character::toBool() { return value == 0 ? 0 : 1; }
int Character::toInt() { return (int) value; }
double Character::toDouble() { return ((int) value) * 1.0; }
char Character::toChar() { return value; }
