/*
 * BezierPatch.cpp
 *
 *  Created on: Dec 6, 2008
 *      Author: eddyc
 */

#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>
#include "Point.h"
#include "Vector.h"
#include "matrix.h"
#include "QuadTree.h"
#include "BezierPatch.h"
#include "boost/foreach.hpp"

using namespace std;
using namespace math;

BezierPatch::BezierPatch(matrix<Point> c) : QuadTree<matrix<Point> >(c) {
	assert(c.ColNo() == 4 && c.RowNo() == 4);
	initMatrices();
	_level = 1;
	_size = 16;
}

BezierPatch::~BezierPatch() {
	this->makeEmpty(root);
}

void BezierPatch::initMatrices() {
	SL.SetSize(4,4);
	SL.Null();
	SL(0,0) = 1.0;
	SL(1,0) = SL(1,1) = SL(2,1) = 0.5;
	SL(2,0) = SL(2,2) = 0.25;
	SL(3,0) = SL(3,3) = 1.0 / 8.0;
	SL(3,1) = SL(3,2) = 3.0 / 8.0;

	SR.SetSize(4,4);
	SR.Null();
	SR(0,0) = SR(0,3) = 1.0 / 8.0;
	SR(0,1) = SR(0,2) = SR(0,0) * 3.0;
	SR(1,1) = SR(1,3) = .25;
	SR(1,2) = SR(2,2) = SR(2,3) = 0.5;
	SR(3,3) = 1.0;
}

matrix<Point> BezierPatch::apply(matrix<double> S, matrix<Point> controls) {
	matrix<Point> newControls(4,4);
	newControls.Null();
	for (int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			for(int k = 0; k < 4; k++)
				newControls(i,j) = newControls(i,j) + S(i,k) * controls(k,j);
	return newControls;
}

matrix<Point> BezierPatch::apply(matrix<Point> controls, matrix<double> S) {
	matrix<Point> newControls(4,4);
	newControls.Null();
	for (int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			for(int k = 0; k < 4; k++)
				newControls(i,j) = newControls(i,j) + S(k,j) * controls(i,k);
	return newControls;
}

void BezierPatch::refine(int level) {
	for (int i = 0; i < level; i++)
		subdivide();
}

void BezierPatch::subdivide() {
	subdivide(root);
}

void BezierPatch::subdivide(QuadTreeNode<matrix<Point> > *t) {
	if (t) {
		if (t->n1) {
			subdivide(t->n1);
			subdivide(t->n2);
			subdivide(t->n3);
			subdivide(t->n4);
		} else {
			t->n1 = new QuadTreeNode<matrix<Point> >(apply(apply(SL, t->element), ~SL));
			t->n2 = new QuadTreeNode<matrix<Point> >(apply(apply(SR, t->element), ~SL));
			t->n3 = new QuadTreeNode<matrix<Point> >(apply(apply(SL, t->element), ~SR));
			t->n4 = new QuadTreeNode<matrix<Point> >(apply(apply(SR, t->element), ~SR));
		}
	}
}

int BezierPatch::size() {
	if (level(root) == _level)
		return _size;
	return (_size = size(root));
}

int BezierPatch::size(QuadTreeNode<matrix<Point> > *t) {
	if (t) {
		if (t->n1)
			return size(t->n1) + size(t->n2) + size(t->n3) + size(t->n4);
		else
			return 16;
	}
	return 0;
}

int BezierPatch::level() {
	return (_level = level(root));
}

int BezierPatch::level(QuadTreeNode<matrix<Point> > *t) {
	if (t) {
		if (t->n1) {
			return 1 + level(t->n1);
		} else
			return 1;
	}
	return 0;
}

matrix<Point> BezierPatch::flatten() {
	return flatten(root);
}

matrix<Point> BezierPatch::flatten(QuadTreeNode<matrix<Point> > *t) {
	if (t) {
		if (t->n1) {
			matrix<Point> m1 = flatten(t->n1);
			matrix<Point> m2 = flatten(t->n2);
			matrix<Point> m3 = flatten(t->n3);
			matrix<Point> m4 = flatten(t->n4);
			int rows = m1.RowNo();
			int cols = m1.ColNo();
			matrix<Point> m(rows*2-1, cols*2-1);
			for (unsigned int i = 0; i < m1.RowNo(); i++) {
				for (unsigned int j = 0; j < m1.ColNo(); j++) {
					m(i,j) = m1(i,j); // top left
					m(i+rows-1, j) = m2(i,j); // bottom left
					m(i, j+cols-1) = m3(i,j); // top right
					m(i+rows-1, j+cols-1) = m4(i,j); // bottom right
				}
			}
			return m;
		} else
			return t->element;
	}
	return 0;
}

void BezierPatch::print() {
	this->printTree();
}
