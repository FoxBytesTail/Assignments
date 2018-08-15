#include "Robot.h"
#include <string>

const string NORTH = "N";
const string SOUTH = "S";
const string EAST = "E";
const string WEST = "W";

using namespace std;
Robot::Robot() {
	Coordinate origin = Coordinate(0, 0);
	Coordinate destination = Coordinate(0, 0);

	DeterminePaths(origin, destination);
}
Robot::Robot(int x1, int y1, int x2, int y2) {
	Coordinate origin = Coordinate(x1, y1);
	Coordinate destination = Coordinate(x2, y2);

	DeterminePaths(origin, destination);
}
Robot::~Robot() {}

void Robot::DeterminePaths(Coordinate origin, Coordinate destination)
{
	findPath(origin, destination, "", pathsArr);
}

void Robot::findPath(Coordinate origin, Coordinate destination, string path, vector<string> &pathsArr) {
	if ((origin.x == destination.x) && (origin.y == destination.y) && path.length() != 0) { 
		pathsArr.push_back(path);
		return; 
	}
	if (origin.y < destination.y) {
		/* y+1 = up  1 */ findPath(Coordinate(origin.x + 0, origin.y + 1), Coordinate(destination.x, destination.y), path + NORTH, pathsArr);
	}
	if (origin.x > destination.x) {
		/*x-1 = left 1 */ findPath(Coordinate(origin.x - 1, origin.y + 0), Coordinate(destination.x, destination.y), path + WEST, pathsArr);
	}
	if (origin.x < destination.x) {
		/*x+1 = right1 */ findPath(Coordinate(origin.x + 1, origin.y - 0), Coordinate(destination.x, destination.y), path + EAST, pathsArr);
	}
	if (origin.y > destination.y) {
		/*y-1 = down 1 */ findPath(Coordinate(origin.x + 0, origin.y - 1), Coordinate(destination.x, destination.y), path + SOUTH, pathsArr);
	}
}

ostream & operator<<(ostream & os, const Robot other)
{
	for (auto & path : other.pathsArr)
		os << path << endl;
	os << "Number of paths: " << other.pathsArr.size() << endl;
	return os;
}
