/******************************************
 * C++ Spherical Linear intERPolations (SLERP)
 * Version: 1.0.9
 * Author:  Douglas Wilhelm Harder
 * Date:    2008/03/03
 *
 * Copyright (c) 2007-8 by Douglas Wilhelm Harder.
 * All rights reserved.
 *
 * References:
 *    Ken Shoemake, 'Animating Rotation with Quatenrion Curves', SIGGRAPH 95.
 *    Erik B. Dam, Martin Koch, Martin Lillholm,
 *      'Quaternions, Interpolation and Animation', Technical Report DIKU-TR-98/5.
 ******************************************/

#include "Slerp.h"
#include "Complex.h"
#include "Quaternion.h"
#include <cmath>

template< typename T, typename S>
Slerp<T, S>::Slerp( const T & q1, const T & q2 ) {
	left = q1/q1.abs();
	T right = q2/q2.abs();

	equal = ( q1 == q2 );

	if ( equal ) {
		return;
	}

	right = left.conj() * right;

	S abs_imag = right.abs_imag();
	imag = right.imag() / abs_imag;

	ln = 0.5*std::log(
		right.real() * right.real() +
		abs_imag * abs_imag
	);

	atan = std::atan2( abs_imag, right.real() );
}

template< typename T, typename S>
T Slerp<T, S>::value( S t ) const {
	if ( equal ) {
		return left;
	}

	S exptln = std::exp( t*ln );

	return ( left * (
		       exptln*std::cos( t*atan ) +
		imag*( exptln*std::sin( t*atan ) )
	) ).imag();
}

template class Slerp< Quaternion<long double>, long double >;
template class Slerp< Quaternion<double>, double >;
template class Slerp< Quaternion<float>, float >;
