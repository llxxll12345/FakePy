#include "stdafx.h"
#include "testExt.h"
#include <unordered_map>
unordered_map<string, int> idMap;
void testExt() {
	string inputName = "input.txt";
	FILE *f = fopen(inputName.c_str(), "r");
	char* line = new char();
	fileGraph g(0);
	int vNum = 0;
	while (fscanf(f, "%s", line) != EOF) {
		g.V++;
		char *from = new char();
		char *to   = new char();
		sscanf(line, "%s->%s", from, to);
		string s1(from), s2(to);
		if (!idMap.count(from)) {
			idMap.insert(make_pair(from, vNum++));
		}
		if (!idMap.count(to)) {
			idMap.insert(make_pair(to, vNum++));
		}
		add_edge(&g, idMap[from], idMap[to], from, to);
	}
	fclose(f);
	bfs(&g, 0);
	int* vis = (int*)malloc(sizeof(int) * g.V);
	for (int i = 0; i < g.V; i++) {
		if (!vis[i])
			dfs(&g, i, vis);
	}

	vector<vector<string>> res;
	res = getBlocks(&g);
	for (int i = 0; i < res.size(); i++) {
		printf("# %d :\n", i);
		for (int j = 0; j < res[i].size(); j++) {
			printf("%s\n ", res[i][j].c_str());
		}
	}

}