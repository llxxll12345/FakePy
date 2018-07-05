#include "stdafx.h"
#include "String.h"
#include "Dictionary.h"
#include <cstdio>
#include "../exceptions/syntaxExceptionh.h"
#include "../exceptions/parameterException.h"
#define dt unordered_map<varType*, varType*> 

Dictionary::Dictionary() {
	myType = Types::Dicts;
	setCanClone(false);
}

Dictionary::Dictionary(dt d) {
	dt temp;
	dt::iterator iter = d.begin();
	for (; iter != d.end(); iter++) {
		varType* Key = iter->first;
		varType* val = iter->second;
		if (val->getCanClone()) {
			temp.insert(make_pair(Key, val->clone()));
		}
		else {
			temp.insert(make_pair(Key, val));
		}
	}
}

Dictionary::~Dictionary() {
	for (auto val : dict)
		delete val.second;
}

dt Dictionary::getDict() {
	return dict;
}
void Dictionary::setDict(dt d) {
	dict.clear();
	dt::iterator iter = d.begin();
	for (; iter != d.end(); iter++) {
		varType* Key = iter->first;
		varType* val = iter->second;
		if (val->getCanClone()) {
			dict.insert(make_pair(Key, val->clone()));
		}
		else {
			dict.insert(make_pair(Key, val));
		}
	}
}

void Dictionary::set(varType* Key, varType* val) {
	if (Key->myType != Types::Strings) {
		printf("Dict error: Invalid key.\n");
		throw new syntaxException();
	}
	if (val->getCanClone())
		dict[Key] = val->clone();
	else
		dict[Key] = val;
}

void Dictionary::add(pair<varType*, varType*> p) {
	add(p.first, p.second);
}

void Dictionary::add(varType* Key, varType* val) {
	if (Key->myType != Types::Strings) {
		printf("Dict error: Invalid key.\n");
		throw new syntaxException();
	}
	if (val->getCanClone())
		dict.insert(make_pair(Key, val->clone()));
	else
		dict.insert(make_pair(Key, val));
}

bool Dictionary::isPrintable() {
	return true;
}

string Dictionary::toString() {
	if (dict.empty())
		return "{}";
	string str;
	dt::iterator iter = dict.begin();
	for (; iter != dict.end(); iter++) {
		str += ((String*) (iter->first))->toString_();
		str += ": ";
		varType* val = iter->second;
		if (val->myType != Types::Strings)
			str += val->toString() + ", ";
		else
			str += ((String*)val)->toString_() + ", ";
	}
	return '{' + str.substr(0, str.size()-2) + '}';
}

varType* Dictionary::clone() {
	Dictionary* nlst = new Dictionary();
	dt::iterator iter = dict.begin();
	for (; iter != dict.end(); iter++) {
		if (iter->second->getCanClone())
			nlst->add(iter->first, iter->second->clone());
		else
			nlst->add(iter->first, iter->second);
	}
	return nlst;
}


varType* Dictionary::operator+(pair<varType*, varType*> p) {
	Dictionary* nlst = (Dictionary*)clone();
	if (p.second->myType != Types::Dicts)
		nlst->add(p);
	else {
		for (pair<varType*, varType*> val: ((Dictionary*)p.second)->getDict())
			nlst->add(val);
	}
	return nlst;
}


bool Dictionary::toBool() {
	return !dict.empty();
}

int Dictionary::toInt() {
	return dict.size();
}


char Dictionary::toChar() { return NULL; } // unsupported method

varType* Dictionary::operator[] (varType* Key) {
	if (Key->myType != Types::Strings) {
		printf("Dict error: Invalid key.\n");
		throw new syntaxException();
	}
	if (!dict.count(Key)) {
		printf("No such key exists!\n");
		throw new parameterException();
	}
	return dict[Key];
}
