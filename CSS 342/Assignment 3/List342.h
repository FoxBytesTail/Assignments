#ifndef LIST_342
#define LIST_342

#include "stdafx.h"
#include "Child.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

template <class ItemType>
class List342
{
public:
	List342<ItemType>();
	List342<ItemType>(const List342<ItemType>&);
	~List342<ItemType>();

	void setItem(ItemType);

	bool BuildList(string fileName);
	bool Insert(ItemType *obj);
	bool Remove(ItemType target, ItemType &result);
	bool Peek(ItemType target, ItemType &result);
	bool isEmpty() const;
	void DeleteList();
	bool Merge(List342<ItemType> &list1);

	template<class ItemType>
	friend ostream& operator<<(ostream& os, const List342<ItemType>& other);

	List342<ItemType> operator+(const List342<ItemType> &other) const;
	List342<ItemType>& operator+=(const List342<ItemType> &other);

	bool operator==(const List342<ItemType> &other) const;
	bool operator!=(const List342<ItemType> &other) const;

	void operator=(const List342<ItemType> &other);

private:
	struct Node
	{
		ItemType *data;
		Node *next;
	};
	Node *head = NULL;
};
#endif

template <class ItemType>
List342<ItemType>::List342()
{
}

template<class ItemType>
List342<ItemType>::List342(const List342<ItemType>& other)
{
	head = NULL;
	*this = other;
}

template<class ItemType>
List342<ItemType>::~List342()
{
	DeleteList();
}

template<class ItemType>
void List342<ItemType>::setItem(ItemType other)
{
	item = other;
}

template <class ItemType>
bool List342<ItemType>::BuildList(string fileName)
{
	ifstream inFile;
	inFile.open(fileName);
	if (inFile.fail())
	{
		cerr << "Error opening file" << endl;
		return false;
	}

	int count = 0;
	while (!inFile.eof())
	{
		ItemType item;
		inFile >> item;
		Insert(&item);
	}

	inFile.close();
	return true;
}

template<class ItemType>
bool List342<ItemType>::Insert(ItemType *obj)
{
	if (head == NULL)
	{
		Node* t = new Node;
		t->data = new ItemType(*obj);
		t->next = NULL;
		head = t;
		return true;
	}

	Node* current = head;
	while (current != NULL)
	{
		if (*current->data == *obj)
		{
			return false;
		}
		if (*obj < *current->data)
		{
			Node* temp2 = new Node;
			temp2->data = new ItemType(*current->data);
			temp2->next = current->next;

			current->data = new ItemType(*obj);
			current->next = temp2;

			return true;
		}
		else if (*obj > *current->data && current->next == NULL)
		{
			Node* temp3 = new Node;
			temp3->data = new ItemType(*obj);
			temp3->next = current->next;
			current->next = temp3;

			return true;
		}
		current = current->next;
	}
	return true;
}

template<class ItemType>
bool List342<ItemType>::Remove(ItemType target, ItemType & result)
{
	Node* temp;
	Node* current;
	current = head;
	if (current != NULL && *current->data == target)
	{
		temp = current;
		head = current->next;
		result = *current->data;
		delete temp;
		return true;
	}
	else if (current == NULL && current->next == NULL)
	{
		return false;
	}

	while (current != NULL)
	{
		temp = current;
		if (current->next != NULL && !(*current->data == target))
		{
			current = current->next;
		}
		if (target == *current->data)
		{
			result = *current->data;
			current = temp;
			temp = current->next;
			current->next = temp->next;
			delete temp;
			return true;
		}
		if (current->next == NULL)
		{
			return false;
		}
	}
	return false;
}


template<class ItemType>
bool List342<ItemType>::Peek(ItemType target, ItemType & result)
{
	Node* temp;
	Node* current;
	current = head;
	if (current != NULL && *current->data == target)
	{
		result = *current->data;
		return true;
	}
	else if (current == NULL && current->next == NULL)
	{
		return false;
	}

	while (current != NULL)
	{
		temp = current;
		if (current->next != NULL && !(*current->data == target))
		{
			current = current->next;
		}
		if (target == *current->data)
		{
			result = *current->data;
			return true;
		}
	}
	return false;
}

template<class ItemType>
bool List342<ItemType>::isEmpty() const
{
	if (head == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class ItemType>
void List342<ItemType>::DeleteList()
{
	Node *current = head;
	Node *temp;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		delete temp;
	}
	head = NULL;
}

template<class ItemType>
bool List342<ItemType>::Merge(List342& list1)
{
	if (list1.head == head || (list1.isEmpty() && isEmpty()))
	{
		return false;
	}
	else if (!list1.isEmpty() && isEmpty())
	{
		head = list1.head;
		list1.head = NULL;
		return true;
	}
	else if (!isEmpty() && list1.isEmpty())
	{
		return true;
	}
	else
	{
		Node* tmp = head;
		Node* ptrDest = head->next;
		Node* ptrSrc = list1.head;

		while (ptrDest != NULL && ptrSrc != NULL)
		{
			if ((ptrDest->data != NULL) && (*ptrDest->data < *ptrSrc->data || *ptrDest->data == *ptrSrc->data))
			{
				if (ptrSrc != NULL && *ptrDest->data == *ptrSrc->data)
				{
					tmp->next = ptrDest;
					ptrDest = ptrDest->next;
					Node* toDelete = ptrSrc;
					continue;
					delete toDelete->data;
					delete toDelete;
				}
				else
				{
					tmp->next = ptrDest;
					ptrDest = ptrDest->next;
				}
			}
			else
			{
				tmp->next = ptrSrc;
				ptrSrc = ptrSrc->next;
			}
			tmp = tmp->next;
		}

		while (ptrDest != NULL || ptrSrc != NULL)
		{
			if (head->next != NULL && (*head->next->data < *head->data))
			{
				Node* next = head->next;
				Node* tmp = head;
				while (tmp != NULL)
				{
					if (tmp->next != NULL)
					{
						if (*head->data > *tmp->next->data)
						{
							tmp = tmp->next;
						}
						else
						{
							Node* insert = head;
							head = head->next;
							insert->next = tmp->next;
							tmp->next = insert;
							break;
						}
					}
				}
			}
			else
			{
				tmp->next = (ptrSrc != NULL) ? ptrSrc : ptrDest;
				if (ptrSrc != NULL)
				{
					ptrSrc = ptrSrc->next;
				}
				else
				{
					ptrDest = ptrDest->next;
				}
				tmp = tmp->next;
			}
		}
	}

	list1.head = NULL; // isEmpty now returns true
	return true;
}

template<class ItemType>
List342<ItemType> List342<ItemType>::operator+(const List342<ItemType>& other) const
{
	List342<ItemType> res = *this;
	res += other;
	return res;
}

template<class ItemType>
List342<ItemType> & List342<ItemType>::operator+=(const List342<ItemType> & other)
{
	List342<ItemType> list = other;
	this->Merge(list);
	return *this;
}

template<class ItemType>
bool List342<ItemType>::operator==(const List342<ItemType> &other) const
{
	if (this == &other)
	{
		return true; 
	}
	if (head == NULL) 
	{
		return false; 
	}
	if (other.head == NULL) 
	{
		return false; 
	}
	if (head == NULL && other.head == NULL) 
	{ 
		return false;
	}

	Node *lhs = head;
	Node *rhs = other.head;
	while (lhs && rhs)
	{
		if (lhs->data != NULL && rhs->data != NULL)
		{
			if (*lhs->data == *rhs->data) 
			{ 
				lhs = lhs->next; rhs = rhs->next; continue; 
			}
			else 
			{
				return false; 
			}
		}
	}
	return true;
}

template<class ItemType>
bool List342<ItemType>::operator!=(const List342<ItemType> & other) const
{
	if (!(*this == other))
	{
		return true;
	}
	return false;
}

template<class ItemType>
void List342<ItemType>::operator=(const List342<ItemType>& other)
{
	Node *sourceNode, *destNode, *nodeToInsert;

	if (this == &other) {
		return;
	}

	if (other.head == NULL)
	{
		return;
	}

	DeleteList();
	destNode = new Node;
	destNode->data = new ItemType(*(other.head)->data);
	this->head = destNode;
	sourceNode = (other.head)->next;
	while (sourceNode != NULL)
	{
		nodeToInsert = new Node;
		nodeToInsert->data = new ItemType(*sourceNode->data);
		if (sourceNode->next == NULL) 
		{
			nodeToInsert->next = NULL;
		}
		destNode->next = nodeToInsert;
		destNode = destNode->next;
		sourceNode = sourceNode->next;
	}

}

template<class ItemType>
ostream & operator<<(ostream &os, const List342<ItemType> &other)
{
	List342<ItemType>::Node *current;
	current = other.head;
	while (current->next != NULL)
	{
		os << *current->data;
		current = current->next;
	}
	os << *current->data;
	return os;
}