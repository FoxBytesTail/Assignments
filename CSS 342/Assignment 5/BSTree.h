#pragma once
#include "Account.h"
using namespace std;

class BSTree
{
private:
	struct Node
	{
		Account *pAcct;
		Node *right;
		Node *left;
	};
	Node *root = NULL;
	int count = 0;

public:
	BSTree();
	~BSTree();
	void Destroy(Node * ptr);
	bool RecursiveInsert(Account * acc, Node *& ptr);
	void Destroy();
	bool Insert(Account *account);
	Account* RecursiveRetrieve(const int id, Node *& ptr);
	Account* Retrieve(const int id);
	void Display() const;
	void Display(Node *ptr) const;
	void Empty();
	bool isEmpty() const;
};

