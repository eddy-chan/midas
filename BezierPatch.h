/*
 * BezierPatch.h
 *
 *  Created on: Dec 6, 2008
 *      Author: eddyc
 */

#ifndef BEZIERPATCH_H_
#define BEZIERPATCH_H_

#include "matrix.h"
#include "Point.h"
#include "QuadTree.h"
#include <map>
using namespace std;
using namespace math;

//typedef map<Point, matrix<Point> > texMap;

class BezierPatch : public QuadTree<matrix<Point> > {
	int _level;
	int _size;

	void initMatrices();
	void subdivide(QuadTreeNode<matrix<Point> > *t);
	int size(QuadTreeNode<matrix<Point> > *t);
	int level(QuadTreeNode<matrix<Point> > *t);
	matrix<Point> flatten(QuadTreeNode<matrix<Point> > *t);
public:
	matrix<double> SL;
	matrix<double> SR;

	explicit BezierPatch(matrix<Point>);
	~BezierPatch();

	matrix<Point> apply(matrix<double> S, matrix<Point> controls);
	matrix<Point> apply(matrix<Point> controls, matrix<double> S);
	void refine(int level = 0);
	void subdivide();
	int size();
	int level();
	matrix<Point> flatten();
//	texMap texturize();
	void print();
};

#endif /* BEZIERPATCH_H_ */
