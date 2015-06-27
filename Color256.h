#ifndef _COLOR256_H_
#define _COLOR256_H_

#include "Color.h"
#include <iostream>
using namespace std;

class Color256 : public Color {
	double _alpha;
	unsigned char red255;
	unsigned char green255;
	unsigned char blue255;
	unsigned char alpha255;
public:
	Color256();
	Color256(const unsigned char,
			const unsigned char,
			const unsigned char,
			const unsigned char = 255);
	Color256(const Color&);
	Color256(const Color256&);
	virtual ~Color256();

	//  Output
	friend ostream& operator<< ( ostream&, const Color256& ) ;

	unsigned char red() const { return red255; }
	unsigned char green() const { return green255; }
	unsigned char blue() const { return blue255; }
	unsigned char alpha() const { return alpha255; }
	void setAlpha(const unsigned char alpha) {
		_alpha = (double)alpha/255.0;
		alpha255 = alpha;
	}
	void setRed(const unsigned char red) {
		_red = (double)red/255.0 ;
		red255 = red;
	}
	void setGreen(const unsigned char green) {
		_green = (double)green/255.0;
		green255 = green;
	}
	void setBlue(const unsigned char blue) {
		_blue = (double)blue/255.0;
		blue255 = blue;
	}
};

#endif
