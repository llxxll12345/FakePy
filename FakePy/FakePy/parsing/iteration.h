#include "stdafx.h"
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../exceptions/parameterException.h"
#include "exprNode.h"
#include "../types/List.h"
class whileBlock {
private:
	exprNode * condition;

};
class forBlock {
private:
	vector<varType*> iterVar;
	List* varList;
	unordered_map<string, varType*> localMap;
	
public:
	
};


#endif 

