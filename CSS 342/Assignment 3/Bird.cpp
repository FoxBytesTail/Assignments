#include "Bird.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

Bird::Bird()
{
	uniqueID = 0;
	name = "";
}

Bird::Bird(string _name)
{
	name = _name;
}

Bird::Bird(string _name, int _id)
{
	name = _name;
	uniqueID = _id;
}

string Bird::getName() const
{
	return name;
}

int Bird::getId() const
{
	return uniqueID;
}

istream &operator >> (istream & is, Bird &other)
{
	string line;
	if (!(getline(is, line)))
	{
		cerr << "undefined input" << endl;
		exit(1);
	}
	istringstream iss(line);

	iss >> other.name >> other.uniqueID;

	return is;
}

ostream & operator<<(ostream & os, const Bird & other)
{
	os << other.name << other.uniqueID;
	return os;
}

bool operator<(const Bird & lhs, const Bird & rhs)
{
	return lhs.uniqueID < rhs.uniqueID;
}

bool operator>(const Bird & lhs, const Bird & rhs)
{
	return lhs.uniqueID > rhs.uniqueID;
}

bool operator==(const Bird & lhs, const Bird & rhs)
{
	return (lhs.uniqueID == rhs.uniqueID) && (lhs.name == rhs.name);
}

bool operator!=(const Bird & lhs, const Bird & rhs)
{
	return !(lhs == rhs);
}

