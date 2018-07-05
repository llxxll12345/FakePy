#include "stdafx.h"
#include "fileGraph.h"

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