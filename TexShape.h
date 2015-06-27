
#ifndef TEXSHAPE_H_
#define TEXSHAPE_H_

#include <iostream>
#include <string>
#include <vector>
#include "Vector.h"
#include "Point.h"
using namespace std;

typedef vector<vector<Point> > polygonList;
typedef polygonList texList;

class TexShape {
public:
	string texturefile;
	polygonList shape;
	texList texture;

	TexShape();
	TexShape(polygonList shape, texList texture, string texturefile);
	~TexShape();
	void print(ostream& of);
};

#endif /* TEXSHAPE_H_ */
