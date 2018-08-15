#include "bintree.h"

//************************************************************
// Method:    BinTree
// Access:    public 
// Returns:   
// Details:   Assigns root to null
//************************************************************
BinTree::BinTree()
{
	root = nullptr;
}

//************************************************************
// Method:    BinTree
// Access:    public 
// Returns:   
// Details:   Copy constructor for binary tree objects
//************************************************************
BinTree::BinTree(const BinTree &rhs)
{
	if (rhs.root == nullptr)
	{
		root = nullptr;
		return;
	}

	clone(rhs.root, root);
}

//************************************************************
// Method:    ~BinTree
// Access:    public 
// Returns:   
// Details:   Destructor method for BinTree objects.
//************************************************************
BinTree::~BinTree()
{
	destructor(root, root);
	root = nullptr;
}

//************************************************************
// Method:    operator<<
// Access:    public 
// Returns:   std::ostream&
// Details:   Overloaded ostream operator to print the tree.
//************************************************************
ostream& operator<<(ostream &os, const BinTree &T)
{
	T.printTree(os, " ", T.root);
	os << endl;
	return os;
}

//************************************************************
// Method:    printTree
// Access:    private 
// Returns:   void
// Details:   Recursive in-order function to add nodedata into
// ostream.
//************************************************************
void BinTree::printTree(ostream &os, string delimiter, Node* root) const
{
	if (root == nullptr) return;
	printTree(os, delimiter, root->left);
	os << *root->data << delimiter;
	printTree(os, delimiter, root->right);
	return;
}

//************************************************************
// Method:    clone
// Access:    private 
// Returns:   void
// Details:   Pre-order traversal, assigning new memory by value.
//************************************************************
void BinTree::clone(Node* origin, Node *& destination)
{
	if (origin == nullptr)
	{
		// lhs = rhs. if rhs is null, then assign lhs = null
		destination = nullptr;
		return;
	}
	else
	{
		// Pre-order traversal (NLR)
		// Copy nodes downwards
		Node *tmp = new Node;
		(*tmp).data = new NodeData(*origin->data);
		destination = tmp;
		clone(origin->left, destination->left);
		clone(origin->right, destination->right);
		return;
	}
}

//************************************************************
// Method:    isEmpty
// Access:    public 
// Returns:   bool
// Details:   Returns whether or not a tree is empty. Empty 
// here is defined as whether the root exists.
//************************************************************
bool BinTree::isEmpty() const
{
	if (root == nullptr) return true;
	return false;
}

//************************************************************
// Method:    makeEmpty
// Access:    public 
// Returns:   void
// Details:   *  Proxy for destructor().
// Attempts to call destructor() to delete the individual 
// NodeData and Nodes, then kills the tree root.
//************************************************************
void BinTree::makeEmpty()
{
	destructor(root, root);
	root = nullptr;
}

//************************************************************
// Method:    destructor
// Access:    private 
// Returns:   void
// Details:   Traverses LRN (postorder because we want to reach
// both children before destroying the node above the L/R)
//************************************************************
void BinTree::destructor(Node* root, Node* cur)
{
	if (root == nullptr) return;
	if (root == nullptr || (root != nullptr && cur == nullptr))  return;

	if (cur != nullptr)
	{
		if (cur->left != nullptr) destructor(root, cur->left);
		if (cur->right != nullptr) destructor(root, cur->right);
	}

	if (cur != nullptr && cur->data != nullptr)
	{
		if (cur->data != nullptr)
		{
			// cerr << "@deleted: nodeData/" << current->data << endl;
			delete cur->data;
			cur->data = nullptr;
		}

		// cerr << "@deleted: node/" << current << endl;
		delete cur;
		cur = nullptr;
	}
}

//************************************************************
// Method:    isEqual
// Access:    private 
// Returns:   bool
// Details:   Checks to see if two bintree objects are the same.
//************************************************************
bool BinTree::isEqual(Node* lhs, Node* rhs) const
{
	// checks to see if btrees are equal, return early if true.
	if (lhs == nullptr && rhs != nullptr) return false;
	if (lhs != nullptr && rhs == nullptr) return false;

	if (lhs != nullptr && rhs != nullptr)
	{
		if (lhs->left != nullptr && rhs->left == nullptr) return false;
		if (lhs->left == nullptr && rhs->left != nullptr) return false;
		if (lhs->right != nullptr && rhs->right == nullptr) return false;
		if (lhs->right == nullptr && rhs->right != nullptr) return false;
	}

	// begin traversing
	if (lhs != nullptr && rhs != nullptr)
	{
		if (((*lhs->data) == (*rhs->data)))
		{
			if (isEqual(lhs->left, rhs->left) == isEqual(lhs->right, rhs->right))
			{
				return true;
			}
			else { return false; }
		}
		else { return false; }
	}

	return true;
}

//************************************************************
// Method:    operator==
// Access:    public 
// Returns:   bool
// Details:   Operator overload for ==, makes a call to isEqual.
//************************************************************
bool BinTree::operator==(const BinTree &rhs) const
{
	return isEqual(root, rhs.root);
}

//************************************************************
// Method:    operator!=
// Access:    public 
// Returns:   bool
// Details:   Operator overload for !=, makes a call to isEqual.
//************************************************************
bool BinTree::operator!=(const BinTree &rhs) const
{
	return false == isEqual(root, rhs.root);
}

//************************************************************
// Method:    operator=
// Access:    public 
// Returns:   BinTree::BinTree&
// Details:   Operator overload for =, makes a call to clone,
// after checking if the two are not equal.
//************************************************************
BinTree& BinTree::operator=(const BinTree &rhs)
{
	if (!isEqual(root, rhs.root))
	{
		destructor(root, root);
		clone(rhs.root, root);
	}

	return *this;
}

//************************************************************
// Method:    insert
// Access:    public 
// Returns:   bool
// Details:   Iteratively inserts NodeData objects into the
// binary tree.
//************************************************************
bool BinTree::insert(NodeData* ptr)
{
	Node* cur = root;
	Node* tmp = new Node;
	(*tmp).data = ptr; (*tmp).left = (*tmp).right = nullptr;

	if (root == nullptr)
	{
		root = tmp;
		return true;
	}
	else if (root != nullptr)
	{
		if (*cur->data == *tmp->data)
		{
			// Refuse duplicate root before we start traversing
			delete tmp;
			return false;
		}
	}

	while (cur != nullptr)
	{
		NodeData data = *cur->data;
		if (data == *tmp->data) { delete tmp; return false; }

		if (data > *tmp->data)
		{
			if (cur->left == nullptr)
			{
				// There's nothing here so we can just set it
				cur->left = tmp;
				return true;
			}

			cur = cur->left;
		}
		else
		{
			if (cur->right == nullptr)
			{
				// There's nothing here so we can just set it
				cur->right = tmp;
				return true;
			}

			cur = cur->right;
		}

		if (cur->left == nullptr && cur->right == nullptr) { continue; }
	}

	delete tmp;
	return false;
}

//************************************************************
// Method:    retrieve
// Access:    public 
// Returns:   bool
// Details:   Retrieves a specified node data object, and 
// assigns its pointer it back to the retval parameter.
//************************************************************
bool BinTree::retrieve(const NodeData &find, NodeData *&retval) const
{
	if (root == nullptr) return false;
	if (root != nullptr && *root->data == find)
	{
		retval = root->data;
		return true;
	}

	Node* cur = root;
	while (cur != nullptr)
	{
		NodeData data = *cur->data;
		if (data == find)
		{
			retval = cur->data;
			return true;
		}

		cur = (data > find) ? cur->left : cur->right;
	}

	return false;
}

//************************************************************
// Method:    getHeightHelper
// Access:    private 
// Returns:   int
// Details:   Traverses through the binary tree recursively
// to reach a specific node.
//************************************************************
int BinTree::getHeightHelper(Node* cur) const
{
	if (cur == nullptr) return 0;
	int left = getHeightHelper(cur->left);
	int right = getHeightHelper(cur->right);
	return left >= right ? ++left : ++right;
}

//************************************************************
// Method:    getHeight
// Access:    public 
// Returns:   int
// Details:   Attempts to find node passed in as a parameter
// when the node is found, pass that node to the get height helper.
//************************************************************
int BinTree::getHeight(const NodeData &find) const
{
	Node* retval = nullptr;
	if (root != nullptr && *root->data == find)
	{
		retval = root;
	}
	else
	{
		Node* cur = root;
		while (cur != nullptr)
		{
			NodeData data = *cur->data;
			if (data == find)
			{
				// found it.
				retval = cur;
				break;
			}

			cur = (data > find) ? cur->left : cur->right;
		}
	}
	return getHeightHelper(retval);
}

//************************************************************
// Method:    getRoot
// Access:    public 
// Returns:   NodeData*
// Details:   Returns the root node of the tree.
//************************************************************
NodeData* BinTree::getRoot() const
{
	return root->data;
}

// UNMODIFIED FROM ZIP/SPEC
//------------------------- displaySideways ---------------------------------
// Displays a binary tree as though you are viewing it from the side;
// hard coded displaying to standard output.
// Preconditions: NONE
// Postconditions: BinTree remains unchanged.
void BinTree::displaySideways() const
{
	sideways(root, 0);
}

// UNMODIFIED FROM ZIP/SPEC
//---------------------------- Sideways -------------------------------------
// Helper method for displaySideways
// Preconditions: NONE
// Postconditions: BinTree remains unchanged.
void BinTree::sideways(Node* current, int level) const
{
	if (current != NULL)
	{
		level++;
		sideways(current->right, level);

		// indent for readability, 4 spaces per depth level 
		for (int i = level; i >= 0; i--)
		{
			cout << "    ";
		}

		cout << *current->data << endl;        // display information of object
		sideways(current->left, level);
	}
}

//************************************************************
// Method:    bstreeToArray
// Access:    public 
// Returns:   void
// Details:	  Creates a bstree from an array, after creating
// the the array, the bstree is emptied.
//************************************************************
void BinTree::bstreeToArray(NodeData* data[])
{
	int dataIndex = 0;
	bstreeToArrayHelper(root, data, dataIndex);
	root = nullptr;
}

//************************************************************
// Method:    bstreeToArrayHelper
// Access:    private 
// Returns:   void
// Details:   Helper method for bstreeToArray, traverses tree
// in order then deletes each node visited.
//************************************************************
void BinTree::bstreeToArrayHelper(Node* cur, NodeData* data[], int& dataIndex)
{
	// Standard boring LNR in-order traversal
	if (cur == nullptr) return;
	if (cur->left != nullptr) bstreeToArrayHelper(cur->left, data, dataIndex);
	data[dataIndex++] = cur->data;
	if (cur->right != nullptr) bstreeToArrayHelper(cur->right, data, dataIndex);
	delete cur;
}

//************************************************************
// Method:    arrayToBSTree
// Access:    public 
// Returns:   void
// Details:   Creates an array from a bstree. After creating
// the bstree, assigns null to the elements within the array.
//************************************************************
void BinTree::arrayToBSTree(NodeData* data[])
{
	int elemInUse = 0;
	// Can't easily get array size when passing it in as a parameter
	// Since it decays to a pointer
	// And can't sizeof after dereferencing because it'll just be an individual element
	// So to fix it, loop through each elem and count one by one.
	while (data[elemInUse]) elemInUse++;
	root = arrayToBstHelper(root, data, 0, --elemInUse);
}

//************************************************************
// Method:    arrayToBstHelper
// Access:    private 
// Returns:   BinTree::BinTree::Node*
// Details:   Calculates the mid point within the array, and
// recursively fills the btree.
//************************************************************
BinTree::Node* BinTree::arrayToBstHelper(Node* bst, NodeData* data[], int low, int high)
{
	/*
	* PDF:
	* The root (recursively) is at (low+high)/2 where low is the
	* lowest subscript of the array range and high is the highest.
	*/
	if (low > high) return nullptr;
	int mid = (low + high) / 2;
	bst = new Node;
	bst->left = nullptr;
	bst->right = nullptr;
	bst->data = data[mid];
	data[mid] = nullptr;
	// In the middle of the array
	// Shift the middle for each end individually
	bst->left = arrayToBstHelper(bst->left, data, low, mid - 1);
	bst->right = arrayToBstHelper(bst->right, data, mid + 1, high);
	return bst;
}