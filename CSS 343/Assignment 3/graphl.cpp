#include "graphl.h"
#include <limits>
#include <stdexcept>
#include <string>

// ------------------------------------GraphL---------------------------------------------------------
// Description: Provides the default constructor for the GraphL class, which performs a DFS traversal. 
// ---------------------------------------------------------------------------------------------------
GraphL::GraphL()
{
}

// -----------------------------------~GraphL---------------------------------------------------------
// Description: Provides the destructor for the GraphL class, which cleans up GraphL objects upon
// program termination
// ---------------------------------------------------------------------------------------------------
GraphL::~GraphL()
{
	EdgeNode* cur;

	while (vertexCount)
	{
		cur = adjacencyList[vertexCount].edgeHead;
		while (cur != nullptr)
		{
			EdgeNode* temp = adjacencyList[vertexCount].edgeHead;
			cur = adjacencyList[vertexCount].edgeHead->nextEdge;
			if (temp != nullptr && vertexCount > -1)
				delete temp;

			adjacencyList[vertexCount].edgeHead = cur;
		}
		vertexCount--;
	}
}

// -----------------------------------BuildGraph-----------------------------------------------------
// Description: Builds an adjancency list from a .txt file containing input data.
// Assumptions: First line contains the number of node. With an equal amount of descriptions to the
// num of nodes. After that there are 2 ints describing an edge from one node to another. 
// 
// Read file until a 0 0 ends building of the file.
// ---------------------------------------------------------------------------------------------------
void GraphL::buildGraph(istream& infile)
{
	int vertex = numeric_limits<int>::max(), destinationVertex = numeric_limits<int>::max();
	string vertices;
	getline(infile, vertices);

	// check that this is a valid vertex count
	try
	{
		vertexCount = stoi(vertices);
	}
	catch (const std::invalid_argument& e)
	{
		return;
	}
	catch (...)
	{
		return;
	}

	if (vertexCount < 1)
		return;

	// read vertexCount lines to fill in descriptions of each vertex
	int parseNames = 0, parseEdges = 0;
	while (++parseNames <= vertexCount)
		adjacencyList[parseNames].data.setData(infile);

	// read until encountering 0 0
	while (vertex != 0 && destinationVertex != 0)
	{
		infile >> vertex >> destinationVertex;
		connect(vertex, destinationVertex);
	}
}

// -----------------------------------DisplayGraph----------------------------------------------------
// Description: Displays each edge node's information and edge in the graph to the console.
// ---------------------------------------------------------------------------------------------------
void GraphL::displayGraph() const
{
	// do nothing/return early if the num of vertices is 0
	if (vertexCount == 0)
		return;

	cout << "Graph: " << endl;

	// foreach vertex in graph
	int i = 0;
	while (i < vertexCount)
	{
		if (i > vertexCount)
			continue;

		// begin printing a single node
		cout << "Node " << (i + 1) << "\t\t" << adjacencyList[(i + 1)].data << "\n";
		EdgeNode* cur = adjacencyList[(i + 1)].edgeHead;

		// cur at this point points to the edge head
		// inside the loop continue onto the next edge and print each edge successively
		// stop when there are no more edges
		while (cur != nullptr)
		{
			// check to prevent errors if no adjacent graph node (= 0 = false so won't execute)
			if (cur->adjGraphNode)
			{
				// print individual edge and its start/end points
				string outputTemplate("  edge {vert} {adj}\n");
				outputTemplate.replace(outputTemplate.find("{vert}"), string("{vert}").length(), to_string(i + 1));
				outputTemplate.replace(outputTemplate.find("{adj}"), string("{adj}").length(), to_string(cur->adjGraphNode));
				cout << outputTemplate;
				cur = cur->nextEdge;
			}
		}
		i++;
	}
}

// -----------------------------------isNodeVisited----------------------------------------------------
// Description: Helper function that returns a bool if a node is visited or not.
// ---------------------------------------------------------------------------------------------------
bool GraphL::isNodeVisited(int n)
{
	return (adjacencyList[n].visited ? 1 : 0);
}

// -----------------------------------setVisitState----------------------------------------------------
// Description: helper function that changes the visited state within a specified index in 
// adjacency list.
// ---------------------------------------------------------------------------------------------------
void GraphL::setVisitState(int n, bool state)
{
	adjacencyList[n].visited = state;
}

// -----------------------------------connect---------------------------------------------------------
// Description: helper function used in build graph that connects a node to it's adjacent vertex node.
// ---------------------------------------------------------------------------------------------------
void GraphL::connect(int vertex, int adjacentVertex)
{
	// Nodes should start numbering at 1
	if (vertex == 0)
		return;
	if (adjacentVertex == 0)
		return;

	EdgeNode* edge = new EdgeNode;

	// Quick check that this is still a valid vertex and not 2.1 billion; ignore if it is
	edge->adjGraphNode = (adjacentVertex <= vertexCount) ? adjacentVertex : 0;
	edge->nextEdge = adjacencyList[vertex].edgeHead;
	adjacencyList[vertex].edgeHead = edge;
}

// -----------------------------------depthFirstSearch------------------------------------------------
// Description: Performs a depth first traversal starting from the first element (index = 0)
// Prints out the resulting traversal path
// ---------------------------------------------------------------------------------------------------
void GraphL::depthFirstSearch()
{
	// If there is no graph, this shouldn't do anything
	if (vertexCount == 0)
		return;

	int index; string dfsPath;

	// Mark all nodes as not visited before, or else calling depthFirstSearch() twice produces undefined behavior
	index = 0;
	while (++index <= vertexCount)
	{
		setVisitState(index, 0);
	}

	// Start DFS traversal
	index = 0;
	while (++index <= vertexCount)
	{
		// DFSHelper takes in a reference to a string that it will modify successively
		// When the entire execution is done, dfsPath will contain the final path taken from this point
		if (!isNodeVisited(index)) depthFirstSearchHelper(index, adjacencyList[index], dfsPath);
	}

	cout << "Depth-first ordering: " << dfsPath << endl;
}

// -----------------------------------depthFirstSearchHelper------------------------------------------
// Description: DFS Helper
// Continually appends the path travelled to a string reference originally passed in
// Recursively traverses down starting from the initial entry point
// ---------------------------------------------------------------------------------------------------
void GraphL::depthFirstSearchHelper(int vertice, GraphNode vertex, string& dfsPathTraveled)
{
	if (vertice == 0) return;

	// Set this vertex visited
	setVisitState(vertice, 1);

	// append the path traveled so far
	dfsPathTraveled.append(to_string(vertice)).append(" ");

	// start traversal at start of vertex passed in
	EdgeNode* current = vertex.edgeHead;

	do
	{
		if (current == nullptr)
			return;

		// if the next adjacent node to this one is not visited, recurse down that by calling dfsH(adjacent)
		int nextAdjacentNode = !isNodeVisited(current->adjGraphNode) ? current->adjGraphNode : 0;
		depthFirstSearchHelper(nextAdjacentNode, adjacencyList[nextAdjacentNode], dfsPathTraveled);
	}
	while (current = current->nextEdge);
}