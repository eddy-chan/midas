/*
 * BezierCurve.h
 *
 *  Created on: Dec 5, 2008
 *      Author: eddyc
 */

#ifndef BEZIERCURVE_H_
#define BEZIERCURVE_H_

#include "matrix.h"
#include "Point.h"
#include <vector>
using namespace std;
using namespace math;

class BezierCurve {
	void initMatrices();
public:
	matrix<double> Sk;
	matrix<double> Si;
	matrix<Point> controls;
	vector<Point> curve;

	BezierCurve(Point, Point, Point, Point);
	BezierCurve(matrix<Point>);
	BezierCurve(vector<Point>);
	~BezierCurve();

	matrix<Point> apply(matrix<double> S, matrix<Point> controls);
	void refine(int level = 0);
	void print();
};

#endif /* BEZIERCURVE_H_ */
