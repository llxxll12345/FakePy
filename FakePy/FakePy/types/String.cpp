#include "stdafx.h"
#include "String.h"
#include "Character.h"

String::String() {
	str = "";
}

String::String(string s) {
	str = s;
	myType = Types::Strings;
}
string String::getStr() {
	return str;
}
void String::setStr(string str) {
	str = str;
}
bool String::isPrintable() {
	return true;
}

string String::toString() {
	return str;
}

string String::toString_() {
	return "\"" + str + "\"";
}


varType* String::operator[](vector<varType*> l) {
	string nstr = "";
	int s = 0, e = str.size(), step = 1, i;
	switch (str.size()) {
	case 1:
		if (l[0]->myType == Types::Voids)
			return NULL;
		else {
			i = idxConv(l[0]->toInt());
			return new Character(str[i]);
		}
		break;
	case 2:		// example: [1:2]
		if (s <= e) {
			if (l[0]->myType != Types::Voids)
				s = idxConv(l[0]->toInt());

			if (l[1]->myType != Types::Voids)
				e = idxConv(l[1]->toInt());
			return new String(str.substr(s, e));
		}
		else {
			printf("Begin > End while step is positive by default :%d->%d\n", s, e);
			return NULL;
		}
		break;
	case 3:		// example: [1:2:3]
		if (l[2]->myType != Types::Voids)
			step = l[2]->toInt();

		// if s is empty, start from the end by default if step < 0
		if (l[0]->myType != Types::Voids)
			s = idxConv(l[0]->toInt());
		else if (step < 0)
			s = str.size() - 1;
		// if e is empty, start from the beginning by default if step > 0
		if (l[1]->myType != Types::Voids)
			e = idxConv(l[1]->toInt());
		else if (step < 0)
			e = -1;

		if (step > 0) {
			while (s < e) {
				nstr += str[s];
				s += step;
			}
		}
		else if (step < 0) {
			while (s > e) {
				nstr += str[s];
				s += step;
			}
		}
		else {
			printf("Step error :%d\n", step);
			return NULL;
		}

		return new String(nstr);
		break;
	default:
		printf("Length error :%d\n", l.size());
		return NULL;
		break;
	}
}


char String::toChar() { return NULL; } // unsupported method
int String::toInt() { return str.size(); }

void String::set(varType* idx, varType* item) {
	int i = idxConv(idx->toInt());
	str[i] = item->toChar();
}

int String::idxConv(int id) {
	if (id < 0)
		id += str.size();
	if (id < 0 || id > str.size() - 1) {
		printf("Index error %d\n", id);
	}
	return id;
}

varType* String::clone() {
	return new String(str);
}

bool String::toBool() {
	return str != "";
}

varType* String::operator+(varType* other) {
	return new String(str + other->toString());
}

varType* String::operator*(varType* other) {
	string nstr = "";
	int num = other->toInt();
	for (int i = 0; i < num; i++)
		nstr += str;
	return new String(nstr);
}

varType* String::operator==(varType* other) {
	return (varType*) new Bool(str == other->toString());
}

varType* String::operator!=(varType* other){
	return (varType*) new Bool(str == other->toString());
}
