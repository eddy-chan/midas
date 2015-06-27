#ifndef _TRANSFORMATIONS_H
#define _TRANSFORMATIONS_H

//
//  ============================================================
//
//                  Transformation
//
// ============================================================
//
//
//      Copyright (C) 1997
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

#include "Vector.h"
#include "ListOfVectors.h"
#include "Point.h"
#include "ListOfPoints.h"
#include "ProjPoint.h"
#include "ListOfProjPoints.h"
#include "Frame.h"

enum transformation_type { TRANSLATE, SCALE } ;

class	Transformation {

   private :

      double		_matrix[4][4] ;

		// Private function creating an identity
		//    transform.

   private:

      void id () ;

   public :

			// Constructors

      Transformation () ;

			// Copy Constructors

      Transformation ( const Transformation& ) ;

			// Destructor

      virtual ~Transformation () ;

			// Assignment

      Transformation& operator= ( const Transformation& ) ;

			// Comparison

      friend int operator== ( const Transformation&,
			      const Transformation& ) ;
      friend int operator!= ( const Transformation&,
			      const Transformation& ) ;

			// Output

      friend ostream& operator<< ( ostream&, const Transformation& ) ;

      ostream& output ( ostream&, const int = 0 ) const ;

			// Arithmetic Operations

      friend Transformation operator+ ( const Transformation&,
					const Transformation& ) ;
      friend Transformation operator- ( const Transformation&,
					const Transformation& ) ;
      friend Transformation operator- ( const Transformation& ) ;
      friend Transformation operator* ( const double,
      					const Transformation& ) ;
      friend Transformation operator* ( const Transformation&,
					const double ) ;
      friend Transformation operator* ( const Transformation&,
					const Transformation& ) ;

      Transformation operator+= ( const Transformation& ) ;
      Transformation operator-= ( const Transformation& ) ;
      Transformation operator*= ( const Transformation& ) ;

			//  The following produce
			//   specific elementary transformations

      friend Transformation identity () ;
      static Transformation translate_by (  const double,
					      const double = 0.0,
					      const double = 0.0 ) ;
      static Transformation translate_by ( const Vector ) ;

      static Transformation scale_by (  const double,
					  const double = 1.0,
					  const double = 1.0 ) ;
      static Transformation scale_by ( const Vector ) ;

      static Transformation x_rotate_by ( const double ) ;

      static Transformation y_rotate_by ( const double ) ;

      static Transformation z_rotate_by ( const double ) ;

      static Transformation rotate_by ( const double,
					const Vector& ) ;

      static Transformation frame_to_frame ( const Frame&,
					   const Frame& ) ;

      static Transformation view ( const double,
				   const double,
				   const double ) ;

      friend Transformation inverse_view ( const double,
					   const double,
					   const double ) ;

			//  Applying the Transformations

      Vector applied_to ( const Vector& ) const ;

      ListOfVectors applied_to ( const ListOfVectors& ) const ;

      Point applied_to ( const Point& ) const ;

      ListOfPoints applied_to ( const ListOfPoints& ) const ;

      ProjPoint applied_to ( const ProjPoint& ) const ;

      ListOfProjPoints applied_to ( const ListOfProjPoints& ) const ;

      Frame applied_to ( const Frame& ) const ;

			//  Member functions

      double matrix ( const int, const int ) const ;

   } ;

#endif

