#include "stdafx.h"
#include "dependencies.h"
#include "fileGraph.h"
#include <queue>
#include <algorithm>
bool dfs(fileGraph* g, int u, int* vis) {
	if (vis[u]) {
		printf("Cycle exists!\n");
		return false;
	}
	vis[u] = 1;
	printf("Cur: %s\n", g->adj[u].fileName);
	for (node* h = g->adj[u].head; h; h = h->nxt) {
		int v = h->to;
		if (vis[v] == 1) {
			printf("Cycle exists!\n");
			return false;
		} 
		if (!dfs(g, v, vis)) {
			printf("Cycle exists!\n");
			return false;
		}
	}
	vis[u] = 2;
	return true;
}
bool bfs(fileGraph* g, int h) {
	int *vis = (int*)malloc(g->V * sizeof(int));
	queue<int> q;
	q.push(h);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		vis[u] = 1;
		printf("Cur: %s\n", g->adj[u].fileName);
		for (node* hd = g->adj[h].head; hd; hd = hd->nxt) {
			int v = hd->to;
			if (vis[v]) {
				printf("Cycle exists!\n");
				return false;
			}
			q.push(v);
		}
	}
}
void topoSort(fileGraph* g, int u, int* vis, stack<string>& s) {
	if (vis[u]) {
		printf("Cycle exists!\n");
		return;
	}
	vis[u] = 1;
	s.push(g->adj[u].fileName);
	printf("Cur: %s\n", g->adj[u].fileName);
	for (node* h = g->adj[u].head; h; h = h->nxt) {
		int v = h->to;
		if (vis[v] == 1) {
			printf("Cycle exists!\n");
			return;
		}
		if (!dfs(g, v, vis)) {
			printf("Cycle exists!\n");
			return;
		}
	}
	vis[u] = 2;
}

int apMin(int a, int b) {
	return a < b ? a : b;
}

void tarjan(int u, int* dfn, int* low, stack<int> &s, int &cnt, fileGraph *g, bool* onstk, vector<vector<string>> &block, int *blNum) {
	dfn[u] = low[u] = ++cnt;
	s.push(u);
	printf("Cur: %s\n", g->adj[u].fileName);
	onstk[u] = true;
	for (node* h = g->adj[u].head; h; h = h->nxt) {
		int v = h->to;
		if (!dfn[v]) {
			tarjan(v, dfn, low, s, cnt, g, onstk, block, blNum);
			low[u] = apMin(low[v], low[u]);
		}
		else if (onstk[v]){
			low[u] = apMin(dfn[v], low[u]);
		}
	}
	if (dfn[u] == low[u]) {
		vector<string> curBlock;
		int curNum = block.size();
		while (s.top() != u) {
			int t = s.top();
			s.pop();
			blNum[t] = curNum;
			curBlock.push_back(g->adj[t].fileName);
			onstk[t] = false;
		}
		int t = s.top();
		s.pop();
		blNum[t] = curNum;
		onstk[t] = false;
		curBlock.push_back(g->adj[t].fileName);
		block.push_back(curBlock);
	}
}

vector<vector<string>> getBlocks(fileGraph* g) {
	vector<vector<string>> res;
	int *dfn = (int*)malloc(sizeof(int) * g->V);
	int *low = (int*)malloc(sizeof(int) * g->V);
	int *blNum = (int*)malloc(sizeof(int) * g->V);
	bool *onstk = (bool*)malloc(sizeof(bool) * g->V);
	stack<int> s;
	int cnt = 0;
	memset(dfn, 0, sizeof(dfn));
	memset(low, 0, sizeof(low));
	memset(blNum, 0, sizeof(blNum));
	memset(onstk, 0, sizeof(onstk));
	for (int i = 0;i < g->V; i++) 
		if (!dfn[i])
			tarjan(0, dfn, low, s, cnt, g, onstk, res, blNum);

	return res;
}