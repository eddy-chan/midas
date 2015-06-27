//============================================================================
// Name        : QuadTree.cpp
// Author      : Eddy Chan
// Version     :
// Copyright   : blah blah blah no copying without permission
// Description : Hello World in C, Ansi-style
//============================================================================
#include <iostream>
#include "QuadTree.h"
using namespace std;

template <class T>
QuadTree<T>::QuadTree(QuadTreeNode<T> *n) {
	root = n;
}

template <class T>
QuadTree<T>::QuadTree(T& el) {
	root = new QuadTreeNode<T>(el);
}

template <class T>
QuadTree<T>::~QuadTree() {
	makeEmpty(root);
}

template <class T>
void QuadTree<T>::makeEmpty(QuadTreeNode<T> *n) const {
	if (n) {
		makeEmpty(n->n1);
		makeEmpty(n->n2);
		makeEmpty(n->n3);
		makeEmpty(n->n4);
		delete n;
	}
	n = NULL;
}

template <class T>
void QuadTree<T>::printTree() const {
	printTree(root);
}

template <class T>
void QuadTree<T>::printTree(QuadTreeNode<T> *n) const {
	if (n) {
		printTree(n->n1);
		printTree(n->n2);
		printTree(n->n3);
		printTree(n->n4);
		cout << n->element << endl;
	}
}
