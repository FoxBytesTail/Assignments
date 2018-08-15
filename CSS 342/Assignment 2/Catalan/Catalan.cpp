// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int catalan(int n) {
	int sum = 0;

	if (n == 1)
		return 1;
	if (n == 0)
		return 1;
	for (int i = 0; i < n; i++) {
		sum += catalan(i) * catalan((n-1) - i);
	}
	return sum;
} 

int main()
{
	int n;
	cout << "Catalan ";
	cin >> n;
	while (n < 0) {
		cout << "Enter an integer greater than 0." << endl;
		cin >> n;
	}

	int cat = catalan(n);
	cout << cat << endl;

	system("pause");
    return 0;
}

