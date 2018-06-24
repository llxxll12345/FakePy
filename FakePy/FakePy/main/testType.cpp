#include "stdafx.h"
#include "vector"
#include "../types/Double.h"
#include "../types/String.h"
#include "../types/List.h"
#include "../types/Int.h"

void testType() {
	Int a(5);
	Int* e = new Int(10);
	varType* ts = (varType*)&a;
	varType* f = (varType*)e;
	double temp1 = 12.523, temp2 = 5.123;
	Double b(temp1);
	Double h(temp2);
	varType* td = (varType*)&b;
	varType* th = (varType*)&h;
	String s("asdf");
	Bool c(1);
	Int d(1);

	vector<varType*> tlst;
	vector<varType*> idx({(varType*) &Int(1), (varType*) &Int(0), (varType*) &Int(-1)});
	tlst.push_back((varType*) &a);
	tlst.push_back((varType*) &b);
	tlst.push_back((varType*) &c);
	tlst.push_back((*f) + ts);
	tlst.push_back((*td) / th);
	List t(tlst);

	t.add((varType*) &a);
	t.add((varType*) &s);
	printf("getVal %d\n", ((Int*)(tlst[0]))->getVal());
	printf("tostring: %s\n", t.toString().c_str());
	printf("Size: %d\n", t.toInt());
	printf("getIndex: %s\n", t[idx]->toString().c_str());

	String s1("asdf"), s2("asdf1");
	varType* t1 = (varType*)&s1;
	varType* t2 = (varType*)&s2;

	//string s3 = ((String*)((*t1) + t2))->toString();
	//printf("%s\n", s3.c_str());

}