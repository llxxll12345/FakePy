#pragma once
#include "stdafx.h"
#ifndef  __GENERALEXCEPTION_H__
#define  __GENERALEXCEPTION_H__
#include <exception>

class generalException : public exception {
public:
	const char* what() const throw() {
		return "Error occured during interpretation.\n";
	}
};

#endif