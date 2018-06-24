#pragma once
#include "stdafx.h"
#ifndef  __INDENTEXCEPTION_H__
#define  __INDENTEXCEPTION_H__
#include <exception>
#include "generalException.h"

class IndentException : public generalException {
public:
	const char* what() const throw() {
		return "Indentation Error!\n";
	}
};

#endif //INDENTEXCEPTION_H__