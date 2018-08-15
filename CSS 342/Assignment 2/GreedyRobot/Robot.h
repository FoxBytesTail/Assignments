#pragma once
#include "Coordinate.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Robot {
 public:
	Robot();
	Robot(int, int, int, int);
	~Robot();
	friend ostream& operator<<(ostream& os, const Robot other);

 private:
	 void DeterminePaths(Coordinate origin, Coordinate destination);
	 void findPath(Coordinate origin, Coordinate destination, string, vector<string>&);
	 vector <string> pathsArr;
};
