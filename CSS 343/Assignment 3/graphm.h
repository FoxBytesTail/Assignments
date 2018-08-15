#ifndef GRAPHM_H
#define GRAPHM_H

#include "nodedata.h"
#include <iostream>
#include <limits>
#include <iomanip>
#define MAXNODES 101

using namespace std;

class GraphM
{

public:
	GraphM();

	void reset();

	void buildGraph(ifstream&);
	void insertEdge(int, int, int);
	void removeEdge(int, int);
	void findShortestPath();
	void displayAll();
	void display(int, int);
	bool pathExists(int, int);

private:
	struct TableType
	{
		bool visited{ false };
		int dist{ numeric_limits<int>::max() };
		int path{ 0 };
	};

	NodeData data[MAXNODES];
	int C[MAXNODES][MAXNODES];
	int size{ 0 };
	TableType T[MAXNODES][MAXNODES];

	void printPath(int, int);
	void printRouteNames(int, int);
};

#endif