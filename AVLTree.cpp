#include "AVLTree.h"

#include <string>

AVLTree::AVLNode::AVLNode() : key(), value(), height(1), left(nullptr), right(nullptr) {}
AVLTree::AVLNode::AVLNode(const KeyType& k, const ValueType& v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}

//0,1, or 2 children
size_t AVLTree::AVLNode::numChildren() const {
    size_t count = 0;
	if (left) ++count;
	if (right) ++count;
	return count;
}

//true if leaf
bool AVLTree::AVLNode::isLeaf() const {
    return !left && !right;
}

size_t AVLTree::AVLNode::getHeight() const {
    return height;
}

//height/balance/rotation helpers
size_t AVLTree::heightOf(AVLNode* node) const {
	return node ? node->height : 0;
}

int AVLTree::balanceOf(AVLNode* node) const {
	if (!node) return 0;
	return static_cast<int>(heightOf(node->left)) - static_cast<int>(heightOf(node->right));
}

void AVLTree::rotateLeft(AVLNode*& node) {
	if (!node || !node->right) return;
	AVLNode* newRoot = node->right;
	AVLNode* movedSubtree = newRoot->left;
	newRoot->left = node;
	node->right = movedSubtree;

	node->height = 1 + std::max(heightOf(node->left), heightOf(node->right));
	newRoot->height = 1 + std::max(heightOf(newRoot->left), heightOf(newRoot->right));
	node = newRoot;
}

void AVLTree::rotateRight(AVLNode*& node) {
	if (!node || !node->left) return;
	AVLNode* newRoot = node->left;
	AVLNode* movedSubtree = newRoot->right;
	newRoot->right = node;
	node->left = movedSubtree;

	node->height = 1 + std::max(heightOf(node->left), heightOf(node->right));
	newRoot->height = 1 + std::max(heightOf(newRoot->left), heightOf(newRoot->right));
	node = newRoot;
}


//insert
bool AVLTree::insert(AVLNode*& current, const KeyType& key, ValueType value)
{
	if (!current)
	{
		current = new AVLNode(key, value);
		return true; //new node inserted
	}

	bool inserted = false;
	if (key < current->key)
	{
		inserted = insert(current->left, key, value);
	} else if (key > current->key)
	{
		inserted = insert(current->right, key, value);
	} else
	{	//duplicate key
		return false;
	}

	if (inserted)
		//update height and rebalance
	{
		current->height = 1+ std::max(heightOf(current->left), heightOf(current->right));
		balanceNode(current);
	}
	return inserted;
}

bool AVLTree::insert(const KeyType& key, ValueType value)
{
	bool inserted = insert(root, key, value);
	if (inserted)
	{
		nodeCount++;
	}
	return inserted;
}


//remove helpers
bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (current->numChildren() == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    if (!current) {
		return false;
	}
	bool removed = false;

	if (key < current->key) {
		//look in left subtree
		removed = remove(current->left, key);
	} else if (key > current->key) {
		//look in right subtree
		removed = remove(current->right, key);
	} else {
		//found node
		removed = removeNode(current);
	}

	//after removal updae node's height and rebalance
	if (removed && current) {
		current->height = 1 + std::max(heightOf(current->left), heightOf(current->right));
		balanceNode(current);
	}
	return removed;
}

void AVLTree::balanceNode(AVLNode *&node) {
	if (!node) return;
	//updating nodes height
	node->height = 1 + std::max(heightOf(node->left), heightOf(node->right));

	int balanceFactor = balanceOf(node);

	//left heavy
	if (balanceFactor > 1) {
		//left child right heavy
		if (balanceOf(node->left) < 0) {
			rotateLeft(node->left);
		}
		//left-left case
		rotateRight(node);
	}
	//right heavy
	else if (balanceFactor < -1) {
		//right child left heavy
		if (balanceOf(node->right) > 0) {
			rotateRight(node->right);
		}
		//right-right case
		rotateLeft(node);
	}
}
