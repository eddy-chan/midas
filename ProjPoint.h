#ifndef PROJPOINT_H
#define PROJPOINT_H

//
//  ============================================================
//
//                        ProjPoint.h
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


#include "Point.h"
#include "Color256.h"

class	ProjPoint {

   private :

			//  Implementation data, the x,y,z coordinates
			//	and the projective coordinate

      double		_x, _y, _z, _w ;
      Color256		color;

   public :

			//  Constructors

      ProjPoint () ;

      ProjPoint ( const double,
	          const double = 0.0,
	          const double = 0.0,
	          const double = 1.0,
		      const unsigned char = 255,
		      const unsigned char = 255,
		      const unsigned char = 255,
		      const unsigned char = 255 ) ;

      ProjPoint ( const Point& ) ;

			//  Copy Constructor

      ProjPoint ( const ProjPoint& ) ;

			//  Destructor

      virtual ~ProjPoint () ;

			//  Assignment

      ProjPoint& operator= ( const ProjPoint& ) ;
      ProjPoint& operator= ( const Point& ) ;

			//  Input/Output

      friend ostream& operator<< ( ostream&, const ProjPoint& ) ;

			//  Comparison

      friend int operator== ( const ProjPoint&, const ProjPoint& ) ;
      friend int operator!= ( const ProjPoint&, const ProjPoint& ) ;

			//  Arithmetic Operators

      friend ProjPoint operator- ( const ProjPoint& ) ;

			//  Affine Combinations

      friend ProjPoint affine ( const ProjPoint&,
			        const ProjPoint&,
			        const double ) ;

      friend ProjPoint affine ( const ProjPoint&,
			        const ProjPoint&,
			        const ProjPoint&,
			        const double,
			        const double,
			        const double ) ;

			//  Projection Function

      Point project() const ;

			//  Access Functions

      double x () const { return _x ; } ;
      double y () const { return _y ; } ;
      double z () const { return _z ; } ;
      double w () const { return _w ; } ;
      unsigned char r () const { return color.red() ; } ;
      unsigned char g () const { return color.green() ; } ;
      unsigned char b () const { return color.blue() ; } ;
      unsigned char a () const { return color.alpha() ; } ;

      void setR(unsigned char r) { color.setRed(r); } ;
      void setG(unsigned char g) { color.setGreen(g); } ;
      void setB(unsigned char b) { color.setBlue(b); } ;
      void setA(unsigned char a) { color.setAlpha(a); } ;

   } ;

const ProjPoint ProjPointZero ( 0.0, 0.0, 0.0, 1.0 ) ;
#endif



