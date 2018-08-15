#include "Coordinate.h"
#include "Robot.h"
#include <iostream>
#include <string>

using namespace std;
int main()
{
	//loop is bad practice, just using it to test multiple coordinates.
	while (true) {
		int x1, y1, x2, y2;
		cout << "Enter x1 y1 x2 y2: " << endl;
		cin >> x1 >> y1 >> x2 >> y2;

		Robot bot(x1, y1, x2, y2);

		cout << bot << endl;
	}
	system("pause");
	return 0;
}