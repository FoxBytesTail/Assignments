#ifndef GRAPHL_H
#define GRAPHL_H

#include "nodedata.h"

struct EdgeNode;
struct GraphNode
{
	NodeData data;
	bool visited{ false };
	EdgeNode* edgeHead{ nullptr };
};
struct EdgeNode
{
	int adjGraphNode{ 0 };
	EdgeNode* nextEdge{ nullptr };
};

class GraphL
{
	static const int MAXNODES = 101;
public:
	GraphL();
	~GraphL();
	void buildGraph(istream&);
	void connect(const int, const int);
	bool isNodeVisited(int);
	void setVisitState(int, bool);
	void depthFirstSearch();
	void displayGraph() const;

private:
	int vertexCount = 0;
	GraphNode adjacencyList[MAXNODES];
	void depthFirstSearchHelper(int, GraphNode, string&);

};
#endif
