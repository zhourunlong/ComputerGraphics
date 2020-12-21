#pragma once

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "Vector2d.h"
#include "Vector3d.h"

//////////////////////////////////////////////////////////////////////////
// Public
//////////////////////////////////////////////////////////////////////////

// static
inline const Vector2d Vector2d::ZERO = Vector2d( 0, 0 );

// static
inline const Vector2d Vector2d::UP = Vector2d( 0, 1 );

// static
inline const Vector2d Vector2d::RIGHT = Vector2d( 1, 0 );

inline Vector2d::Vector2d( double f )
{
    m_elements[0] = f;
    m_elements[1] = f;
}

inline Vector2d::Vector2d( double x, double y )
{
    m_elements[0] = x;
    m_elements[1] = y;
}

inline Vector2d::Vector2d( const Vector2d& rv )
{
    m_elements[0] = rv[0];
    m_elements[1] = rv[1];
}

inline Vector2d& Vector2d::operator = ( const Vector2d& rv )
{
 	if( this != &rv )
	{
        m_elements[0] = rv[0];
        m_elements[1] = rv[1];
    }
    return *this;
}

inline const double& Vector2d::operator [] ( int i ) const
{
    return m_elements[i];
}

inline double& Vector2d::operator [] ( int i )
{
    return m_elements[i];
}

inline double& Vector2d::x()
{
    return m_elements[0];
}

inline double& Vector2d::y()
{
    return m_elements[1];
}

inline double Vector2d::x() const
{
    return m_elements[0];
}	

inline double Vector2d::y() const
{
    return m_elements[1];
}

inline Vector2d Vector2d::xy() const
{
    return *this;
}

inline Vector2d Vector2d::yx() const
{
    return Vector2d( m_elements[1], m_elements[0] );
}

inline Vector2d Vector2d::xx() const
{
    return Vector2d( m_elements[0], m_elements[0] );
}

inline Vector2d Vector2d::yy() const
{
    return Vector2d( m_elements[1], m_elements[1] );
}

inline Vector2d Vector2d::normal() const
{
    return Vector2d( -m_elements[1], m_elements[0] );
}

inline double Vector2d::abs() const
{
    return sqrt(absSquared());
}

inline double Vector2d::absSquared() const
{
    return m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1];
}

inline void Vector2d::normalize()
{
    double norm = abs();
    m_elements[0] /= norm;
    m_elements[1] /= norm;
}

inline Vector2d Vector2d::normalized() const
{
    double norm = abs();
    return Vector2d( m_elements[0] / norm, m_elements[1] / norm );
}

inline void Vector2d::negate()
{
    m_elements[0] = -m_elements[0];
    m_elements[1] = -m_elements[1];
}

inline Vector2d::operator const double* () const
{
    return m_elements;
}

inline Vector2d::operator double* ()
{
    return m_elements;
}

inline void Vector2d::print() const
{
	printf( "< %.4lf, %.4lf >\n",
		m_elements[0], m_elements[1] );
}

inline Vector2d& Vector2d::operator += ( const Vector2d& v )
{
	m_elements[ 0 ] += v.m_elements[ 0 ];
	m_elements[ 1 ] += v.m_elements[ 1 ];
	return *this;
}

inline Vector2d& Vector2d::operator -= ( const Vector2d& v )
{
	m_elements[ 0 ] -= v.m_elements[ 0 ];
	m_elements[ 1 ] -= v.m_elements[ 1 ];
	return *this;
}

inline Vector2d& Vector2d::operator *= ( double f )
{
	m_elements[ 0 ] *= f;
	m_elements[ 1 ] *= f;
	return *this;
}

// static
inline double Vector2d::dot( const Vector2d& v0, const Vector2d& v1 )
{
    return v0[0] * v1[0] + v0[1] * v1[1];
}

// static
inline Vector3d Vector2d::cross( const Vector2d& v0, const Vector2d& v1 )
{
	return Vector3d
		(
			0,
			0,
			v0.x() * v1.y() - v0.y() * v1.x()
		);
}

// static
inline Vector2d Vector2d::lerp( const Vector2d& v0, const Vector2d& v1, double alpha )
{
	return alpha * ( v1 - v0 ) + v0;
}

//////////////////////////////////////////////////////////////////////////
// Operator overloading
//////////////////////////////////////////////////////////////////////////

inline Vector2d operator + ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() + v1.x(), v0.y() + v1.y() );
}

inline Vector2d operator - ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() - v1.x(), v0.y() - v1.y() );
}

inline Vector2d operator * ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() * v1.x(), v0.y() * v1.y() );
}

inline Vector2d operator / ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() / v1.x(), v0.y() / v1.y() );
}

inline Vector2d operator - ( const Vector2d& v )
{
    return Vector2d( -v.x(), -v.y() );
}

inline Vector2d operator * ( double f, const Vector2d& v )
{
    return Vector2d( f * v.x(), f * v.y() );
}

inline Vector2d operator * ( const Vector2d& v, double f )
{
    return Vector2d( f * v.x(), f * v.y() );
}

inline Vector2d operator / ( const Vector2d& v, double f )
{
    return Vector2d( v.x() / f, v.y() / f );
}

inline bool operator == ( const Vector2d& v0, const Vector2d& v1 )
{
    return( v0.x() == v1.x() && v0.y() == v1.y() );
}

inline bool operator != ( const Vector2d& v0, const Vector2d& v1 )
{
    return !( v0 == v1 );
}
