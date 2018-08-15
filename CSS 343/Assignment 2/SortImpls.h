#pragma once
#include <vector>
using namespace std;

const int MIN_SIZE = 4;

void BubbleSort(vector<int>&, int, int);
void InsertionSort(vector<int>&, int, int);
void MergeSort(vector<int>&, int, int);
void Merge(vector<int>&, int, int, int);
void IterativeMergeSort(vector<int>&, int, int);
void NoCopyMerge(vector<int>&, vector<int>&, int, int, int);
void ShellSort(vector<int>&, int, int);
void QuickSort(vector<int>&, int, int);
int Partition(vector<int>&, int, int);
int SortFirstMiddleLast(vector<int>&, int, int);

void BubbleSort(vector<int> &item, int first, int last) //Carrano
{
	bool sorted = false;
	int pass = 1;
	while (!sorted && (pass < last))
	{
		sorted = true;
		for (int index = 0; index < last - pass; index++)
		{
			int nextIndex = index + 1;
			if (item[index] > item[nextIndex])
			{
				swap(item[index], item[nextIndex]);
				sorted = false;
			}
		}
		pass++;
	}
}

void InsertionSort(vector<int> &item, int first, int last) // Carrano
{
	for (int unsorted = first + 1; unsorted <= last; unsorted++)
	{
		int nextItem = item[unsorted];
		int loc = unsorted;
		while ((loc > first) && (item[loc - 1] > nextItem))
		{
			item[loc] = item[loc - 1];
			loc--;
		}

		item[loc] = nextItem;
	}
}

void MergeSort(vector<int> &item, int first, int last) { //Carrano
	if (first < last)
	{
		int mid = first + (last - first) / 2;
		MergeSort(item, first, mid);
		MergeSort(item, mid + 1, last);
		Merge(item, first, mid, last);
	}
}

void Merge(vector<int> &item, int first, int mid, int last) { //Carrano
	vector<int> tempArray(last + 1);
	int first1 = first;
	int last1 = mid;
	int first2 = mid + 1;
	int last2 = last;

	int index = first1;
	while ((first1 <= last1) && (first2 <= last2))
	{
		if (item[first1] <= item[first2])
		{
			tempArray[index] = item[first1];
			first1++;
		}
		else
		{
			tempArray[index] = item[first2];
			first2++;
		}
		index++;
	}
	while (first1 <= last1)
	{
		tempArray[index] = item[first1];
		first1++;
		index++;
	}
	while (first2 <= last2)
	{
		tempArray[index] = item[first2];
		first2++;
		index++;
	}
	for (index = first; index <= last; index++)
	{
		item[index] = tempArray[index];
	}
}

void QuickSort(vector <int> &item, int first, int last) //Carrano
{
	if (last - first + 1 < MIN_SIZE)
	{
		InsertionSort(item, first, last);
	}
	else
	{
		int pivotIndex = Partition(item, first, last);
		QuickSort(item, first, pivotIndex - 1);
		QuickSort(item, pivotIndex + 1, last);
	}
}

int SortFirstMiddleLast(vector<int> &item, int first, int last) //Carrano
{
	int mid = first + (last - first) / 2;

	if (item[first] > item[mid])
	{
		swap(item[first], item[mid]);
	}
	if (item[mid] > item[last])
	{
		swap(item[mid], item[last]);
	}
	if (item[first] > item[mid])
	{
		swap(item[first], item[mid]);
	}
	return mid;
}

int Partition(vector<int> &item, int first, int last) // Carrano
{
	int pivotIndex = SortFirstMiddleLast(item, first, last);

	swap(item[pivotIndex], item[last - 1]);
	pivotIndex = last - 1;
	int pivot = item[pivotIndex];

	int indexFromLeft = first + 1;
	int indexFromRight = last - 2;

	bool done = false;
	while (!done)
	{
		while (item[indexFromLeft] < pivot)
		{
			indexFromLeft = indexFromLeft + 1;
		}

		while (item[indexFromRight] > pivot)
		{
			indexFromRight = indexFromRight - 1;
		}

		if (indexFromLeft < indexFromRight)
		{
			swap(item[indexFromLeft], item[indexFromRight]);
			indexFromLeft = indexFromLeft + 1;
			indexFromRight = indexFromRight - 1;
		}
		else
		{
			done = true;
		}
	}
	swap(item[pivotIndex], item[indexFromLeft]);
	pivotIndex = indexFromLeft;
	return pivotIndex;
}

void ShellSort(vector<int> &item, int first, int last) //Professor Dimpsey's Ppt.
{
	for (int gap = last / 2; gap > 0; gap = (gap == 2) ? 1 : int(gap / 2.2))
	{
		for (int i = gap; i < last; i++)
		{
			int tmp = item[i];
			int j = i;
			for (; (j >= gap) && (tmp < item[j - gap]); j -= gap)
			{
				item[j] = item[j - gap];
			}
			item[j] = tmp;
		}
	}
}

void IterativeMergeSort(vector<int> &item, int first, int last)
{
	vector<int> temp(last + 1);

	for (int size = 1; size < item.size(); size += size)
	{
		int s = 0;
		while (s < item.size()) {
			NoCopyMerge(item, temp, s, s + size, s + 2 * size);
			s = s + (size * 2);
		}
		swap(item, temp);
	}
}

void NoCopyMerge(vector<int>& start, vector<int>& end, int first, int mid, int last)
{
	if (mid > start.size())
	{
		mid = start.size();
	}
	if (last > start.size())
	{
		last = start.size();
	}

	int first1 = first;
	int mid1 = mid;
	int curIndex = first1;

	while (curIndex < last)
	{
		if (first1 == mid)
		{
			end[curIndex] = start[mid1];
			mid1++;
		}
		else if (mid1 == last)
		{
			end[curIndex] = start[first1];
			first1++;
		}
		else if (start[mid1] < start[first1])
		{
			end[curIndex] = start[mid1];
			mid1++;
		}
		else
		{
			end[curIndex] = start[first1];
			first1++;
		}
		curIndex++;
	}
}