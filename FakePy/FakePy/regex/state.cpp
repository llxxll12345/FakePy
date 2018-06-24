#pragma once
#include "stdafx.h"
#include "state.h"
State::~State() {
	NFAStates.clear();
	trans.clear();
}

State::State(set<State*> NFA, int id) {
	NFAStates = NFA;
	stateId = id;
	acceptingState = false;
	groupId = 0;
	stateIter iter;
	for (iter = NFAStates.begin(); iter != NFAStates.end(); iter++) {
		if ((*iter)->acceptingState)
			acceptingState = true;
	}
}

State::State(const State &other) {
	*this = other;
}

void State::addTrans(char ch, State* p) {
	trans.insert(make_pair(ch, p));
}

void State::remTrans(State* p) {
	transIter iter;
	//printf("trans size: %d\n", trans.size());
	for (iter = trans.begin(); iter != trans.end();) {
		//printf("ch, %c \n", (*iter).first);
		State *to = iter->second;
		//(*iter).second->getId();
		if (to == p)
			trans.erase(iter++);
		else iter++;
	}
}

void State::getTrans(char ch, table &s) {
	s.clear();
	transIter iter;
	for (iter = trans.lower_bound(ch); iter != trans.upper_bound(ch); iter++) {
		State *p = iter->second;
		s.push_back(p);
	}
}

set<State*>& State::getNFAState() {
	return NFAStates;
}

bool State::isDead() {
	if (acceptingState)
		return false;
	if (trans.empty())
		return true;
	transIter iter;
	for (iter = trans.begin(); iter != trans.end(); iter++) {
		State* to = iter->second;
		if (to != this)
			return false;
	}
	return true;
}

State& State::operator=(const State& other) {
	this->trans     = other.trans;
	this->stateId   = other.stateId;
	this->NFAStates = other.NFAStates;
	return *this;
}

bool State::operator==(const State& other) {
	if (NFAStates.empty())
		return stateId == other.stateId;
	return NFAStates == other.NFAStates;
}

string State::getId() {
	printf("State id is: %d\n", stateId);
	return to_string(stateId);
}

 
