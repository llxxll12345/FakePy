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

void add_edge(fileGraph *g, int u, int v, string s1, string s2) {
	nodeList nlst = g->adj[u];
	nlst.fileName = s1;
	node* h = nlst.head;
	if (!h) { 
		h = new node(v, s2); 
		return; 
	}
	while (h->nxt != NULL) {
		h = h->nxt;
	}
	h->nxt = new node(v, s2);
}

void printGraph(fileGraph *g) {
	for (int i = 0; i < g->V; i++) {
		nodeList nlst = g->adj[i];
		printf("Node %d: ", i);
		node* h = nlst.head;
		if (!h) {
			printf(" Empty\n");
			continue;
		}
		printf("%d \"%s\", ", h->to, h->fileName);
		while (h->nxt != NULL) {
			h = h->nxt;
			printf("%d \"%s\", ", h->to, h->fileName);
		}
	}
}
#endif