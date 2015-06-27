//
//  ============================================================
//
//		Transformation
//
//  ============================================================
//
//
// 	Copyright (C) 1997
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

#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <iostream>
#include "TransformationWithFrames.h"

#define		TRUE		1
#define		FALSE		0

const double         pi = 3.1415926535897 ;

/*
   ============================================================

                CONSTRUCTOR

   ============================================================
*/

Transformation :: Transformation ()

   {
      id() ;
   }

/*
   ============================================================

                COPY CONSTRUCTOR

   ============================================================
*/

Transformation :: Transformation ( const Transformation& t )

   {
      for ( int i = 0 ; i < 4 ; i++ )
	 for ( int j = 0 ; j < 4 ; j++ )
	    _matrix[i][j] = t._matrix[i][j] ;
   }

/*
   ============================================================

                DESTRUCTOR

   ============================================================
*/

Transformation :: ~Transformation ()  { }

/*
   ============================================================

                ASSIGNMENT

   ============================================================
*/

Transformation& Transformation :: operator= ( const Transformation& t )

   {
      if ( this == &t ) return ( *this ) ;

      for ( int i = 0 ; i < 4 ; i++ )
	 for ( int j = 0 ; j < 4 ; j++ )
	    _matrix[i][j] = t._matrix[i][j] ;

      return ( *this ) ;
   }

/*
   ============================================================

                COMPARISONS

   ============================================================
*/

int  operator== ( const Transformation& t1, const Transformation& t2 )

   {
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ ) {
	 for ( j = 0 ; j < 4 ; j++ ) {
	    if ( t1._matrix[i][j] != t2._matrix[i][j] ) return ( FALSE ) ;
	    }
	 }

      return ( TRUE ) ;
   }

int  operator!= ( const Transformation& t1, const Transformation& t2 )

   {
      return ( ! ( t1 == t2 ) ) ;
   }


/*
   ============================================================

                OUTPUT

   ============================================================
*/

ostream& operator<< ( ostream& co, const Transformation& t )

   {
      return t.output ( co ) ;
   }

/*
   ============================================================

                OUTPUT

   ============================================================
*/

ostream& Transformation :: output ( ostream& co, const int indent ) const

   {
      for ( int k = 0 ; k < indent ; k++ )
	 co << " " ;

      co.setf ( ios :: showpoint ) ;
      co.setf ( ios :: fixed ) ;
      co.precision ( 6 ) ;

      co << "Transformation ( " ;
      for ( int j = 0 ; j < 4 ; j++ )
	 co << setw ( 10 ) << _matrix[0][j] ;

      for ( int i = 1 ; i < 4 ; i++ ) {

	 co << endl ;
	 for ( int k = 0 ; k < indent ; k++ )
	    co << " " ;
	 co << "                 " ;

	 for ( int j = 0 ; j < 4 ; j++ ) {
	    co << setw ( 10 ) << _matrix[i][j] ;
	    }
	 }

      co << " )" ;

      return ( co ) ;
   }

/*
   ============================================================

                matrix

   ============================================================
*/

double Transformation :: matrix ( const int i, const int j ) const

   {
      if ( ( i < 0 ) || ( i > 3 ) ||
           ( j < 0 ) || ( j > 3 ) ) {
	 cerr << "Illegal subscripts ("
	      << i << ", " << j << ") to operator() in the "
	      << "Transformation class" << endl ;
	 exit ( 200 ) ;
	 }

      return ( _matrix[i][j] ) ;
   }

/*
   ============================================================

                ARITHMETIC FUNCTIONS

   ============================================================
*/

Transformation  operator+ ( const Transformation& t1,
			      const Transformation& t2 )

   {
      Transformation		tt ;
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    tt._matrix[i][j] = t1._matrix[i][j] + t2._matrix[i][j] ;

      return ( tt ) ;
   }

Transformation  operator- ( const Transformation& t1,
			      const Transformation& t2 )

   {
      Transformation		tt ;
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    tt._matrix[i][j] = t1._matrix[i][j] - t2._matrix[i][j] ;

      return ( tt ) ;
   }

Transformation  operator- ( const Transformation& t )

   {
      Transformation		tt ;
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    tt._matrix[i][j] = -t._matrix[i][j] ;

      return ( tt ) ;
   }

Transformation  operator* ( const double c, const Transformation& t )

   {
      Transformation		tt ;
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    tt._matrix[i][j] = c * t._matrix[i][j] ;

      return ( tt ) ;
   }

Transformation  operator* ( const Transformation& t, const double c )

   {
      Transformation		tt ;
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    tt._matrix[i][j] = c * t._matrix[i][j] ;

      return ( tt ) ;
   }


Transformation  operator* ( const Transformation& t1,
			      const Transformation& t2 )

   {
      Transformation		tt ;
      int 			i, j, k ;
      double			sum ;

      for ( i = 0 ; i < 4 ; i++ ) {
	 for ( j = 0 ; j < 4 ; j++ ) {

	    sum = 0.0 ;
	    for ( k = 0 ; k < 4 ; k++ ) {
	       sum += t1._matrix[i][k] * t2._matrix[k][j] ;
	       }

	    tt._matrix[i][j] = sum ;

	    }
	 }

      return ( tt ) ;
   }

Transformation  Transformation :: operator+= ( const Transformation& t )

   {
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    _matrix[i][j] += t._matrix[i][j] ;

      return ( *this ) ;
   }

Transformation  Transformation :: operator-= ( const Transformation& t )

   {
      int 		i, j ;

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    _matrix[i][j] -= t._matrix[i][j] ;

      return ( *this ) ;
   }

Transformation  Transformation :: operator*= ( const Transformation& t )

   {
      double		tt[4][4] ;
      int 		i, j, k ;
      double		sum ;

      for ( i = 0 ; i < 4 ; i++ ) {
	 for ( j = 0 ; j < 4 ; j++ ) {

	    sum = 0.0 ;
	    for ( k = 0 ; k < 4 ; k++ ) {
	       sum += _matrix[i][k] * t._matrix[k][j] ;
	       }

	    tt[i][j] = sum ;

	    }
	 }

      for ( i = 0 ; i < 4 ; i++ )
	 for ( j = 0 ; j < 4 ; j++ )
	    _matrix[i][j] = tt[i][j] ;

      return ( *this ) ;
   }

/*
   ============================================================

                IDENTITY Transformation

   ============================================================
*/

Transformation identity ()

   {
      Transformation t ;
      return ( t ) ;
   }

/*
   ============================================================

                TRANSLATE_BY

   ============================================================
*/

Transformation Transformation::translate_by ( const Vector v )

   {
      Transformation t ;
      t._matrix[3][0] = v.x() ;
      t._matrix[3][1] = v.y() ;
      t._matrix[3][2] = v.z() ;

      return ( t ) ;
   }

Transformation Transformation::translate_by (  const double x,
				 const double y,
				 const double z )

   {
      return ( translate_by ( Vector ( x, y, z ) ) ) ;
   }

/*
   ============================================================

                SCALE_BY

   ============================================================
*/

Transformation Transformation::scale_by ( const Vector v )

   {
      Transformation t ;
      t._matrix[0][0] = v.x() ;
      t._matrix[1][1] = v.y() ;
      t._matrix[2][2] = v.z() ;

      return ( t ) ;
   }

Transformation Transformation::scale_by (  const double x,
			     const double y,
			     const double z )

   {
      return ( scale_by ( Vector ( x, y, z ) ) ) ;
   }

/*
   ============================================================

                X_ROTATE_BY

   ============================================================
*/

Transformation Transformation::x_rotate_by ( const double parameter )

   {
      Transformation tt ;

      double cp = cos ( parameter * pi / 180.0 ) ;
      double sp = sin ( parameter * pi / 180.0 ) ;

      tt._matrix[1][1] = cp ;
      tt._matrix[1][2] = sp ;
      tt._matrix[2][1] = -sp ;
      tt._matrix[2][2] = cp ;

      return ( tt ) ;
   }

/*
   ============================================================

                Y_ROTATE_BY

   ============================================================
*/

Transformation Transformation::y_rotate_by ( const double parameter )

   {
      Transformation tt ;

      double cp = cos ( parameter * pi / 180.0 ) ;
      double sp = sin ( parameter * pi / 180.0 ) ;

      tt._matrix[0][0] = cp ;
      tt._matrix[0][2] = -sp ;
      tt._matrix[2][0] = sp ;
      tt._matrix[2][2] = cp ;

      return ( tt ) ;
   }

/*
   ============================================================

                Z_ROTATE_BY

   ============================================================
*/

Transformation Transformation::z_rotate_by ( const double parameter )

   {
      Transformation tt ;

      double cp = cos ( parameter * pi / 180.0 ) ;
      double sp = sin ( parameter * pi / 180.0 ) ;

      tt._matrix[0][0] = cp ;
      tt._matrix[0][1] = sp ;
      tt._matrix[1][0] = -sp ;
      tt._matrix[1][1] = cp ;

      return ( tt ) ;
   }

/*
   ============================================================

                ROTATE_BY

   ============================================================
*/

Transformation Transformation::rotate_by ( const double parameter, const Vector& axis )

   {
      double length1 = sqrt ( axis.x() * axis.x() + axis.z() * axis.z() ) ;
      double theta = atan2 ( axis.x(), axis.z() ) * 180.0 / pi ;
      double phi = atan2 ( axis.y(), length1 ) * 180.0 / pi ;

      return ( y_rotate_by ( -theta )
	     * x_rotate_by ( phi )
             * z_rotate_by ( parameter )
             * x_rotate_by ( -phi )
             * y_rotate_by ( theta ) ) ;
   }

/*
   ============================================================

                ID

   ============================================================
*/

void Transformation :: id ()

   {
      for ( int i = 0 ; i < 4 ; i++ ) {
	 for ( int j = 0 ; j < 4 ; j++ ) {
	    _matrix[i][j] = 0.0 ;
	    }
	 _matrix[i][i] = 1.0 ;
	 }
   }

/*
   ============================================================

                FRAMETOFRAME

   ============================================================
*/

Transformation Transformation::frame_to_frame ( const Frame& f1, const Frame& f2 )

			//  First frame is "from" and second
			//   is the destination frame

   {
      Transformation t1 ;

      t1._matrix[0][0] = f1.u().x() ;
      t1._matrix[0][1] = f1.u().y() ;
      t1._matrix[0][2] = f1.u().z() ;

      t1._matrix[1][0] = f1.v().x() ;
      t1._matrix[1][1] = f1.v().y() ;
      t1._matrix[1][2] = f1.v().z() ;

      t1._matrix[2][0] = f1.w().x() ;
      t1._matrix[2][1] = f1.w().y() ;
      t1._matrix[2][2] = f1.w().z() ;

      t1._matrix[3][0] = f1.origin().x() ;
      t1._matrix[3][1] = f1.origin().y() ;
      t1._matrix[3][2] = f1.origin().z() ;

      Transformation t2 ;
      Vector t ;

      double d = dot ( f2.u(), cross ( f2.v(), f2.w() ) ) ;

      t = Vector ( 1.0, 0.0, 0.0 ) ;

      t2._matrix[0][0] = dot ( t, cross ( f2.v(), f2.w() ) ) / d ;
      t2._matrix[0][1] = dot ( f2.u(), cross ( t, f2.w() ) ) / d ;
      t2._matrix[0][2] = dot ( f2.u(), cross ( f2.v(), t ) ) / d ;

      t = Vector ( 0.0, 1.0, 0.0 ) ;

      t2._matrix[1][0] = dot ( t, cross ( f2.v(), f2.w() ) ) / d ;
      t2._matrix[1][1] = dot ( f2.u(), cross ( t, f2.w() ) ) / d ;
      t2._matrix[1][2] = dot ( f2.u(), cross ( f2.v(), t ) ) / d ;

      t = Vector ( 0.0, 0.0, 1.0 ) ;

      t2._matrix[2][0] = dot ( t, cross ( f2.v(), f2.w() ) ) / d ;
      t2._matrix[2][1] = dot ( f2.u(), cross ( t, f2.w() ) ) / d ;
      t2._matrix[2][2] = dot ( f2.u(), cross ( f2.v(), t ) ) / d ;

      t = Point ( 0.0, 0.0, 0.0 ) - f2.origin() ;

      t2._matrix[3][0] = dot ( t, cross ( f2.v(), f2.w() ) ) / d ;
      t2._matrix[3][1] = dot ( f2.u(), cross ( t, f2.w() ) ) / d ;
      t2._matrix[3][2] = dot ( f2.u(), cross ( f2.v(), t ) ) / d ;

      Transformation tt = t1 * t2 ;

      return ( tt ) ;
   }

/*
   ============================================================

                VIEW

   ============================================================
*/

Transformation Transformation::view ( const double near,
			const double far,
			const double angle )

   {
      if ( ( angle > 0.0 ) && ( near < far ) ) {
	 double tan_angle = tan ( angle * 0.5 * pi / 180.0 ) ;
	 double cot_angle = 1.0 / tan_angle ;

	 Transformation tt ;
	 tt._matrix[0][0] = cot_angle ;
	 tt._matrix[1][1] = cot_angle ;
	 tt._matrix[2][2] = ( far + near ) / ( far - near ) ;
	 tt._matrix[3][2] = 2.0 * near * far / ( far - near ) ;
	 tt._matrix[2][3] = -1.0 ;
	 tt._matrix[3][3] = 0.0 ;
	 return ( tt ) ;
	 }

      return ( identity() ) ;
   }

/*
   ============================================================

                INVERSE_VIEW

   ============================================================
*/

Transformation inverse_view ( const double near,
				const double far,
				const double angle )

   {
      if ( ( angle > 0.0 ) && ( near < far ) ) {
	 double tan_angle = tan ( angle * 0.5 * pi / 180.0 ) ;

	 Transformation tt ;
	 tt._matrix[0][0] = tan_angle ;
	 tt._matrix[1][1] = tan_angle ;
	 tt._matrix[2][2] = 0.0 ;
	 tt._matrix[3][2] = -1.0 ;
	 tt._matrix[2][3] = ( far - near ) / ( 2.0 * near * far) ;
	 tt._matrix[3][3] = ( far + near ) / ( 2.0 * near * far) ;

	 return ( tt ) ;
	 }

      return ( identity() ) ;
   }

/*
   ============================================================

                APPLIED_TO

   ============================================================
*/

Vector Transformation :: applied_to ( const Vector& v ) const

   {
      double            sum, a[4], aa[4] ;

      a[0] = v.x() ;
      a[1] = v.y() ;
      a[2] = v.z() ;
      a[3] = 0.0 ;

      for ( int i = 0 ; i < 4 ; i++ ) {
         sum = 0.0 ;
         for ( int j = 0 ; j < 4 ; j++ ) {
            sum += a[j] * _matrix[j][i] ;
            }
         aa[i] = sum ;
         }

      return ( Vector ( aa[0], aa[1], aa[2] ) ) ;
   }


ListOfVectors Transformation :: applied_to ( const ListOfVectors& v ) const

   {
      ListOfVectors l ;
      int n = v.length() ;

      for ( int i = 0 ; i < n ; i++ )
	 l.add ( applied_to ( v[i] ) ) ;

      return ( l ) ;
   }


Point Transformation :: applied_to ( const Point& p ) const

   {
      double            sum, a[4], aa[4] ;

      a[0] = p.x() ;
      a[1] = p.y() ;
      a[2] = p.z() ;
      a[3] = 1.0 ;

      for ( int i = 0 ; i < 4 ; i++ ) {
         sum = 0.0 ;
         for ( int j = 0 ; j < 4 ; j++ ) {
            sum += a[j] * _matrix[j][i] ;
            }
         aa[i] = sum ;
         }

      if ( aa[3] != 1.0 ) {
         aa[0] /= aa[3] ;
         aa[1] /= aa[3] ;
         aa[2] /= aa[3] ;
         }

      return ( Point ( aa[0], aa[1], aa[2], p.r(), p.g(), p.b(), p.a() ) ) ;
   }


ListOfPoints Transformation :: applied_to ( const ListOfPoints& p ) const

   {
      ListOfPoints l ;
      int n = p.length() ;

      for ( int i = 0 ; i < n ; i++ )
	 l.add ( applied_to ( p[i] ) ) ;

      return ( l ) ;
   }


ProjPoint Transformation :: applied_to ( const ProjPoint& p ) const

   {
      double            sum, a[4], aa[4] ;

      a[0] = p.x() ;
      a[1] = p.y() ;
      a[2] = p.z() ;
      a[3] = p.w() ;

      for ( int i = 0 ; i < 4 ; i++ ) {
         sum = 0.0 ;
         for ( int j = 0 ; j < 4 ; j++ ) {
            sum += a[j] * _matrix[j][i] ;
            }
         aa[i] = sum ;
         }

      return ( ProjPoint ( aa[0], aa[1], aa[2], aa[3], p.r(), p.g(), p.b(), p.a() ) ) ;
   }


ListOfProjPoints Transformation :: applied_to ( const ListOfProjPoints& p ) const

   {
      ListOfProjPoints l ;
      int n = p.length() ;

      for ( int i = 0 ; i < n ; i++ )
	 l.add ( applied_to ( p[i] ) ) ;

      return ( l ) ;
   }

Frame Transformation :: applied_to ( const Frame& f ) const

   {
      Transformation t ;

      Vector uu = applied_to ( f.u() ) ;
      Vector vv = applied_to ( f.v() ) ;
      Vector ww = applied_to ( f.w() ) ;
      Point oo = applied_to ( f.origin() ) ;

      return ( Frame ( uu, vv, ww, oo ) ) ;
   }

