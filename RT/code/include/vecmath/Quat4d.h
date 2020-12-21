#pragma once

class Vector3d;
class Vector4d;

#include "Matrix3d.h"

class Quat4d
{
public:

	static const Quat4d ZERO;
	static const Quat4d IDENTITY;

	Quat4d();

	// q = w + x * i + y * j + z * k
	Quat4d( double w, double x, double y, double z );
		
	Quat4d( const Quat4d& rq ); // copy constructor
	Quat4d& operator = ( const Quat4d& rq ); // assignment operator
	// no destructor necessary

	// returns a quaternion with 0 real part
	Quat4d( const Vector3d& v );

	// copies the components of a Vector4d directly into this quaternion
	Quat4d( const Vector4d& v );

	// returns the ith element
	const double& operator [] ( int i ) const;
	double& operator [] ( int i );

	double w() const;
	double x() const;
	double y() const;
	double z() const;
	Vector3d xyz() const;
	Vector4d wxyz() const;

	double abs() const;
	double absSquared() const;
	void normalize();
	Quat4d normalized() const;

	void conjugate();
	Quat4d conjugated() const;

	void invert();
	Quat4d inverse() const;

	// log and exponential maps
	Quat4d log() const;
	Quat4d exp() const;
	
	// returns unit vector for rotation and radians about the unit vector
	Vector3d getAxisAngle( double* radiansOut );

	// sets this quaternion to be a rotation of fRadians about v = < fx, fy, fz >, v need not necessarily be unit length
	void setAxisAngle( double radians, const Vector3d& axis );

	// ---- Utility ----
	void print();
 
	 // quaternion dot product (a la vector)
	static double dot( const Quat4d& q0, const Quat4d& q1 );	
	
	// linear (stupid) interpolation
	static Quat4d lerp( const Quat4d& q0, const Quat4d& q1, double alpha );

	// spherical linear interpolation
	static Quat4d slerp( const Quat4d& a, const Quat4d& b, double t, bool allowFlip = true );
	
	// spherical quadratic interoplation between a and b at point t
	// given quaternion tangents tanA and tanB (can be computed using squadTangent)	
	static Quat4d squad( const Quat4d& a, const Quat4d& tanA, const Quat4d& tanB, const Quat4d& b, double t );

	static Quat4d cubicInterpolate( const Quat4d& q0, const Quat4d& q1, const Quat4d& q2, const Quat4d& q3, double t );

	// Log-difference between a and b, used for squadTangent
	// returns log( a^-1 b )	
	static Quat4d logDifference( const Quat4d& a, const Quat4d& b );

	// Computes a tangent at center, defined by the before and after quaternions
	// Useful for squad()
	static Quat4d squadTangent( const Quat4d& before, const Quat4d& center, const Quat4d& after );		

	static Quat4d fromRotationMatrix( const Matrix3d& m );

	static Quat4d fromRotatedBasis( const Vector3d& x, const Vector3d& y, const Vector3d& z );

	// returns a unit quaternion that's a uniformly distributed rotation
	// given u[i] is a uniformly distributed random number in [0,1]
	// taken from Graphics Gems II
	static Quat4d randomRotation( double u0, double u1, double u2 );

private:

	double m_elements[ 4 ];

};

Quat4d operator + ( const Quat4d& q0, const Quat4d& q1 );
Quat4d operator - ( const Quat4d& q0, const Quat4d& q1 );
Quat4d operator * ( const Quat4d& q0, const Quat4d& q1 );
Quat4d operator * ( double f, const Quat4d& q );
Quat4d operator * ( const Quat4d& q, double f );
