#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "Vector3d.h"
#include "Vector2d.h"

//////////////////////////////////////////////////////////////////////////
// Public
//////////////////////////////////////////////////////////////////////////

// static
inline const Vector3d Vector3d::ZERO = Vector3d( 0, 0, 0 );

// static
inline const Vector3d Vector3d::UP = Vector3d( 0, 1, 0 );

// static
inline const Vector3d Vector3d::RIGHT = Vector3d( 1, 0, 0 );

// static
inline const Vector3d Vector3d::FORWARD = Vector3d( 0, 0, -1 );

inline Vector3d::Vector3d( double f )
{
    m_elements[0] = f;
    m_elements[1] = f;
    m_elements[2] = f;
}

inline Vector3d::Vector3d( double x, double y, double z )
{
    m_elements[0] = x;
    m_elements[1] = y;
    m_elements[2] = z;
}

inline Vector3d::Vector3d( const Vector2d& xy, double z )
{
	m_elements[0] = xy.x();
	m_elements[1] = xy.y();
	m_elements[2] = z;
}

inline Vector3d::Vector3d( double x, const Vector2d& yz )
{
	m_elements[0] = x;
	m_elements[1] = yz.x();
	m_elements[2] = yz.y();
}

inline Vector3d::Vector3d( const Vector3d& rv )
{
    m_elements[0] = rv[0];
    m_elements[1] = rv[1];
    m_elements[2] = rv[2];
}

inline Vector3d& Vector3d::operator = ( const Vector3d& rv )
{
    if( this != &rv )
    {
        m_elements[0] = rv[0];
        m_elements[1] = rv[1];
        m_elements[2] = rv[2];
    }
    return *this;
}

inline const double& Vector3d::operator [] ( int i ) const
{
    return m_elements[i];
}

inline double& Vector3d::operator [] ( int i )
{
    return m_elements[i];
}

inline double& Vector3d::x()
{
    return m_elements[0];
}

inline double& Vector3d::y()
{
    return m_elements[1];
}

inline double& Vector3d::z()
{
    return m_elements[2];
}

inline double Vector3d::x() const
{
    return m_elements[0];
}

inline double Vector3d::y() const
{
    return m_elements[1];
}

inline double Vector3d::z() const
{
    return m_elements[2];
}

inline Vector2d Vector3d::xy() const
{
	return Vector2d( m_elements[0], m_elements[1] );
}

inline Vector2d Vector3d::xz() const
{
	return Vector2d( m_elements[0], m_elements[2] );
}

inline Vector2d Vector3d::yz() const
{
	return Vector2d( m_elements[1], m_elements[2] );
}

inline Vector3d Vector3d::xyz() const
{
	return Vector3d( m_elements[0], m_elements[1], m_elements[2] );
}

inline Vector3d Vector3d::yzx() const
{
	return Vector3d( m_elements[1], m_elements[2], m_elements[0] );
}

inline Vector3d Vector3d::zxy() const
{
	return Vector3d( m_elements[2], m_elements[0], m_elements[1] );
}

inline double Vector3d::length() const
{
	return sqrt( m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2] );
}

inline double Vector3d::squaredLength() const
{
    return
        (
            m_elements[0] * m_elements[0] +
            m_elements[1] * m_elements[1] +
            m_elements[2] * m_elements[2]
        );
}

inline void Vector3d::normalize()
{
	double norm = length();
	m_elements[0] /= norm;
	m_elements[1] /= norm;
	m_elements[2] /= norm;
}

inline Vector3d Vector3d::normalized() const
{
	double norm = length();
	return Vector3d
		(
			m_elements[0] / norm,
			m_elements[1] / norm,
			m_elements[2] / norm
		);
}

inline Vector2d Vector3d::homogenized() const
{
	return Vector2d
		(
			m_elements[ 0 ] / m_elements[ 2 ],
			m_elements[ 1 ] / m_elements[ 2 ]
		);
}

inline void Vector3d::negate()
{
	m_elements[0] = -m_elements[0];
	m_elements[1] = -m_elements[1];
	m_elements[2] = -m_elements[2];
}

inline Vector3d::operator const double* () const
{
    return m_elements;
}

inline Vector3d::operator double* ()
{
    return m_elements;
}

inline void Vector3d::print() const
{
	printf( "< %.4lf, %.4lf, %.4lf >\n",
		m_elements[0], m_elements[1], m_elements[2] );
}

inline Vector3d& Vector3d::operator += ( const Vector3d& v )
{
	m_elements[ 0 ] += v.m_elements[ 0 ];
	m_elements[ 1 ] += v.m_elements[ 1 ];
	m_elements[ 2 ] += v.m_elements[ 2 ];
	return *this;
}

inline Vector3d& Vector3d::operator -= ( const Vector3d& v )
{
	m_elements[ 0 ] -= v.m_elements[ 0 ];
	m_elements[ 1 ] -= v.m_elements[ 1 ];
	m_elements[ 2 ] -= v.m_elements[ 2 ];
	return *this;
}

inline Vector3d& Vector3d::operator *= ( double f )
{
	m_elements[ 0 ] *= f;
	m_elements[ 1 ] *= f;
	m_elements[ 2 ] *= f;
	return *this;
}

// static
inline double Vector3d::dot( const Vector3d& v0, const Vector3d& v1 )
{
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

// static
inline Vector3d Vector3d::cross( const Vector3d& v0, const Vector3d& v1 )
{
    return Vector3d
        (
            v0.y() * v1.z() - v0.z() * v1.y(),
            v0.z() * v1.x() - v0.x() * v1.z(),
            v0.x() * v1.y() - v0.y() * v1.x()
        );
}

// static
inline Vector3d Vector3d::lerp( const Vector3d& v0, const Vector3d& v1, double alpha )
{
	return alpha * ( v1 - v0 ) + v0;
}

// static
inline Vector3d Vector3d::cubicInterpolate( const Vector3d& p0, const Vector3d& p1, const Vector3d& p2, const Vector3d& p3, double t )
{
	// geometric construction:
	//            t
	//   (t+1)/2     t/2
	// t+1        t	        t-1

	// bottom level
	Vector3d p0p1 = Vector3d::lerp( p0, p1, t + 1 );
	Vector3d p1p2 = Vector3d::lerp( p1, p2, t );
	Vector3d p2p3 = Vector3d::lerp( p2, p3, t - 1 );

	// middle level
	Vector3d p0p1_p1p2 = Vector3d::lerp( p0p1, p1p2, 0.5f * ( t + 1 ) );
	Vector3d p1p2_p2p3 = Vector3d::lerp( p1p2, p2p3, 0.5f * t );

	// top level
	return Vector3d::lerp( p0p1_p1p2, p1p2_p2p3, t );
}

inline Vector3d operator + ( const Vector3d& v0, const Vector3d& v1 )
{
    return Vector3d( v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2] );
}

inline Vector3d operator - ( const Vector3d& v0, const Vector3d& v1 )
{
    return Vector3d( v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2] );
}

inline Vector3d operator * ( const Vector3d& v0, const Vector3d& v1 )
{
    return Vector3d( v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2] );
}

inline Vector3d operator / ( const Vector3d& v0, const Vector3d& v1 )
{
    return Vector3d( v0[0] / v1[0], v0[1] / v1[1], v0[2] / v1[2] );
}

inline Vector3d operator - ( const Vector3d& v )
{
    return Vector3d( -v[0], -v[1], -v[2] );
}

inline Vector3d operator * ( double f, const Vector3d& v )
{
    return Vector3d( v[0] * f, v[1] * f, v[2] * f );
}

inline Vector3d operator * ( const Vector3d& v, double f )
{
    return Vector3d( v[0] * f, v[1] * f, v[2] * f );
}

inline Vector3d operator / ( const Vector3d& v, double f )
{
    return Vector3d( v[0] / f, v[1] / f, v[2] / f );
}

inline bool operator == ( const Vector3d& v0, const Vector3d& v1 )
{
    return( v0.x() == v1.x() && v0.y() == v1.y() && v0.z() == v1.z() );
}

inline bool operator != ( const Vector3d& v0, const Vector3d& v1 )
{
    return !( v0 == v1 );
}
