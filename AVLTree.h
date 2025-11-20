/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <optional>
#include <string>
#include <vector>
#include <ostream>

using namespace std;

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

		AVLNode();
		AVLNode(const KeyType& key, const ValueType& value);

        // 0, 1 or 2
        size_t numChildren() const;
        // true or false
        bool isLeaf() const;
        // number of hops to deepest leaf node
        size_t getHeight() const;


    };

public:
	//Constructors/destructor/assigners
	AVLTree();
	AVLTree(const AVLTree& other);
	AVLTree& operator=(const AVLTree& other);
	~AVLTree();

	bool insert(const KeyType& key, ValueType value);
	bool remove(const KeyType& key);
	bool contains(const KeyType& key) const;
	optional<ValueType> get(const KeyType& key) const;
	ValueType& operator[](const KeyType& key);

	vector<ValueType> findRange(const KeyType& lowKey, const KeyType& highKey) const;
	vector<KeyType> keys() const;
	size_t size() const;
	size_t getHeight() const;

	friend ostream& operator<<(ostream& os, const AVLTree& avlTree);

    private:
    AVLNode* root;
	size_t nodeCount;

    /* Helper methods for remove */

	bool insert(AVLNode*& current, const KeyType& key, ValueType value);
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);
    // removeNode contains the logic for actually removing a node based on the numebr of children
    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);

	//height/balance helpers
	size_t heightOf(AVLNode* node) const;
	int balanceOf(AVLNode* node) const;

	//rotations
	void rotateLeft(AVLNode*& node);
	void rotateRight(AVLNode*& node);

	//search helper
	AVLNode* findNode(AVLNode* current, const KeyType& key) const;

	//traversal helpers
	void findRange(AVLNode* current, const KeyType& lowKey, const KeyType& highKey, vector<ValueType>& out) const;
	void collectKeys(AVLNode* current, vector<KeyType>& out) const;

	//copy/destroy helpers
	AVLNode* cloneSubtree(const AVLNode* otherRoot);
	void destroySubtree(AVLNode* current);
	//printing helper
	void printSideways(ostream& os, const AVLNode* current, size_t depth) const;
};

#endif //AVLTREE_H
