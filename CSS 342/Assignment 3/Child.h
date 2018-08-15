#pragma once
#include <string>
using namespace std;
class Child 
{
public:
	Child();
	Child(string, string, int);
	string getLastName() const;
	string getFirstName() const;
	int getAge() const;
	friend istream& operator>> (istream& is, Child &other);
	friend ostream& operator<< (ostream& os, const Child& other);
	friend bool operator<(const Child& lhs, const Child& rhs);
	friend bool operator>(const Child& lhs, const Child& rhs);
	friend bool operator==(const Child& lhs, const Child& rhs);
	friend bool operator!=(const Child& lhs, const Child& rhs);
private:
	string firstName, lastName;
	int age;
};