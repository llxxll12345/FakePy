#pragma once
#include "stdafx.h"
#ifndef __REGEX_H__
#define __REGEX_H__
#include "state.h"


class regex {
public:
	regex();
	~regex();

	bool compile(std::string pat);
	bool match(std::string str);

	void cleanUp();
	typedef vector<State*>						table;
	typedef table::reverse_iterator				rTIter;
	typedef table::iterator						TIter;
	typedef set<State*>::iterator				stateIter;
	typedef multimap<char, State*>::iterator	transIter;

private:
	table NFAtable;
	table DFAtable;

	stack<table>		sStk;  // character class stack
	stack<char>			eStk;  // expression stack
	set<char>			input; // input set
	string				text;  // strText
	string				postfix;// Poststr regex
	char*				infix; // Infix regex
	char				curCh; // curpreprocchar
	int					nxtStateId;

	bool constructThompsonNFA();
	void pushsStk(char chInput); // pushOnCharacterStack
	bool poptable(table  &NFAtable);
	bool cat();
	bool any();
	bool opt(); //closureoptional
	bool mult();
	bool Or(); //or
	bool checkEq(table a, table b);
	bool eqHelper(vector<table> a, vector<table> b);
	bool isOp(char ch);
	bool isInput(char inputCh);
	bool isLParan(char inputCh);
	bool isRParan(char inputCh);
	void eps(set<State*> T, set<State*> &Res);
	void move(char chInput, set<State*> T, set<State*> &Res);
	void convertNFAtoDFA();
	void reduceDFA();
	

	int convertToPostfix();
	int preCh();
	int preClo();
	int preCat();
	int preOr();
	string preBracket(string str);
	void minimizeDFA();

	void printtable(table &table);

protected:
};
#endif