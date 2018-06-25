#include "stdafx.h"
#ifndef __DEPENDENCIES_H__
#define __DEPENDENCIES_H__
#include "fileGraph.h"
bool dfs(fileGraph* g, int u, int* vis);
bool bfs(fileGraph* g, int h);
void topoSort(fileGraph* g, int u, int* vis, stack<string>& s);
vector<vector<string>> getBlocks(fileGraph* g);

#endif // !__DEPENDENCIES_H__
