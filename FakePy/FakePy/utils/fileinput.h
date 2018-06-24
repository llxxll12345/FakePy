#pragma once
#include "stdafx.h"
#include  <fstream>
vector<string> readFromFile(string &s) {
	vector<string> res;
	fstream fio(s.c_str(), ios::in | ios::out);
	if (fio.is_open()) {
		for (string str; getline(fio, str);) {
			res.push_back(str);
		}
	}
	return res;
}