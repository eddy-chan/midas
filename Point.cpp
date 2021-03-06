//
// ============================================================
//
//                      POINT.C
//
// ============================================================
//
//
//      Copyright (C) 1992,1993,1994,1995,1996
//
//                      Professor Kenneth I. Joy
//                      Computer Science Department
//                      University of California
//                      Davis, CA  95616
//
//      Permission is granted to use at your own risk and
//      distribute this software in source and binary forms
//      provided the above copyright notice and this paragraph
//      are preserved on all copies.  This software is provided
//      "as is" with no express or implied warranty.
//
//
//  ============================================================
//

#include <cstdlib>
#include <cmath>
#include <strings.h>
#include <iostream>
#include <boost/format.hpp>
#include <iomanip>
#include "Point.h"
using namespace std;

#define		TRUE		1
#define		FALSE		0


/*
   ==========================================================

	       CONSTRUCTORS, DESTRUCTOR

   ==========================================================
*/

Point :: Point ()

   {
      _x = 0.0 ;
      _y = 0.0 ;
      _z = 0.0 ;
      color.setRed(255);
      color.setGreen(255);
      color.setBlue(255);
      color.setAlpha(255);
   }

Point :: Point ( const double x, const double y, const double z, const unsigned char r,
		const unsigned char g, const unsigned char b, const unsigned char a )

   {
      _x = x ;
      _y = y ;
      _z = z ;
      color.setRed(r);
      color.setGreen(g);
      color.setBlue(b);
      color.setAlpha(a);
   }

Point :: Point ( const double x, const double y, const double z, const Color256 c )

   {
      _x = x ;
      _y = y ;
      _z = z ;
      color = c;
   }

Point :: Point ( const Point& p )

   {
      _x = p._x ;
      _y = p._y ;
      _z = p._z ;
      color.setRed(p.r());
      color.setGreen(p.g());
      color.setBlue(p.b());
      color.setAlpha(p.a());
   }

Point :: ~Point () { }


/*
   ==========================================================

	       OPERATOR=

   ==========================================================
*/

Point& Point :: operator= ( const Point& p )

   {
      if ( this == &p ) return ( *this ) ;

      _x = p._x ;
      _y = p._y ;
      _z = p._z ;
      color.setRed(p.r());
      color.setGreen(p.g());
      color.setBlue(p.b());
      color.setAlpha(p.a());

      return ( *this ) ;
   }

/*
   ==========================================================

	       OUTPUT

   ==========================================================
*/

ostream& operator<< ( ostream& co, const Point& p )

   {
	co << "(" << setprecision(4)
	<< p._x
	<< ","
	<< p._y
	<< ","
	<< p._z
	<< ",";
	co << p.color << ")";
//	<< ")";

      return co ;
   }

/*
   ==========================================================

	       COMPARISON OPERATIONS

   ==========================================================
*/

int operator== ( const Point& p1, const Point& p2 )

   {
      if ( ( p1._x == p2._x ) &&
           ( p1._y == p2._y ) &&
           ( p1._z == p2._z ) )
	    return ( TRUE ) ;
	 else
	    return ( FALSE ) ;
   }

int operator!= ( const Point& p1, const Point& p2 )

   {
      if ( ( p1._x != p2._x ) ||
           ( p1._y != p2._y ) ||
           ( p1._z != p2._z ) )
	    return ( TRUE ) ;
	 else
	    return ( FALSE ) ;
   }

/*
   ==========================================================

	       ARITHMETIC OPERATIONS

   ==========================================================
*/

Point operator+ ( const Point& p1, const Vector& p2 )

   {
      Point pp(p1);

      pp._x = p1._x + p2.x() ;
      pp._y = p1._y + p2.y() ;
      pp._z = p1._z + p2.z() ;

      return ( pp ) ;
   }

Point operator+ ( const Vector& p2, const Point& p1 )

   {
      Point pp(p1);

      pp._x = p1._x + p2.x() ;
      pp._y = p1._y + p2.y() ;
      pp._z = p1._z + p2.z() ;

      return ( pp ) ;
   }

Vector operator- ( const Point& p1, const Point& p2 )

   {
      return ( Vector ( p1._x - p2._x, p1._y - p2._y, p1._z - p2._z ) ) ;
   }

Point operator- ( const Point& p, const Vector& v )

   {
      return ( Point ( p._x - v.x(), p._y - v.y(), p._z - v.z() ) ) ;
   }

Point operator- ( const Point& p )

   {
      Point pp(p) ;

      pp._x = -p._x ;
      pp._y = -p._y ;
      pp._z = -p._z ;

      return ( pp ) ;
   }

Point& Point :: operator+= ( const Vector& v )

   {
      _x += v.x() ;
      _y += v.y() ;
      _z += v.z() ;

      return ( *this ) ;
   }

Point& Point :: operator-= ( const Vector& v )

   {
      _x -= v.x() ;
      _y -= v.y() ;
      _z -= v.z() ;

      return ( *this ) ;
   }

Point operator+ ( const Point& p1, const Point& p2 ) {
	return Point(p1._x + p2._x, p1._y + p2._y, p1._z + p2._z, Color256(affine(p1.color, p2.color, 0.5, 0.5)));
}
Point operator* ( const double s, const Point& p ) {
	return Point(s * p._x, s * p._y, s* p._z, p.color);
}
Point operator* ( const Point& p, const double s ) {
	return Point(s * p._x, s * p._y, s* p._z, p.color);
}

/*
   ==========================================================

	       AFFINE

   ==========================================================
*/

Point affine ( const Point& p1, const Point& p2, const double t )

   {
      Point p ;

      p._x = p1._x + t * ( p2._x - p1._x ) ;
      p._y = p1._y + t * ( p2._y - p1._y ) ;
      p._z = p1._z + t * ( p2._z - p1._z ) ;

      return ( p ) ;
   }

Point affine ( const Point& p1,
	       const Point& p2,
	       const Point& p3,
	       const double a1,
	       const double a2,
	       const double a3 )

   {
      Point p ;

      p._x = a1 * p1._x + a2 * p2._x + a3 * p3._x ;
      p._y = a1 * p1._y + a2 * p2._y + a3 * p3._y ;
      p._z = a1 * p1._z + a2 * p2._z + a3 * p3._z ;

      return ( p ) ;
   }

Point average ( Point *p, const int n )

   {
      double xsum = 0.0 ;
      double ysum = 0.0 ;
      double zsum = 0.0 ;

      double weight = 1.0 / ( (double) n ) ;

      for ( int i = 0 ; i < n ; i++ ) {
	 xsum += weight * p[i].x() ;
	 ysum += weight * p[i].y() ;
	 zsum += weight * p[i].z() ;
	 }

      return ( Point ( xsum, ysum, zsum ) ) ;
   }

Point midpoint ( const Point& p1, const Point& p2 )

   {
      return ( affine ( p1, p2, 0.5 ) ) ;
   }

