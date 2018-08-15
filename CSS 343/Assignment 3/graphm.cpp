#include "graphm.h"
#include <string>
#include <limits>

// -----------------------------------GraphM----------------------------------------------------------
// Description: Provides the default constructor for GraphM objects. Makes a call to reset() to
// initialize graph objs.
// ---------------------------------------------------------------------------------------------------
GraphM::GraphM()
{
	reset();
}

// -----------------------------------reset----------------------------------------------------------
// Description: Initializes the Table struct and the corresponding cost array.
// ---------------------------------------------------------------------------------------------------
void GraphM::reset()
{
	size = 0;

	for (int row = 1; row < MAXNODES; row++)
	{
		for (int col = 1; col < MAXNODES; col++)
		{
			T[row][col].visited = false;
			T[row][col].path = 0;
			T[row][col].dist = numeric_limits<int>::max();
			C[row][col] = numeric_limits<int>::max();
		}
	}
}

// -----------------------------------BuildGraph-----------------------------------------------------
// Description: Builds a matrix from a .txt file containing input data.
// Assumptions: First line contains the number of node. With an equal amount of descriptions to the
// num of nodes. After that there are 3 ints describing an edge from one node to another along with
// the cost. 
// 
// Read file until 0 0 terminates the file.
// ---------------------------------------------------------------------------------------------------
void GraphM::buildGraph(ifstream& file)
{
	// initialize vars
	int nodes, src, dest, edge;
	string loc;

	// read in first line, using a tmp var here to check later.
	nodes = 0;
	file >> nodes;

	//return early if file is empty
	if (file.eof())
	{
		return;
	}

	// reads in the description of location
	if (nodes > 0 && nodes < MAXNODES)
	{
		src = dest = edge = 0;
		size = nodes;
		getline(file, loc);

		for (int i = 1; i <= size; i++)
		{
			data[i].setData(file);
		}

		// use do/while to read into src/dest/edge at least onc
		// or else it's not going to run in the first place
		do
		{
			file >> src >> dest >> edge;
			insertEdge(src, dest, edge);
		}
		while (!file.eof() && (src != 0 && dest != 0 && edge != 0));
	}
}

// -----------------------------------insertEdge-----------------------------------------------------
// Description: Helper method that inserts the cost into the cost array.
// ---------------------------------------------------------------------------------------------------
void GraphM::insertEdge(int src, int dest, int cost)
{
	// assumes well formed input.
	if (src == 0) return;
	if (dest == 0) return;
	C[src][dest] = cost;
}

// -----------------------------------removeEdge-----------------------------------------------------
// Description: Helper method that sets a path in the cost array to infinity.
// ---------------------------------------------------------------------------------------------------
void GraphM::removeEdge(int src, int dest)
{
	// assumes well formed input
	C[src][dest] = numeric_limits<int>::max();
}

// -----------------------------------pathExists-----------------------------------------------------
// Description: Helper method that returns a bool determining if there is a path.
// ---------------------------------------------------------------------------------------------------
bool GraphM::pathExists(int a, int b)
{
	// if cost is infinity then no path exists
	if (C[a][b] == numeric_limits<int>::max())
		return false;
	return true;
}

// ---------------------------------printPath---------------------------------------------------------
// Description:  Prints the path from source to destination
// ---------------------------------------------------------------------------------------------------
void GraphM::printPath(int src, int dest)
{
	if (src == dest)
	{
		cout << " " << dest;
	}
	else
	{
		printPath(src, T[src][dest].path);
		cout << " " << dest;
	}
}

// -----------------------------------printRouteNames-----------------------------------------
// Description: Prints the path names (name of each location) taken from source to destination
// -------------------------------------------------------------------------------------------
void GraphM::printRouteNames(int src, int dest)
{
	if (src == dest)
	{
		if (data != nullptr)
		{
			cout << data[dest] << endl;
		}
		return;
	}

	if (src != dest)
	{
		// Look up next step to take
		printRouteNames(src, T[src][dest].path);
		if (data != nullptr)
		{
			cout << data[dest] << endl;
		}
	}

	return;
}

// -----------------------------------findShortestPath------------------------------------------------
// Description: finds the shortest path between all nodes in the graph. Then, update Table T with 
// shortest path info.
// ---------------------------------------------------------------------------------------------------
void GraphM::findShortestPath()
{
	int v = 0, w = 0, src = 0;
	// going from node1 -> node1 is always 0 cost / 0 distance
	// set everything to 0
	while (src++ < size && size != 0)
		T[src][src].dist = 0;

	src = 0;
	while (src++ < size && size != 0)
	{
		v = 0;
		while (true)
		{
			if (v == -1) break;
			int lowcost = numeric_limits<int>::max();
			int dest = 0;

			while (dest++ < size)
			{
				if (T[src][dest].visited) continue;

				v = (T[src][dest].dist < lowcost) ? dest : v;
				lowcost = (T[src][dest].dist < lowcost) ? T[src][dest].dist : lowcost;
			}

			T[src][v].visited = true;
			if (lowcost == numeric_limits<int>::max()) break;

			int w = 0;
			while (w++ < size)
			{
				if (T[src][w].visited) continue;
				if (!pathExists(v, w)) continue;
				if (!(T[src][w].dist < T[src][v].dist + C[v][w]))
				{
					T[src][w].path = v;

					// determines the minimum path and updates if necessary
					T[src][w].dist = (T[src][w].dist < (T[src][v].dist + C[v][w])) ? T[src][w].dist : (T[src][v].dist + C[v][w]);
				}
			}
		}
	}
}

// -----------------------------------display---------------------------------------------------------
// Description: Prints the path numbers from source to destination
//              Uses a helper/utility to also print the names of each point taken in the path 
// ---------------------------------------------------------------------------------------------------
void GraphM::display(int src, int dest)
{
	if (size < 1) return;

	if (T[src][dest].dist < numeric_limits<int>::max())
	{
		// Print route then list of route names
		cout << src << "\t" << dest << "\t" << T[src][dest].dist << "\t";
		printPath(src, dest);
		cout << endl;
		printRouteNames(src, dest);
		cout << endl;
	}
	else
	{
		cout << src << "\t" << dest << "\t";
		for (int i = 0; i < 5; i++)
			cout << '-';
		cout << endl;
	}
}

// -----------------------------------displayAll------------------------------------------------------
// Description: Prints the shortest path between all nodes in the graph.
// ---------------------------------------------------------------------------------------------------
void GraphM::displayAll()
{
	if (size > 0)
	{
		// print header
		cout << "Description\tFrom\tTo\tDijkstra\tPath" << endl;
		int rowIndex = 0;

		while (rowIndex++ < size)
		{
			// print node name
			cout << data[rowIndex] << endl;
			int columnIndex = 0;
			while (columnIndex++ < size)
			{

				TableType temp = T[rowIndex][columnIndex];
				if (temp.dist == 0) continue;

				// print from and to nodes
				cout << "\t\t" << rowIndex << "\t" << columnIndex << "\t";

				if (temp.dist < numeric_limits<int>::max())
				{
					// print distance
					cout << "" << T[rowIndex][columnIndex].dist << "\t\t";
					printPath(rowIndex, columnIndex);
					cout << endl;
				}

				if (temp.dist == numeric_limits<int>::max())
				{
					cout << "----\t" << endl;
				}
			}
		}
	}
}