#pragma once
#ifndef __BIGNUM_H__
#define __BIGNUM_H__
#include "stdafx.h"
#include "assert.h"
#define nlst vector<int>
#define ll long long
/* the number stored in the vector is in regular order */
const string ZERO = "0";
const char ZCH = '0';
/*
class bigNum {
private:
	vector<int> num;
public: 
	bigNum(vector<int> &n) {
		this->num = n;
	}

	bigNum(string s) {
		this->num = strToVec(s);
	}

	int comp(bigNum y) {
		nlst a = num, b = y.num;
		if (a.size() < b.size())
			return -1;
		else if (a.size() > b.size())
			return 1;
		for (int i = 0; i < a.size(); i++) {
			if (a[i] == b[i])
				continue;
			else
				return (a[i] > b[i]) ? 1 : -1;
		}
		return 0;
	}

	int comp(string a, string b) {
		if (a.size() < b.size())
			return -1;
		else if (a.size() > b.size())
			return 1;
		for (int i = 0; i < a.size(); i++) {
			if (a[i] == b[i])
				continue;
			else
				return (a[i] > b[i]) ? 1 : -1;
		}
		return 0;
	}

	bigNum bigNumMul(bigNum y) {
		vector<int> a = num, b = y.num;
		vector<int> res(a.size() + b.size() + 1, '0');
		reverse(a.begin(), a.end());
		reverse(b.begin(), b.end());
		int carry = 0;
		for (int i = 0; i < a.size(); i++) {
			for (int j = 0; j < b.size(); j++) {
				res[i + j] += (a[i] * b[j]) % 10;
				res[i + j + 1] += (a[i] * b[j]) / 10;
			}
		}
		for (int i = 0; i < res.size() - 1; i++) {
			res[i + 1] += res[i] / 10;
			res[i] %= 10;
		}
		while (res.back() == 0 && res.size() > 1) {
			res.pop_back();
		}
		reverse(res.begin(), res.end());
		//printfres);
		return bigNum(res);
	}

	bigNum bigNumPlus(bigNum y) {
		nlst a = num, b = y.num;
		nlst res;
		int carry = 0;
		for (int i = a.size() - 1, j = b.size() - 1; i >= 0 || j >= 0; i--, j--) {
			int digit;
			digit = (i < 0) ? b[j] + carry : (j < 0) ? a[i] + carry : a[i] + b[j] + carry;
			carry = digit / 10;
			res.push_back(digit % 10);
		}
		assert(carry < 10);
		res.push_back(carry);
		while (res.back() == 0 && res.size() > 1) {
			res.pop_back();
		}
		reverse(res.begin(), res.end());
		return bigNum(res);
	}

	string bgMinus(string a, string b) {
		printf("a : %s\n", a.c_str());
		printf("b : %s\n", b.c_str());
		string res(a.size(), '0');
		int carry = 0;
		reverse(a.begin(), a.end());
		reverse(b.begin(), b.end());
		for (int i = 0; i < a.size() || i < b.size(); i++) {
			int digit = (i >= b.size()) ? a[i] - '0' - carry :
				(i >= a.size()) ? b[i] - '0' - carry : a[i] - b[i] - carry;
			res[i] = digit < 0 ? (digit + 10) + '0' : digit + '0';
			carry = digit < 0 ? 1 : 0;
		}

		getchar();
		while (res.size() > 1 && res.back() == '0') res.pop_back();
		reverse(res.begin(), res.end());
		printf("res : %s\n", res.c_str());
		return res;
	}


	vector<int> numToVec(int num) {
		vector<int> res;
		while (num) {
			res.push_back(num % 10);
			num /= 10;
		}
		reverse(res.begin(), res.end());
		return res;
	}

	vector<int> strToVec(string &a) {
		vector<int> res;
		for (int i = 0; i < a.length(); i++) {
			res.push_back(a[i] - '0');
		}
		return res;
	}

	bool isZero(vector<int> &a) {
		for (int i = 0; i < a.size(); i++) {
			if (a[i] != 0)
				return false;
		}
		return true;
	}

	bool isZero(string a) {
		for (int i = 0; i < a.size(); i++) {
			if (a[i] != '0')
				return false;
		}
		return true;
	}

	bigNum divide_strings(string b) {
		string a = (num), b = y.num;
		string q(a.size(), ZCH);
		if (comp(b) == -1) {
			return bigNum(ZERO);
		}
		if (comp(b) == 0) {
			return bigNum(ZERO) ;
		}
		int idx = 0;
		string temp;
		while (idx < a.size()) {
			int cnt = 0;
			while (idx < a.size() && comp(temp, b) == -1) {			// temp < b
				temp += a[idx++];
			}
			printf("temp : %s\n", temp.c_str());
			while (comp(temp, b) != -1) {			// temp >= b
				temp = bgMinus(temp, b);
				cnt++;
			}
			printf("CNT: %d \n", cnt);
			printf("temp1 : %s\n", temp.c_str());
			q[idx - 1] = cnt + '0';
			if (idx < a.size() && isZero(temp)) {
				for (; idx < a.size() && a[idx] == '0'; idx++);
				printf("idx %d\n", idx);
				temp = "";
			}
			//assert(cnt < 10);

		}
		printf("q : %s\n", q.c_str());
		if (!temp.size()) temp = '0';
		while (q[0] == '0') {
			q.erase(q.begin());
			q.shrink_to_fit();
		}
		return temp;
	}



	bigNum operator+(bigNum &other) const {

	}

	bigNum operator-(bigNum &other) const {

	}

	bigNum operator*(bigNum &other) const {

	}

	bigNum operator/(bigNum &other) const {

	}

	bigNum operator%(bigNum &other) const {

	}

	bigNum operator=(bigNum &other) const {

	}

	bigNum operator>(bigNum &other) const {

	}

	bigNum operator<(bigNum &other) const {

	}

	bigNum operator>=(bigNum &other) const {

	}

	bigNum operator<=(bigNum &other) const {

	}

	bigNum operator==(bigNum &other) const {

	}
};


void bigNumTest() {
	

}*/

#endif