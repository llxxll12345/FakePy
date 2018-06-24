#pragma once
#include"stdafx.h"
class Stream {
public:
	Stream(const char* str);
	~Stream();
	int peek();
	int nxt();
	void back();

private:
	const char *str;
	int			pos;
};