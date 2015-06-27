#include <iostream>
#include <cstdlib>
#include "Color256.h"
#include <boost/format.hpp>

using namespace std;

Color256::Color256() : Color() {
	_alpha = 1.0;
}

Color256::Color256(const unsigned char r,
		const unsigned char g, const unsigned char b, const unsigned char a)
: Color((double)r/255.0, (double)g/255.0, (double)b/255.0)
{
//	_red = (double)r/255.0;
//	_green = (double)g/255.0;
//	_blue = (double)b/255.0;
	_alpha = (double)a/255.0;
	red255 = r;
	green255 = g;
	blue255 = b;
	alpha255 = a;
}

Color256::Color256(const Color& c) : Color(c) {
	setRed(c.red() * 255.0);
	setGreen(c.green() * 255.0);
	setBlue(c.blue() * 255.0);
	_alpha = 1.0;
	alpha255 = 255;
}

Color256::Color256(const Color256& c) : Color(c) {
	setRed(c.red());
	setGreen(c.green());
	setBlue(c.blue());
	setAlpha(c.alpha());
}

Color256::~Color256() { }

ostream& operator<< ( ostream& co, const Color256& c ) {
	unsigned short r, g, b;
//	cerr << c._red << " " << c._green << " " << c._blue << " " << c._alpha << endl;
	r = c.red();
	g = c.green();
	b = c.blue();
	//a = c.alpha();
    // formatting - %[ N$ ][ flags ][ width ][ . precision ]type-char
	co << boost::format("%1$02x%2$02x%3$02x") % r % g % b;

	return co;
}
