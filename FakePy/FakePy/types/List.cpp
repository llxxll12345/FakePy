#include "stdafx.h"
#include "List.h"
#include "String.h"

List::List() {
	myType = Types::Lists;
	setCanClone(false);
}

List::List(vector<varType*> l) {
	for (varType* val: l) {
		if (val->getCanClone())
			mylist.push_back(val->clone());
		else
			mylist.push_back(val);
	}
}

List::~List() {
	for (varType* val: mylist)
		delete val;
}

vector<varType*> List::getList() {
	return mylist;
}
void List::setList(vector<varType*> l) {
	mylist = l;
}

void List::set(varType* idx, varType* item) {
	int i = idxConv((*idx).toInt());
	mylist[i] = item;
}

int List::idxConv(int idx) {
	if (idx < 0) idx += mylist.size();
	if (idx < 0 || idx > mylist.size() - 1) {
		printf("idx out of bounds: %d\n", idx);
		return NULL;
	}
	return idx;
}

void List::add(varType* item) {
	if (item->getCanClone()) mylist.push_back(item->clone());
	else mylist.push_back(item);
}

bool List::isPrintable() {
	return true;
}

string List::toString() {
	if (mylist.empty())
		return "[]";
	string str;
	for (varType* val : mylist)
		if (val->myType != Types::Strings)
			str += ", " + val->toString();
		else
			str += ", " + ((String*)val)->toString_();
	return '[' + str.substr(2) + ']';
}

varType* List::clone() {
	List* nlst = new List();
	for (varType* val : mylist) {
		nlst->add(val);
	}
	return nlst;
}

varType* List::operator+(varType* other)  {
	List* nlst = (List*) clone();
	if (other->myType != Types::Lists)
		nlst->add(other);
	else {
		for (varType* val: ((List*) other)->getList())
			nlst->add(val);
	}
	return nlst;
}
varType* List::operator*(varType* other) {
	int num = other->toInt();
	List* nlst = new List();
	for (int i = 0; i < num; i++)
		for (varType* val : mylist)
			nlst->add(val);
	return nlst;
}

bool List::toBool() {
	return !mylist.empty();
}

int List::toInt() {
	return mylist.size();
}


char List::toChar() { return NULL; } // unsupported method

varType* List::operator[] (vector<varType*> l) {
	vector<varType*> nlst;
	int s = 0, e = mylist.size(), step = 1, i;
	switch (l.size()) {
	case 1:
		if (l[0]->myType == Types::Voids)
			return NULL;
		i = idxConv(l[0]->toInt());
		if (mylist[i]->getCanClone())
			return mylist[i]->clone();
		return mylist[i];
		break;
	case 2:		// example: [1:2]
		if (l[0]->myType != Types::Voids)
			s = idxConv(l[0]->toInt());

		if (l[1]->myType != Types::Voids)
			e = idxConv(l[1]->toInt());
		return new List(vector<varType*>(mylist.begin() + s, mylist.begin() + e));
		break;
	case 3:		// example: [1:2:3]
		if (l[2]->myType != Types::Voids)
			step = l[2]->toInt();

		if (l[0]->myType != Types::Voids)
			s = idxConv(l[0]->toInt());
		else if (step < 0)
			s = mylist.size() - 1;

		if (l[1]->myType != Types::Voids)
			e = idxConv(l[1]->toInt());
		else if (step < 0)
			e = -1;

		if (step > 0) {
			while (s < e) {
				if (mylist[s]->getCanClone())
					nlst.push_back(mylist[s]->clone());
				else
					nlst.push_back(mylist[s]);
				s += step;
			}
		}

		else if (step < 0) {
			while (s > e) {
				if (mylist[s]->getCanClone())
					nlst.push_back(mylist[s]->clone());
				else
					nlst.push_back(mylist[s]);
				s += step;
			}
		}
		else {
			printf("Step error :%d\n", step);
			return NULL;
		}

		return new List(nlst);
		break;
	default:
		printf("Length error :%d\n", l.size());
		return NULL;
		break;
	}
}
