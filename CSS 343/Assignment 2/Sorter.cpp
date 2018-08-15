// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "SortImpls.h"
#include <windows.h>

using namespace std;

void InitArray(vector<int> &, int);
void PrintArrayDetails(const vector<int> &, string);
int main(int argc, char *argv[])
{
	int size = 0;
	string sort_name = "";
	bool printOut = true;
	if ((argc != 3) && (argc != 4))
	{
		cerr << "Usage: Sorter SORT_TYPE ARRAY_SIZE [YES|NO]" << endl;
		return -1;
	}
	sort_name = string(argv[1]);
	size = atoi(argv[2]);
	if (size <= 0)
	{
		cerr << "Array size must be positive" << endl;
		return -1;
	}
	if (argc == 4)
	{
		string printArr = string(argv[3]);
		if (printArr == "NO")
		{
			printOut = false;
		}
		else if (printArr == "YES")
		{
			printOut = true;
		}
		else
		{
			cerr << "Usage: Sorter SORT_TYPE ARRAY_SIZE [YES|NO]" << endl;
			return -1;
		}
	}
	srand(1);
	vector<int> items(size);
	InitArray(items, size);
	if (printOut)
	{
		cout << "Initial:" << endl;
		PrintArrayDetails(items, string("items"));
	}
	int begin = GetTickCount();

	if (sort_name=="MergeSort")
	{
		MergeSort(items, 0, size - 1);
	}
	if (sort_name == "IterativeMergeSort")
	{
		IterativeMergeSort(items, 0, size - 1);
	}
	if (sort_name == "ShellSort")
	{
		ShellSort(items, 0, size);
	}
	if (sort_name == "BubbleSort")
	{
		BubbleSort(items, 0, size);
	}
	if (sort_name == "InsertionSort")
	{
		InsertionSort(items, 0, size - 1);
	}
	if (sort_name == "QuickSort")
	{
		QuickSort(items, 0, size - 1);
	}
	int end = GetTickCount();
	// Linux timer:
	// gettimeofday(&endTime, 0);
	if (printOut)
	{
		cout << "Sorted:" << endl;
		PrintArrayDetails(items, string("item"));
	}
	int elapsed_secs = end - begin;
	cout << "Time (ms): " << elapsed_secs << endl;
	system("pause");
	return 0;
}
void InitArray(vector<int> &array, int randMax)
{
	if (randMax < 0)
	{
		return;
	}
	vector<int> pool(randMax);
	for (int i = 0; i < randMax; i++)
	{
		pool[i] = i;
	}
	int spot;
	for (int i = 0; i < randMax; i++)
	{
		spot = rand() % (pool.size());
		array[i] = pool[spot];
		pool.erase(pool.begin() + spot);
	}
}
void PrintArrayDetails(const vector<int> &array, string name)
{
	int size = array.size();
	for (int i = 0; i < size; i++)
		cout << name << "[" << i << "] = " << array[i] << endl;
}