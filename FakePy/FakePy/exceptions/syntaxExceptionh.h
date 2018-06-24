#pragma once
#include "stdafx.h"
#ifndef  __SYNTAXEXCEPTION_H__
#define  __SYNTAXEXCEPTION_H__
#include <exception>
#include "generalException.h"
class syntaxException : public generalException {
public:
	const char* what() const throw() {
		return "Syntax exception!\n";
	}
};

#endif //
