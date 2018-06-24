#include "stdafx.h"
#include "regex.h"
#include "assert.h"

#define EPSILON   -1 // epsilon
#define ANY       '*'// closure
#define MULT      '+'// plus closure
#define OPT		  '?'// 
#define LPAREN    '('// open paren
#define RPAREN    ')'// close paren
#define OR        '|'
#define FCAT      '&'// false concat


regex::regex() {

}

regex::~regex() {
	cleanUp();
}

bool regex::isOp(char ch) {
	return ((ch == FCAT) || (ch == OR) || (ch == ANY) || (ch == MULT) || (ch == OPT) || (ch == LPAREN) || (ch == RPAREN));
}

bool regex::isInput(char ch) { 
	return(!isOp(ch)); 
}

bool regex::isLParan(char ch) {
	return ch == LPAREN;
}

bool regex::isRParan(char ch) {
	return ch == RPAREN;
}

void regex::cleanUp() {
	for (int i = 0; i < NFAtable.size();i++) {
		delete NFAtable[i];	// State->~State();
	}
	NFAtable.clear();

	for (int i = 0; i < NFAtable.size();i++) {
		delete DFAtable[i];
	}
	DFAtable.clear();

	nxtStateId = 0;

	while (!sStk.empty())
		sStk.pop();
	while (!eStk.empty())
		eStk.pop();

	input.clear();
}

bool regex::poptable(table &NFAtable) {
	if (sStk.size() > 0) {
		NFAtable = sStk.top();
		sStk.pop();
		return true;
	}
	return false;
}

bool regex::cat() {
	table lt, rt;
	if (poptable(rt) || poptable(lt))
		return false;

	(*(lt.rbegin()))->addTrans(EPSILON, (*rt.begin()));	// first pos and last pos
	lt.insert(lt.end(), rt.begin(), rt.end());      // follow pos;
	sStk.push(lt);
	return true;
}



void regex::pushsStk(char ch) {
	State *s0 = new State(nxtStateId++);
	State *s1 = new State(nxtStateId++);

	s0->addTrans(ch, s1);

	table subtable;
	subtable.push_back(s0);
	subtable.push_back(s1);
	sStk.push(subtable);
	input.insert(ch);
}

bool regex::opt() {
	table prev;
	if (!poptable(prev))
		return false;

	State* lt = new State(nxtStateId++);
	State* rt = new State(nxtStateId++);

	lt->addTrans(EPSILON, rt);
	lt->addTrans(EPSILON, (*prev.begin()));
	(*(prev.rbegin()))->addTrans(EPSILON, rt);

	prev.insert(prev.begin(), lt);
	prev.push_back(rt);

	sStk.push(prev);
	return true;
}


bool regex::mult() {
	table prev;
	if (poptable(prev))
		return false;

	State *lt = new State(nxtStateId++);
	State *rt = new State(nxtStateId++);
	
	lt->addTrans(EPSILON, (*(prev.begin())));
	(*prev.rbegin())->addTrans(EPSILON, rt);
	(*prev.rbegin())->addTrans(EPSILON, *(prev.begin()));

	prev.insert(prev.begin(), lt);
	prev.push_back(rt);

	sStk.push(prev);
	return true;
}
bool regex::any() {

	table Prevtable;
	if (!poptable(Prevtable))
		return false;

	State *Lefttable = new State(nxtStateId++);
	State *Righttable = new State(nxtStateId++);

	Lefttable->addTrans(EPSILON, Righttable);
	Lefttable->addTrans(EPSILON, ((*(Prevtable.begin()))));
	((*(Prevtable.rbegin())))->addTrans(EPSILON, Righttable);
	((*(Prevtable.rbegin())))->addTrans(EPSILON, ((*(Prevtable.begin()))));

	Prevtable.insert(Prevtable.begin(), Lefttable);
	Prevtable.push_back(Righttable);

	sStk.push(Prevtable);

	return true;
}

bool regex::Or() {

	table  Uppertable, Lowertable;

	if (!poptable(Lowertable) || !poptable(Uppertable))
		return false;

	State *Lefttable = new State(nxtStateId++);
	State *Righttable = new State(nxtStateId++);

	Lefttable->addTrans(EPSILON, ((*(Uppertable.begin()))));
	Lefttable->addTrans(EPSILON, ((*(Lowertable.begin()))));
	((*(Uppertable.rbegin())))->addTrans(EPSILON, Righttable);
	((*(Lowertable.rbegin())))->addTrans(EPSILON, Righttable);

	Lowertable.push_back(Righttable);
	Uppertable.insert(Uppertable.begin(), Lefttable);
	Uppertable.insert(Uppertable.end(), Lowertable.begin(), Lowertable.end());

	sStk.push(Uppertable);
	return true;
}
void regex::eps(std::set<State*> startSet, std::set<State*> &result) {
	std::stack<State*> UnVisitedStates;
	result.clear();
	result = startSet;
	stateIter iter;
	for (iter = startSet.begin(); iter != startSet.end(); ++iter) {
		UnVisitedStates.push(*iter);
	}

	while (!UnVisitedStates.empty()) {
		State* curState = UnVisitedStates.top();
		UnVisitedStates.pop();

		table epsilonStates;
		curState->getTrans(EPSILON, epsilonStates);

		TIter epsilonItr;

		for (epsilonItr = epsilonStates.begin(); epsilonItr != epsilonStates.end(); ++epsilonItr) {
			if (result.find((*epsilonItr)) == result.end()) {
				result.insert((*epsilonItr));
				UnVisitedStates.push((*epsilonItr));
			}
		}
	}
}

void regex::move(char chInput, std::set<State*> NFAState, std::set<State*> &Result) {
	Result.clear();
	stateIter iter;
	for (iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
		table States;
		(*iter)->getTrans(chInput, States);
		for (int index = 0; index < States.size(); ++index) {
			Result.insert(States[index]);
		}
	}
}

void regex::convertNFAtoDFA() {
	for (int i = 0; i < (int)DFAtable.size(); ++i)
		delete DFAtable[i];
	DFAtable.clear();

	if (NFAtable.size() == 0)
		return;

	std::set<State*> NFAStartStateSet;
	NFAStartStateSet.insert(NFAtable[0]);

	std::set<State*> DFAStartStateSet;
	eps(NFAStartStateSet, DFAStartStateSet);
	
	nxtStateId = 0;
	State *DFAStartState = new State(DFAStartStateSet, nxtStateId++);

	DFAtable.push_back(DFAStartState);

	table UnVisitedStates;
	UnVisitedStates.push_back(DFAStartState);
	while (!UnVisitedStates.empty()) {

		State* CurDFAState = UnVisitedStates[UnVisitedStates.size() - 1];
		UnVisitedStates.pop_back();

		set<char>::iterator iter;
		for (iter = input.begin(); iter != input.end(); ++iter) {
			set<State*> MoveRes, EpsilonClosureRes;

			move(*iter, CurDFAState->getNFAState(), MoveRes);
			eps(MoveRes, EpsilonClosureRes);

			stateIter MoveResItr;
			stateIter EpsilonClosureResItr;

			bool bFound = false;
			State *s = NULL;
			for (int i = 0; i < (int)DFAtable.size(); ++i) {
				s = DFAtable[i];
				if (s->getNFAState() == EpsilonClosureRes) {
					bFound = true;
					break;
				}
			}
if (!bFound) {
	State* U = new State(EpsilonClosureRes, nxtStateId++);
	UnVisitedStates.push_back(U);
	DFAtable.push_back(U);
	CurDFAState->addTrans(*iter, U);
}
else {
	CurDFAState->addTrans(*iter, s);
}
		}
	}
	reduceDFA();
}

void regex::reduceDFA() {
	set<State*> DeadEndSet;
	for (int i = 0; i < DFAtable.size(); ++i) {
		if (DFAtable[i]->isDead()) {
			DeadEndSet.insert(DFAtable[i]);
		}
	}
	if (DeadEndSet.empty())
		return;
	stateIter iter;
	for (iter = DeadEndSet.begin(); iter != DeadEndSet.end(); ++iter) {
		for (int i = 0; i < DFAtable.size(); ++i)
			DFAtable[i]->remTrans(*iter);

		TIter pos;
		for (pos = DFAtable.begin(); pos != DFAtable.end(); ++pos) {
			if (*pos == *iter) {
				break;
			}
		}
		DFAtable.erase(pos);
		delete *iter;
	}
}

bool regex::checkEq(table a, table b) {
	if (a.size() == b.size()) {
		TIter itera, iterb;
		for (itera = a.begin(), iterb = b.begin(); itera != a.end(); itera++, iterb++) {
			if ((*itera) != (*iterb))
				return false;
		}

	}
	return false;
}

bool regex::eqHelper(vector<table> a, vector<table> b) {
	if (a.size() == b.size()) {
		for (int i = 0; i < a.size(); i++) {
			if (!checkEq(a[i], b[i]))
				return false;
		}
		return true;
	}
	return false;
}

bool mycomp(State* a, State * b) {
	return a->groupId < b->groupId;
}

void regex::minimizeDFA(){}

/*
void regex::minimizeDFA() {
	vector<table> curGroup(DFAtable.size()), lastGroup(DFAtable.size());
	printf("curGroup size: %d\n", curGroup.size());
	TIter iter;
	for (iter = DFAtable.begin(); iter != DFAtable.end(); iter++) {
		if (!(*iter)->acceptingState) {
			(*iter)->groupId = 1;
		}
		else (*iter)->groupId = 0;
		curGroup[(*iter)->groupId].push_back(*iter);
	}	
	int maxGpid = 1;
	//printf("input: %d\n", input.size());
	while (!eqHelper(curGroup, lastGroup)) {
		lastGroup = curGroup;
		curGroup.clear();
		for (int i = 0; i <= DFAtable.size(); i++) {
			table temp;
			curGroup.push_back(temp);
		}
		for (int i = 0; i <= lastGroup.size(); i++) {
			table gp = lastGroup[i];
			printf("gp\n");
			printtable(gp);
			for (int j = 0; j < gp.size(); j++) {
				for (int k = j + 1; k < gp.size(); k++) {
					State *a = gp[j], *b = gp[k];
					if (a->groupId == b->groupId && a->stateId != b->stateId) {
						set<char>::iterator chiter;
						for (chiter = input.begin(); chiter != input.end(); chiter++) {
							table ta, tb;
							a->getTrans(*chiter, ta);
							b->getTrans(*chiter, tb);
							printf("asz:%d, bsz:%d, aid:%d, bid: %d\n", ta.size(), tb.size(), a->stateId, b->stateId);
							if (ta.size() != 0 && tb.size() != 0) {
								assert(ta.size() == 1 && tb.size() == 1);
								curGroup[i].push_back(a);
								if (ta[0]->groupId != tb[0]->groupId) {
									curGroup[++maxGpid].push_back(b);
									b->groupId = maxGpid;
								}
								else curGroup[i].push_back(a);
							}
							else {
								curGroup[i].push_back(a);
								curGroup[++maxGpid].push_back(b);
								b->groupId = maxGpid;
							}
						}
					}
				}
			}
		}

		assert(maxGpid <= DFAtable.size());
	}

	printf("Group nums: %d", curGroup.size());
	table newTable;
	for (int i = 0; i <= maxGpid; i++) {
		newTable.push_back(curGroup[i][0]);
	}
	printtable(newTable);
	return;
}
*/
string regex::preBracket(string strregex) {
	int startPos, endPos, separatorPos;
	string ReplacedStrregex;

	startPos = strregex.find_first_of("[");
	endPos = strregex.find_first_of("]");
	separatorPos = strregex.find_first_of("-");

	if (startPos == -1 || endPos == -1)
		return strregex;

	ReplacedStrregex += strregex.substr(0, startPos);
	ReplacedStrregex.push_back('(');
	string result = strregex.substr(startPos + 1, endPos - startPos - 1);
	char first = result[0];
	char last = result[result.size() - 1];

	if (separatorPos != std::string::npos) {
		while (first != last) {
			ReplacedStrregex.push_back(first);
			ReplacedStrregex += "|";
			first++;
		}
		ReplacedStrregex.push_back(first);
	}
	else {
		startPos++;
		while (startPos != endPos - 1) {
			ReplacedStrregex.push_back(strregex[startPos]);
			ReplacedStrregex += "|";
			startPos++;
		}
		ReplacedStrregex.push_back(strregex[endPos - 1]);
	}
	ReplacedStrregex += strregex.substr(endPos + 1, strregex.size() - endPos);
	ReplacedStrregex += ")";

	return ReplacedStrregex;
}

bool regex::compile(string strregex) {

	while (strregex.find("[") != std::string::npos) {
		strregex = preBracket(strregex);
	}

	infix = const_cast<char*>(strregex.c_str());
	cleanUp();

	if (!constructThompsonNFA())
		return false;

	poptable(NFAtable);
	NFAtable[NFAtable.size() - 1]->acceptingState = true;

	convertNFAtoDFA();
	//minimizeDFA();

	//printtable(DFAtable);
	//printtable(NFAtable);
	return true;
}

bool regex::match(string strText) {
	strText = strText;
	State *pState = DFAtable[0];
	//printtable(DFAtable);
	vector<State*>  Transition;
	for (int i = 0; i < (int)strText.size(); ++i) {
		char CurrChar = strText[i];
		pState->getTrans(CurrChar, Transition);
		if (Transition.empty())
			return false;
		pState = Transition[0];
		if (pState->acceptingState)
			return true;

	}
	return false;
}

/////////////////////////////////Convert expression into postfix////////////////

int regex::preCh() {
	if (curCh == '\0')
		return -1;
	if (curCh == '\\') {
		curCh = *(infix++);
		if (curCh == '\0')
			return -1;
		postfix += curCh;
		curCh = *(infix++);
	}
	else if (!isOp(curCh) || curCh == FCAT) {
		if (curCh == '&')
			postfix += "\\&";
		else
			postfix += curCh;

		curCh = *(infix++);
	}
	else if (curCh == '(') {
		curCh = *(infix++);
		preOr();
		if (curCh != ')')
			return -1;
		curCh = *(infix++);
	}
	else
		return -1;
	return 0;
}
int regex::preClo() {
	preCh();
	while (curCh != '\0' && (!isOp(curCh) || curCh == '&')) {
		postfix += curCh;
		curCh = *(infix++);
	}
	return 0;
}
int regex::preCat(void) {
	preClo();
	while (!isOp(curCh) || curCh == '&') {
		preClo();
		postfix += "&";
	}
	return 0;
}
int regex::preOr() {
	preCat();
	while (curCh == '|') {
		curCh = *(infix++);
		preCat();
		postfix += "|";
	}
	return 0;
}

//shutting-yard
int regex::convertToPostfix() {
	curCh = *(infix++);
	preOr();
	return 0;
}


/////////////////////////////////Dfa construction////////////////
bool regex::constructThompsonNFA() {
	convertToPostfix();
	for (int i = 0; i < (int)postfix.size(); ++i) {
		char curChar = postfix[i];
		if (isInput(curChar)) {
			pushsStk(curChar);
		}
		else if (isOp(curChar)) {
			switch (curChar) {
			case ANY:
				any();
				break;
			case MULT:
				mult();
				break;
			case OPT:
				opt();
				break;
			case OR:
				Or();
				break;
			case FCAT:
				cat();
				break;
			}
		}
	}
	return true;
}


/////////////////////////////////Print the table of states////////////////
void regex::printtable(table &t) {
	string res;
	TIter i;
	printf("\nStates with available acceptingState\n");
	for (i = t.begin(); i != t.end(); i++) {
		if ((*i)->acceptingState) {
			res += "\t" + (*i)->getId() + "\n";
		}
	}
	res += '\n';
	for (int i = 0; i < t.size(); ++i) {
		State *pState = t[i];
		table State;
		pState->getTrans(EPSILON, State);
		for (int j = 0; j < State.size(); ++j) {
			res += "\t" + pState->getId() + " -> " + State[j]->getId();
			res += "\t[label = \"epsilon \"]\n";
		}
		set<char>::iterator iter;
		for (iter = input.begin(); iter != input.end(); ++iter) {
			pState->getTrans(*iter, State);
			for (int j = 0; j < State.size(); ++j) {
				res += "\t" + pState->getId() + " -> " + State[j]->getId();
				res += "\t[label = \"" + to_string(*iter) + " \"]\n";
			}
		}
	}
	printf("%s", res.c_str());
}