/*
 * BezierCurve.cpp
 *
 *  Created on: Dec 5, 2008
 *      Author: eddyc
 */

#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>
#include "Point.h"
#include "Vector.h"
#include "matrix.h"
#include "BezierCurve.h"
#include <boost/foreach.hpp>

using namespace std;
using namespace math;

BezierCurve::BezierCurve(Point p1, Point p2, Point p3, Point p4) {
	this->controls.SetSize(4,4);
	controls(0,0) = p1;
	controls(1,0) = p2;
	controls(2,0) = p3;
	controls(3,0) = p4;
	curve.push_back(p1);
	curve.push_back(p2);
	curve.push_back(p3);
	curve.push_back(p4);
	initMatrices();
}

BezierCurve::BezierCurve(vector<Point> controls) : curve(controls) {
	assert(controls.size() == 4);
	this->controls(0,0) = controls[0];
	this->controls(1,0) = controls[1];
	this->controls(2,0) = controls[2];
	this->controls(3,0) = controls[3];
	initMatrices();
}

BezierCurve::BezierCurve(matrix<Point> c) : controls(c) {
	curve.push_back(c(0,0));
	curve.push_back(c(1,0));
	curve.push_back(c(2,0));
	curve.push_back(c(3,0));
	initMatrices();
}

BezierCurve::~BezierCurve() {

}

void BezierCurve::initMatrices() {
	Sk.SetSize(4,4);
	Sk(0,0) = 1.0;
	Sk(1,0) = Sk(1,1) = Sk(2,1) = 0.5;
	Sk(2,0) = Sk(2,2) = 0.25;
	Sk(3,0) = Sk(3,3) = 1.0 / 8.0;
	Sk(3,1) = Sk(3,2) = 3.0 / 8.0;

	Si.SetSize(4,4);
	Si(0,3) = Si(2,1) = 1.0;
	Si(1,2) = Si(3,0) = -1.0;
	Si(1,3) = 2.0;
	Si(2,2) = -4.0;
	Si(2,3) = 4.0;
	Si(3,1) = 6.0;
	Si(3,2) = -12.0;
	Si(3,3) = 8.0;
}

matrix<Point> BezierCurve::apply(matrix<double> S, matrix<Point> controls) {
	matrix<Point> newControls(4,1);
	newControls(0,0) = S(0,0) * controls(0,0) + S(0,1) * controls(1,0) +
						S(0,2) * controls(2,0) + S(0,3) * controls(3,0);
	newControls(1,0) = S(1,0) * controls(0,0) + S(1,1) * controls(1,0) +
						S(1,2) * controls(2,0) + S(1,3) * controls(3,0);
	newControls(2,0) = S(2,0) * controls(0,0) + S(2,1) * controls(1,0) +
						S(2,2) * controls(2,0) + S(2,3) * controls(3,0);
	newControls(3,0) = S(3,0) * controls(0,0) + S(3,1) * controls(1,0) +
						S(3,2) * controls(2,0) + S(3,3) * controls(3,0);
	return newControls;
}

void BezierCurve::refine(int level) {
	matrix<Point> subdivision = controls;
	curve.clear();

	// get the first subdivision by applying Sk level times
	for (int k = 0; k < level; k++)
		subdivision = apply(Sk, subdivision);
	curve.push_back(subdivision(0,0));
	curve.push_back(subdivision(1,0));
	curve.push_back(subdivision(2,0));
	curve.push_back(subdivision(3,0));

	// get subsequent subdivisions by applying Si 2^level - 1 times
	for (int i = 0; i < (int)pow(2.0,level) - 1; i++) {
		subdivision = apply(Si, subdivision);
		curve.push_back(subdivision(0,0));
		curve.push_back(subdivision(1,0));
		curve.push_back(subdivision(2,0));
		curve.push_back(subdivision(3,0));
	}
}

void BezierCurve::print() {
	BOOST_FOREACH(Point p, curve) {
		cout << p << endl;
	}
}
