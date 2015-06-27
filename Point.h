#ifndef POINT_H
#define POINT_H

//
//  ============================================================
//
//                        Point.h
//
//  ============================================================
//
//
// 	Copyright (C) 1992,1993,1994,1995,1996
//
//			Professor Kenneth I. Joy
//			Computer Science Department
//			University of California
//			Davis, CA  95616
//
// 	Permission is granted to use at your own risk and
//	distribute this software in source and binary forms
//	provided the above copyright notice and this paragraph
//	are preserved on all copies.  This software is provided
//	"as is" with no express or implied warranty.
//
//
//  ============================================================
//


#include "Vector.h"
#include "Color256.h"

class	Point {

   protected :

      double		_x, _y, _z ;
      Color256		color;

   public :

			//  Constructors

      Point () ;

      Point ( const double,
	      const double = 0.0,
	      const double = 0.0,
	      const unsigned char = 255,
	      const unsigned char = 255,
	      const unsigned char = 255,
	      const unsigned char = 255 ) ;

      Point ( const double,
	      const double,
	      const double,
	      const Color256) ;

			//  Copy Constructor

      Point ( const Point& ) ;

			//  Destructor

      virtual ~Point () ;

			//  Assignment

      Point& operator= ( const Point& ) ;

			//  Output

      friend ostream& operator<< ( ostream&, const Point& ) ;

			//  Comparison

      friend int operator== ( const Point&, const Point& ) ;
      friend int operator!= ( const Point&, const Point& ) ;

			//  Arithmetic Operations

      friend Point operator+ ( const Point&, const Vector& ) ;
      friend Point operator+ ( const Vector&, const Point& ) ;
      friend Vector operator- ( const Point&, const Point& ) ;
      friend Point operator- ( const Point&, const Vector& ) ;
      friend Point operator- ( const Point& ) ;

      friend Point operator+ ( const Point&, const Point& );
      friend Point operator* ( const double, const Point& );
      friend Point operator* ( const Point&, const double );

      Point& operator+= ( const Vector& ) ;
      Point& operator-= ( const Vector& ) ;

			//  Affine Combinations

      friend Point affine ( const Point&,
			    const Point&,
			    const double ) ;

      friend Point affine ( const Point&,
			    const Point&,
			    const Point&,
			    const double,
			    const double,
			    const double ) ;

      friend Point average ( Point *, const int ) ;
      friend Point midpoint ( const Point&, const Point& ) ;

			//  Access Functions

      double x () const { return _x ; } ;
      double y () const { return _y ; } ;
      double z () const { return _z ; } ;
      unsigned char r () const { return color.red() ; } ;
      unsigned char g () const { return color.green() ; } ;
      unsigned char b () const { return color.blue() ; } ;
      unsigned char a () const { return color.alpha() ; } ;

      void setR(unsigned char r) { color.setRed(r); } ;
      void setG(unsigned char g) { color.setGreen(g); } ;
      void setB(unsigned char b) { color.setBlue(b); } ;
      void setA(unsigned char a) { color.setAlpha(a); } ;

   } ;

const Point PointZero( 0.0, 0.0, 0.0 ) ;
#endif



