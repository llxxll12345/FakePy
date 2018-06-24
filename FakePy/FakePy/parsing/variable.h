#pragma once
#ifndef  __VARIABLE_H__
#define  __VARIABLE_H__

#include "stdafx.h"

struct variable {
	double item;
	variable(double it) {
		item = it;
	}
};

map<string, double> varMap;

struct Func {
	double * (*fun)(double a);
	Func(double* (*a) (double a)) {
		fun = a;
	}
	double getVal(int n) {
		return *fun(n);
	}
};
map<string, Func> funcMap;

stack<string> exprStk;

template<typename T>
stack<double> varStk;

#endif // ! __VARIABLE_H__

