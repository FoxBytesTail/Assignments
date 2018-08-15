#ifndef BINTREE_H
#define BINTREE_H
#include "nodedata.h"

class BinTree {
	friend ostream& operator<<(ostream&, const BinTree&);
public:
	BinTree();
	BinTree(const BinTree&);
	BinTree& operator=(const BinTree&);
	~BinTree();
	bool isEmpty() const;
	void makeEmpty();
	bool operator==(const BinTree&) const;
	bool operator!=(const BinTree&) const;
	bool insert(NodeData*);
	bool retrieve(const NodeData&, NodeData *&) const;
	void bstreeToArray(NodeData*[]);
	void arrayToBSTree(NodeData*[]);
	void displaySideways() const;
	int getHeight(const NodeData&) const;
	NodeData* getRoot() const;
private:
	struct Node {
		NodeData* data;
		Node* left;
		Node* right;
	};
	Node* root;
	void destructor(Node *, Node *);
	void clone(Node *, Node *&);
	void sideways(Node*, int) const;
	bool isEqual(Node*, Node*) const;
	void bstreeToArrayHelper(Node*, NodeData*[], int&);
	Node* arrayToBstHelper(Node*, NodeData*[], int, int);
	void printTree(ostream&, string, Node *) const;
	int getHeightHelper(Node *) const;
};

#endif
