#ifndef COLOR_H
#define COLOR_H

#include <iostream>
using namespace std;

//
// ============================================================
//
//		Color
//
// ============================================================
//
//
//      Copyright (C) 1997
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


enum ColorModel { RGB,		// RGB will be the default
		  HSV,		// Hue, Saturation, Value
		  HLS 		// Hue, Lightness, Saturation
		  } ;

class Color {

   protected :

      double		_red,
			_green,
			_blue ;

      void hlstorgb ( const double, const double, const double ) ;

   public :

		//  Constructors

      Color () ;

		//  This constructor takes three color values and
		//    a color model (Defaulting to RGB).

      Color ( const double,
	      const double,
	      const double,
	      const ColorModel = RGB ) ;

		//  Copy Constructors

      Color ( const Color& ) ;

		//  Destructors

      virtual ~Color () ;

		//  Assignment

      Color& operator= ( const Color& ) ;

		//  Output

      friend ostream& operator<< ( ostream&, const Color& ) ;
      ostream& output ( ostream& ) const ;

		//  Comparison

      friend int operator== ( const Color&, const Color& ) ;
      friend int operator!= ( const Color&, const Color& ) ;

		//  Since this is an additive color model, we
		//    can do operations on the colors.

      friend Color operator+ ( const Color&, const Color& ) ;
      friend Color operator- ( const Color&, const Color& ) ;
      friend Color operator* ( const double, const Color& ) ;
      friend Color operator* ( const Color&, const double ) ;
      friend Color operator/ ( const Color&, const double ) ;

      Color& operator+= ( const Color& ) ;
      Color& operator-= ( const Color& ) ;
      Color& operator*= ( const double ) ;
      Color& operator/= ( const double ) ;

      friend Color affine ( const Color&,
			    const Color&,
			    const double&,
			    const double& ) ;

      friend Color affine ( const Color&,
			    const Color&,
			    const Color&,
			    const double&,
			    const double&,
			    const double& ) ;

		//  Change the lightness

      void darker_by ( const double ) ;
      void lighter_by ( const double ) ;

		//  Access Functions

      double hue() const ;
      double saturation() const ;
      double lightness() const ;
      double value() const { return lightness() ; } ;

      double red() const { return _red ; } ;
      double green() const { return _green ; } ;
      double blue() const { return _blue ; } ;

      void set_red ( const double r )
			{ _red = r ; } ;
      void set_green ( const double g )
			{ _green = g ; } ;
      void set_blue ( const double b )
			{ _blue = b ; } ;

   } ;

const Color White (  1.0,  1.0,  1.0 ) ;
const Color Black (  0.0,  0.0,  0.0 ) ;
const Color Red (  1.0,  0.0,  0.0 ) ;
const Color Green (  0.0,  1.0,  0.0 ) ;
const Color Blue (  0.0,  0.0,  1.0 ) ;
const Color Cyan (  0.0,  1.0,  1.0 ) ;
const Color Yellow (  1.0,  1.0,  0.0 ) ;
const Color Magenta (  1.0,  0.0,  1.0 ) ;
const Color Orange (  1.0, 0.65,  0.0 ) ;

const Color Gray10 ( 0.1, 0.1, 0.1 ) ;
const Color Gray20 ( 0.2, 0.2, 0.2 ) ;
const Color Gray30 ( 0.3, 0.3, 0.3 ) ;
const Color Gray40 ( 0.4, 0.4, 0.4 ) ;
const Color Gray50 ( 0.5, 0.5, 0.5 ) ;
const Color Gray60 ( 0.6, 0.6, 0.6 ) ;
const Color Gray70 ( 0.7, 0.7, 0.7 ) ;
const Color Gray80 ( 0.8, 0.8, 0.8 ) ;
const Color Gray90 ( 0.9, 0.9, 0.9 ) ;

#endif
