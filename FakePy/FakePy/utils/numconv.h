#pragma once
#ifndef __NUMCONV_H__
#define __NUMCOMV_H__
#include "stdafx.h"

string decToBin32(string input) {
	string res;
	long long sum = 0;
	int sign = 0;
	if (input[0] == '-') {
		sign = 1;
		input = input.substr(2);
	}
	for (char ch : input) {
		sum = sum * 10 + (ch - '0');
	}
	while (sum) {
		res += (sum % 2 + '0');
		sum >>= 1;
	}
	if (res.size() > 31) {
		printf("Exceed 32bit format! @ dec -> bin32\n");
		return NULL;
	}
	while (res.size() < 31) {
		res += '0';
	}
	res += (sign + '0');
	reverse(res.begin(), res.end());
}

string bin32ToDec(string input) {
	string res;
	long long sum = 0;
	if (input.size() > 32) {
		printf("Exceed 32bit format! @ bin32 -> dec\n");
		return NULL;
	}
	int sign = (input[0]) == '1' ? -1 : 0;
	for (int i = 1; i < input.size(); i++) {
		if (input[i] == 1) sum += (1 << (32 - i));
	}
}

int hex32ToDec(string input) {
	if (input.size() > 8) {
		printf("Exceedint 32bit format! @ hex32 -> dec\n");
		return NULL;
	}
	int sum = 0;
	for (int i = input.size() - 1; i >= 0; i++) {
		int toBeAdd = 1 << ((input.size() - i - 1) * 4);
		int mult = 0;
		switch (input[i]) {
		case'A': mult = 10; break;
		case'B': mult = 11; break;
		case'C': mult = 12; break;
		case'D': mult = 13; break;
		case'E': mult = 14; break;
		case'F': mult = 15; break;
		default: mult = input[i] - '0';
		}
		sum += toBeAdd * mult;
	}
	return sum;
}

string DecToHex32(string input) {
	string res;
	long long sum = 0;
	bool sign = 0;
	if (input[0] == '-') {
		sign = 1;
		input = input.substr(3);
	}
	else {
		input = input.substr(2);
	}
	for (char ch : input) {
		sum = sum * 10 + (ch - '0');
	}
	while (sum) {
		char c;
		switch (sum % 16) {
		case'10':c = 'A';
		case'11':c = 'B';
		case'12':c = 'C';
		case'13':c = 'D';
		case'14':c = 'E';
		case'15':c = 'F';
		default: c = (sum % 16) - '0';
		}
		res += c;
		sum >>= 4;
	}
	if (res.size() > 8) {
		printf("Exceed 32bit format! @ dec -> bin32\n");
		return NULL;
	}
	if (sign) {
		res += "-0x";
	}
	else {
		res += "0x";
	}
	reverse(res.begin(), res.end());
	return res;
}
#endif