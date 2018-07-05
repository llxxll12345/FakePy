#include "stdafx.h"
#ifndef __FILEGRAPH_H__
#define __FILEGRAPH_H__
struct node {
	int to, val;
	string fileName;
	node* nxt;
	node(int t, string f) : to(t), fileName(f) {};
	~node() { delete nxt; };
};

struct edge {
	int from, to, val;
	edge(int f, int t, int v = 0) {
		from = f;
		to = t;
		val = v;
	}
	~edge() {};
};

struct nodeList {
	node* head;
	string fileName;
	nodeList() {};
	nodeList(node* h, string f) : head(h), fileName(f){};
	~nodeList() { delete head; }
};

struct fileGraph {
	int E, V;
	nodeList *adj;
	fileGraph( int v, int e= NULL) {
		E = (E != NULL) ? e:0;
		V = v;
		adj = (nodeList*)malloc(v * sizeof(nodeList));
	}
	~fileGraph(){
		delete adj;
	}
};

void add_edge(fileGraph *g, int u, int v, string s1, string s2);

void printGraph(fileGraph *g);


#endif