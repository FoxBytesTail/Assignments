#include "Child.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Child::Child()
{
	firstName = "";
	lastName = "";
	age = NULL;
}

Child::Child(string _firstName, string _lastName, int _age)
{
	firstName = _firstName;
	lastName = _lastName;
	age = _age;
}

string Child::getLastName() const
{
	return lastName;
}

string Child::getFirstName() const
{
	return firstName;
}

int Child::getAge() const
{
	return age;
}

istream &operator >> (istream & is, Child &other)
{
	string line;
	if (!(getline(is, line)))
	{
		cerr << "undefined input" << endl;
		exit(1);
	}
	istringstream iss(line);

	iss >> other.firstName >> other.lastName >> other.age;

	return is;
}

ostream & operator<<(ostream & os, const Child & other)
{
	os << other.firstName << other.lastName << other.age;
	return os;
}

bool operator<(const Child & lhs, const Child & rhs)
{
	if (lhs.getLastName() == rhs.getLastName() && lhs.firstName < rhs.firstName)
	{
		return true;
	}
	else if (lhs.getLastName() < rhs.getLastName())
	{
		return true;
	}
	return false;
}

bool operator>(const Child & lhs, const Child & rhs)
{
	if (lhs.getLastName() == rhs.getLastName() && lhs.firstName > rhs.firstName)
	{
		return true;
	}
	else if (lhs.getLastName() > rhs.getLastName())
	{
		return true;
	}
	return false;
}

bool operator==(const Child & lhs, const Child & rhs)
{
	if (lhs.age == rhs.age && lhs.firstName == rhs.firstName && lhs.lastName == rhs.lastName)
	{
		return true;
	}
	return false;
}

bool operator!=(const Child & lhs, const Child & rhs)
{
		return !(lhs == rhs);
}
