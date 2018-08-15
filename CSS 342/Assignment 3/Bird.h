#pragma once
#include <iostream>
#include <string>

using namespace std;
class Bird 
{
public:
	Bird();
	Bird(string);
	Bird(string, int);

	string getName() const;
	int getId() const;

	friend istream& operator>> (istream& is, Bird &other);
	friend ostream& operator<< (ostream& os, const Bird& other);
	friend bool operator<(const Bird& lhs, const Bird& rhs);
	friend bool operator>(const Bird& lhs, const Bird& rhs);
	friend bool operator==(const Bird& lhs, const Bird& rhs);
	friend bool operator!=(const Bird& lhs, const Bird& rhs);

private:
	int uniqueID;
	string name;
};