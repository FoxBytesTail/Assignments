#include <iostream>
#include "Funds.h"
#include "BSTree.h"
#include "Account.h"

BSTree::BSTree()
{
	root = NULL;
}

BSTree::~BSTree()
{
	Destroy();
}

void BSTree::Destroy()
{
	Destroy(root);
	root = NULL;
}

void BSTree::Destroy(Node *ptr)
{
	if (ptr == NULL) { return; /* Tree is empty */ }
	if (ptr->left != NULL)
	{
		Destroy(ptr->left);
	}
	if (ptr->right != NULL)
	{
		Destroy(ptr->right);
	}

	delete ptr->pAcct;
	delete ptr;
	return;
}

bool BSTree::RecursiveInsert(Account *acc, Node *&ptr)
{
	if (ptr == NULL)
	{
		Node *tmp = new Node;
		tmp->pAcct = acc;
		tmp->left = NULL;
		tmp->right = NULL;
		ptr = tmp;

		if (root == NULL) root = ptr;
		return true;
	}
	else
	{
		if (acc->getAccountId() < ptr->pAcct->getAccountId())
		{
			RecursiveInsert(acc, ptr->left);
			return true;
		}
		else
		{
			RecursiveInsert(acc, ptr->right);
			return true;
		}
	}
	return false;
}

Account* BSTree::RecursiveRetrieve(const int id, Node*& ptr)
{
	if (ptr == NULL) return NULL;
	if (ptr != NULL)
	{
		if (ptr->pAcct->getAccountId() == id)
		{
			return ptr->pAcct;
		}
		if (ptr->pAcct != NULL && id < ptr->pAcct->getAccountId())
		{
			return RecursiveRetrieve(id, ptr->left);
		}
		else if (ptr->pAcct != NULL && id > ptr->pAcct->getAccountId())
		{
			return RecursiveRetrieve(id, ptr->right);
		}
	}

	return NULL;
}


bool BSTree::Insert(Account *value)
{
	return RecursiveInsert(value, root);
}

Account* BSTree::Retrieve(const int id)
{
	return RecursiveRetrieve(id, root);
}

void BSTree::Display() const
{
	Display(root);
}

void BSTree::Display(Node *ptr) const
{
	if (ptr == NULL)
	{
		return;
	}

	if (ptr != NULL)
	{
		if (ptr->left != NULL)
		{
			Display(ptr->left);
		}
		if (ptr != NULL)
		{
			Account* client = ptr->pAcct;
			cout << client->getName() << " Account ID: " << client->getAccountId() << endl;
			for (int i = 0; i < 10; i++)
			{
				cout << "    " << FundTypes[i] << ": $" << client->Balance(i) << endl;
			}
			cout << endl;
		}
		if (ptr->right != NULL)
		{
			Display(ptr->right);
		}
	}
}

void BSTree::Empty()
{
}

bool BSTree::isEmpty() const
{
	if (root == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
