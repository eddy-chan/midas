//
// ============================================================
//
//		Color.C
//
// ============================================================
//

#include <cstdlib>
#include <cmath>
#include <strings.h>
#include <iostream>
#include "Color.h"
using namespace std;

#define        TRUE            1
#define        FALSE           0

//
// ============================================================
//
//		CONSTRUCTORS
//
// ============================================================
//

Color :: Color ()

   {
      _red = 0.0 ;
      _green = 0.0 ;
      _blue = 0.0 ;
   }

Color :: Color ( const double c1,
		 const double c2,
		 const double c3,
		 const ColorModel model )

   {
      switch ( model ) {
	 case RGB : {

	    _red = c1 ;
	    if ( _red < 0.0 ) _red = 0.0 ;
	    if ( _red > 1.0 ) _red = 1.0 ;

	    _green = c2 ;
	    if ( _green < 0.0 ) _green = 0.0 ;
	    if ( _green > 1.0 ) _green = 1.0 ;

	    _blue = c3 ;
	    if ( _blue < 0.0 ) _blue = 0.0 ;
	    if ( _blue > 1.0 ) _blue = 1.0 ;
	 }
	    break ;

	 case HLS : {

	    double _hue = c1 ;
	    while ( _hue < 0.0 ) _hue += 360.0 ;
	    while ( _hue > 360.0 ) _hue -= 360.0 ;

	    double _lightness = c2 ;
	    if ( _lightness < 0.0 ) _lightness = 0.0 ;
	    if ( _lightness > 1.0 ) _lightness = 1.0 ;

	    double _saturation = c3 ;
	    if ( _saturation < 0.0 ) _saturation = 0.0 ;
	    if ( _saturation > 1.0 ) _saturation = 1.0 ;

	    hlstorgb ( _hue, _lightness, _saturation ) ;
	 }
	    break ;

	 case HSV : {

	    double _hue1 = c1 ;
	    while ( _hue1 < 0.0 ) _hue1 += 360.0 ;
	    while ( _hue1 > 360.0 ) _hue1 -= 360.0 ;

	    double _lightness1 = c3 ;
	    if ( _lightness1 < 0.0 ) _lightness1 = 0.0 ;
	    if ( _lightness1 > 1.0 ) _lightness1 = 1.0 ;

	    double _saturation1 = c2 ;
	    if ( _saturation1 < 0.0 ) _saturation1 = 0.0 ;
	    if ( _saturation1 > 1.0 ) _saturation1 = 1.0 ;

	    hlstorgb ( _hue1, _lightness1, _saturation1 ) ;
	 }
	    break ;
	 }
   }


//
// ============================================================
//
//		COPY CONSTRUCTOR
//
// ============================================================
//

Color :: Color ( const Color& c )

   {
      _red = c._red ;
      _green = c._green ;
      _blue = c._blue ;
   }

Color :: ~Color () { }

//
// ============================================================
//
//		ASSIGNMENT
//
// ============================================================
//

Color& Color :: operator= ( const Color& c )

   {
      if ( this == &c ) return ( *this ) ;

      _red = c._red ;
      _green = c._green ;
      _blue = c._blue ;

      return ( *this ) ;
   }

//
// ============================================================
//
//		OPERATOR<<
//
// ============================================================
//

ostream& operator<< ( ostream& co, const Color& c )

   {
      return c.output ( co ) ;
   }

//
// ============================================================
//
//		OUTPUT
//
// ============================================================
//

ostream& Color :: output ( ostream& co ) const

   {
      co << "Color ( "
	 << _red << ", "
	 << _green << ", "
	 << _blue << " )" ;

      return ( co ) ;
   }

//
// ============================================================
//
//		DARKER_BY
//
// ============================================================
//

void Color :: darker_by ( const double factor )

   {
      if ( ( factor < 0.0 ) ||
	   ( factor > 1.0 ) ) {
	 cerr << "Darker_by factor of [" << factor
	      << "is out of range" << endl ;
	 exit ( 200 ) ;
	 }


      operator*= ( 1.0 - factor ) ;
   }

//
// ============================================================
//
//		LIGHTER_BY
//
// ============================================================
//

void Color :: lighter_by ( const double factor )

   {
      if ( ( factor < 0.0 ) || ( factor > 1.0 ) ) {
	 cerr << "Lighter_by factor of [" << factor
	      << "is out of range" << endl ;
	 exit ( 200 ) ;
	 }

      operator+= ( Color ( factor, factor, factor ) ) ;
   }

/*
  ============================================================

			CVALUE

  ============================================================
*/

static	double 	cvalue ( double c1, double c2, double h )

  {
      if ( h > 360.0 ) h -= 360.0 ;

      if ( h < 0.0 ) h += 360.0 ;

      if ( h < 60.0 )
	    {
	       return ( c1 + ( c2 - c1 ) * h / 60.0 )  ;
	    }
	 else if ( h < 180.0 )
	    {
	       return ( c2 ) ;
	    }
	 else if ( h < 240.0 )
	    {
	       return ( c1 + ( c2 - c1 ) * ( 240.0 - h ) / 60.00 ) ;
	    }
	 else {
	    return ( c1 ) ;
	    }

  }

/*
  ============================================================

			HLS_TO_RGB

  ============================================================
*/

void Color :: hlstorgb ( const double h, const double l, const double s )

  {
      double			m1, m2,
				cvalue ( double c1,
					double c2,
					double h ) ;


      if ( l <= 0.5 )
	    m2 = l * ( 1.0 + s ) ;
         else
	    m2 = l + s - l * s ;

      m1 = 2.0 * l - m2  ;

      if ( s == 0.0 )
	    {
		  // Achromatic Case

	    _red = l  ;
	    _green = l  ;
	    _blue = l  ;
	    }
	 else
	    {
		  // Chromatic Case

	    _red = cvalue ( m1, m2, h + 120.0 )  ;
	    _green = cvalue ( m1, m2, h )  ;
	    _blue = cvalue ( m1, m2, h - 120.0 )  ;
	    }

      if ( _red < 0.0 ) _red = 0.0 ;
      if ( _red > 1.0 ) _red = 1.0 ;
      if ( _green < 0.0 ) _green = 0.0 ;
      if ( _green > 1.0 ) _green = 1.0 ;
      if ( _blue < 0.0 ) _blue = 0.0 ;
      if ( _blue > 1.0 ) _blue = 1.0 ;
  }


/*
  ============================================================

			MIN3

  ============================================================
*/

static double 	min3 ( double x, double y, double z )

  {
      double 		min ;

      min = x ;
      if ( y < min ) min = y ;
      if ( z < min ) min = z ;

      return ( min ) ;
  }

/*
  ============================================================

			MAX3

  ============================================================
*/

static double 	max3 ( double x, double y, double z )

  {
      double 		max ;

      max = x ;
      if ( y > max ) max = y ;
      if ( z > max ) max = z ;

      return ( max ) ;
  }


//
// ============================================================
//
//	       	Hue
//
// ============================================================
//

double Color :: hue () const

  {
      double 		cmax, cmin ;
      double		h = 0.0, l, s ;

      cmax = max3 ( _red, _green, _blue ) ;
      cmin = min3 ( _red, _green, _blue ) ;

	  // Lightness Calculation

      l = ( cmax + cmin ) * 0.5 ;

	  // Saturation Calculation

      if ( cmax != cmin ) {

			 // The Chromatic Case

	    if ( l < 0.5 )
		   s = ( cmax - cmin ) / ( cmax + cmin ) ;
	       else
		   s = ( cmax - cmin ) / ( 2.0 - cmax - cmin ) ;

		      // Hue Calculation

	    double rc = ( cmax - _red ) / ( cmax - cmin ) ;
	    double gc = ( cmax - _green ) / ( cmax - cmin ) ;
	    double bc = ( cmax - _blue ) / ( cmax - cmin ) ;

	    if ( _red == cmax ) h = bc - gc ;
	    if ( _green == cmax ) h = 2.0 + rc - bc ;
	    if ( _blue == cmax ) h = 4.0 + gc - rc ;

	    h *= 60.0 ;
	    if ( h < 0.0 ) h += 360.0 ;
	    }

      return ( h ) ;
  }


//
// ============================================================
//
//	       	Lightness
//
// ============================================================
//

double Color :: lightness() const

  {
      double 		cmax, cmin ;

      cmax = max3 ( _red, _green, _blue ) ;
      cmin = min3 ( _red, _green, _blue ) ;

	  // Lightness Calculation

      return ( ( cmax + cmin ) * 0.5 ) ;
  }

//
// ============================================================
//
//	       	Saturation
//
// ============================================================
//

double Color :: saturation () const

  {
      double 		cmax, cmin ;
      double		s = 0.0 ;

      cmax = max3 ( _red, _green, _blue ) ;
      cmin = min3 ( _red, _green, _blue ) ;

	  // Lightness Calculation

      double lightness = ( cmax + cmin ) * 0.5 ;

	  // Saturation Calculation

      if ( cmax != cmin ) {

			 // The Chromatic Case

	    if ( lightness < 0.5 )
		   s = ( cmax - cmin ) / ( cmax + cmin ) ;
	       else
		   s = ( cmax - cmin ) / ( 2.0 - cmax - cmin ) ;
	    }

      return ( s ) ;
  }


/*
   ==========================================================

	       COMPARISON OPERATIONS

   ==========================================================
*/

int operator== ( const Color& c1, const Color& c2 )

   {
      if ( ( c1._red == c2._red ) &&
           ( c1._green == c2._green ) &&
           ( c1._blue == c2._blue ) )
	    return ( TRUE ) ;
	 else
	    return ( FALSE ) ;
   }

int operator!= ( const Color& c1, const Color& c2 )

   {
      if ( ( c1._red != c2._red ) ||
           ( c1._green != c2._green ) ||
           ( c1._blue != c2._blue ) )
	    return ( TRUE ) ;
	 else
	    return ( FALSE ) ;
   }

/*
   ==========================================================

	       ARITHMETIC OPERATIONS

   ==========================================================
*/

Color operator+ ( const Color& c1, const Color& c2 )

   {
      Color cc ;

      cc._red = c1._red + c2._red ;
      cc._green = c1._green + c2._green ;
      cc._blue = c1._blue + c2._blue ;

      return ( cc ) ;
   }

Color operator- ( const Color& c1, const Color& c2 )

   {
      Color cc ;

      cc._red = c1._red - c2._red ;
      cc._green = c1._green - c2._green ;
      cc._blue = c1._blue - c2._blue ;

      return ( cc ) ;
   }

Color operator* ( const double c, const Color& v )

   {
      Color cc ;

      cc._red = c * v._red ;
      cc._green = c * v._green ;
      cc._blue = c * v._blue ;

      return ( cc ) ;
   }

Color operator* ( const Color& v, const double c )

   {
      Color cc ;

      cc._red = c * v._red ;
      cc._green = c * v._green ;
      cc._blue = c * v._blue ;

      return ( cc ) ;
   }

Color operator/ ( const Color& v, const double c )

   {
      Color cc ;

      cc._red = v._red / c ;
      cc._green = v._green / c ;
      cc._blue = v._blue / c ;

      return ( cc ) ;
   }

Color& Color :: operator+= ( const Color& c )

   {
      _red += c._red ;
      _green += c._green ;
      _blue += c._blue ;

      return *this ;
   }

Color& Color :: operator-= ( const Color& c )

   {
      _red -= c._red ;
      _green -= c._green ;
      _blue -= c._blue ;

      return *this ;
   }

Color& Color :: operator*= ( const double c )

   {
      _red *= c ;
      _green *= c ;
      _blue *= c ;

      return *this ;
   }

Color& Color :: operator/= ( const double c )

   {
      _red /= c ;
      _green /= c ;
      _blue /= c ;

      return *this ;
   }

/*
   ==========================================================

	       AFFINE

   ==========================================================
*/

Color affine ( const Color& c1,
	       const Color& c2,
	       const double& a1,
	       const double& a2 )

   {
      return ( a1 * c1 + a2 * c2 ) ;
   }

Color affine ( const Color& c1,
	       const Color& c2,
	       const Color& c3,
	       const double& a1,
	       const double& a2,
	       const double& a3 )

   {
      return ( a1 * c1 + a2 * c2 + a3 * c3 ) ;
   }
