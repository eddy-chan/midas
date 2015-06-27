/* Very simple quad tree structure
 * nodes are public so we can do whatever wih it
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <cstdlib>

template <class T>
class QuadTree ;

template <class T>
class QuadTreeNode {
public:
	T element;
	QuadTreeNode *n1;
	QuadTreeNode *n2;
	QuadTreeNode *n3;
	QuadTreeNode *n4;
	QuadTreeNode(const T &el, QuadTreeNode *node1 = NULL, QuadTreeNode *node2 = NULL, QuadTreeNode *node3 = NULL, QuadTreeNode *node4 = NULL)
		: element(el), n1(node1), n2(node2), n3(node3), n4(node4) {}
	friend class QuadTree<T>;
};

template <class T>
class QuadTree {
public:
	QuadTreeNode<T> *root;

	QuadTree(QuadTreeNode<T> *n = NULL);
	QuadTree(T& el);
	~QuadTree();
	void printTree() const;
	void printTree(QuadTreeNode<T> *n) const;
	QuadTreeNode<T>* getRoot() { return root; }
	void makeEmpty(QuadTreeNode<T> *n) const;
};

#include "QuadTree.cpp"
#endif /*QUADTREE_H_*/
