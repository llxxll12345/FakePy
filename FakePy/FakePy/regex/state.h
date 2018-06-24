#pragma once
#ifndef __STATE_H__
#define __STATE_H__
#include "stdafx.h"


class State {
private:
	set<State*> NFAStates;
public:
	typedef vector<State*> table;
	typedef set<State*>::iterator stateIter;
	typedef multimap<char, State*>::iterator transIter;
	multimap<char, State*> trans;
	int stateId;
	bool acceptingState;
	bool marked;
	int	 groupId;

	State() : stateId(-1), acceptingState(false) {};
	State(int id) : stateId(id) {};
	State(set<State*> NFAState, int id);
	State(const State &other);

	~State();

	void addTrans(char ch, State* prev);
	void remTrans(State *prev);
	void getTrans(char ch, table &states);
	set<State*> &getNFAState();

	bool isDead();
	State& operator=(const State &other);
	bool operator==(const State &other);

	string getId();
};
#endif // !__STATE_H__