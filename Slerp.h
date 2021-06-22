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

#ifndef CA_UWATERLOO_ALUMNI_DWHARDER_SLERP
#define CA_UWATERLOO_ALUMNI_DWHARDER_SLERP

template< typename T, typename S = double >
class Slerp {
	private:
		T left, imag;
		S ln, atan;
		bool equal;

	public:
		Slerp( const T & = T::I, const T & = T::I );
		T value( S ) const;
};

#endif
