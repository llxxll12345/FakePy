#pragma once
#ifndef __PATTERN_H__
#define __PATTERN_H__

#include "stdafx.h"
#include "state.h"

class Pattern {
public:
	State *prev;
	int	  startId;
	Pattern() : prev(NULL), startId(-1) {};
	Pattern(const Pattern &other) {
		*this = other;
	}
	~Pattern() {};
	Pattern& operator=(const Pattern& other) {
		prev = other.prev;
		startId = other.startId;
		return *this;
	}
};

#endif