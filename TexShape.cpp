/*
 * TexShape.cpp
 *
 *  Created on: Dec 10, 2008
 *      Author: eddyc
 */
#include <vector>
#include "TexShape.h"
#include "boost/foreach.hpp"

TexShape::TexShape() {

}

TexShape::TexShape(polygonList shape, texList texture, string texturefile) {
	this->shape = shape;
	this->texture = texture;
	this->texturefile = texturefile;
}

TexShape::~TexShape() {

}

void TexShape::print(ostream& of) {
	of << "t " << texturefile << " " << shape.size() << endl;
	for (int i = 0; i < shape.size(); i++) {
		if (shape[i].size() < 1)
			continue;
		BOOST_FOREACH(Point p, shape[i]) {
			of << p << " ";
		}
		of << endl;
		BOOST_FOREACH(Point p, texture[i]) {
			of << p << " ";
		}
		of << endl;
	}
}
