#pragma once
#include "stdafx.h"
#ifndef  __PARAMETEREXCEPTION_H__
#define  __PARAMETEREXCEPTION_H__
#include <exception>
#include "generalException.h"

class parameterException : public generalException {
public:
	const char* what() const throw() {
		return "Parameter exception!\n";
	}
};

#endif //PARAMETEREXCEPTION_H__#pragma once
