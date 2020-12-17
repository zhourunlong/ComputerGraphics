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
const Vector2d Vector2d::ZERO = Vector2d( 0, 0 );

// static
const Vector2d Vector2d::UP = Vector2d( 0, 1 );

// static
const Vector2d Vector2d::RIGHT = Vector2d( 1, 0 );

Vector2d::Vector2d( double f )
{
    m_elements[0] = f;
    m_elements[1] = f;
}

Vector2d::Vector2d( double x, double y )
{
    m_elements[0] = x;
    m_elements[1] = y;
}

Vector2d::Vector2d( const Vector2d& rv )
{
    m_elements[0] = rv[0];
    m_elements[1] = rv[1];
}

Vector2d& Vector2d::operator = ( const Vector2d& rv )
{
 	if( this != &rv )
	{
        m_elements[0] = rv[0];
        m_elements[1] = rv[1];
    }
    return *this;
}

const double& Vector2d::operator [] ( int i ) const
{
    return m_elements[i];
}

double& Vector2d::operator [] ( int i )
{
    return m_elements[i];
}

double& Vector2d::x()
{
    return m_elements[0];
}

double& Vector2d::y()
{
    return m_elements[1];
}

double Vector2d::x() const
{
    return m_elements[0];
}	

double Vector2d::y() const
{
    return m_elements[1];
}

Vector2d Vector2d::xy() const
{
    return *this;
}

Vector2d Vector2d::yx() const
{
    return Vector2d( m_elements[1], m_elements[0] );
}

Vector2d Vector2d::xx() const
{
    return Vector2d( m_elements[0], m_elements[0] );
}

Vector2d Vector2d::yy() const
{
    return Vector2d( m_elements[1], m_elements[1] );
}

Vector2d Vector2d::normal() const
{
    return Vector2d( -m_elements[1], m_elements[0] );
}

double Vector2d::abs() const
{
    return sqrt(absSquared());
}

double Vector2d::absSquared() const
{
    return m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1];
}

void Vector2d::normalize()
{
    double norm = abs();
    m_elements[0] /= norm;
    m_elements[1] /= norm;
}

Vector2d Vector2d::normalized() const
{
    double norm = abs();
    return Vector2d( m_elements[0] / norm, m_elements[1] / norm );
}

void Vector2d::negate()
{
    m_elements[0] = -m_elements[0];
    m_elements[1] = -m_elements[1];
}

Vector2d::operator const double* () const
{
    return m_elements;
}

Vector2d::operator double* ()
{
    return m_elements;
}

void Vector2d::print() const
{
	printf( "< %.4d, %.4d >\n",
		m_elements[0], m_elements[1] );
}

Vector2d& Vector2d::operator += ( const Vector2d& v )
{
	m_elements[ 0 ] += v.m_elements[ 0 ];
	m_elements[ 1 ] += v.m_elements[ 1 ];
	return *this;
}

Vector2d& Vector2d::operator -= ( const Vector2d& v )
{
	m_elements[ 0 ] -= v.m_elements[ 0 ];
	m_elements[ 1 ] -= v.m_elements[ 1 ];
	return *this;
}

Vector2d& Vector2d::operator *= ( double f )
{
	m_elements[ 0 ] *= f;
	m_elements[ 1 ] *= f;
	return *this;
}

// static
double Vector2d::dot( const Vector2d& v0, const Vector2d& v1 )
{
    return v0[0] * v1[0] + v0[1] * v1[1];
}

// static
Vector3d Vector2d::cross( const Vector2d& v0, const Vector2d& v1 )
{
	return Vector3d
		(
			0,
			0,
			v0.x() * v1.y() - v0.y() * v1.x()
		);
}

// static
Vector2d Vector2d::lerp( const Vector2d& v0, const Vector2d& v1, double alpha )
{
	return alpha * ( v1 - v0 ) + v0;
}

//////////////////////////////////////////////////////////////////////////
// Operator overloading
//////////////////////////////////////////////////////////////////////////

Vector2d operator + ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() + v1.x(), v0.y() + v1.y() );
}

Vector2d operator - ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() - v1.x(), v0.y() - v1.y() );
}

Vector2d operator * ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() * v1.x(), v0.y() * v1.y() );
}

Vector2d operator / ( const Vector2d& v0, const Vector2d& v1 )
{
    return Vector2d( v0.x() / v1.x(), v0.y() / v1.y() );
}

Vector2d operator - ( const Vector2d& v )
{
    return Vector2d( -v.x(), -v.y() );
}

Vector2d operator * ( double f, const Vector2d& v )
{
    return Vector2d( f * v.x(), f * v.y() );
}

Vector2d operator * ( const Vector2d& v, double f )
{
    return Vector2d( f * v.x(), f * v.y() );
}

Vector2d operator / ( const Vector2d& v, double f )
{
    return Vector2d( v.x() / f, v.y() / f );
}

bool operator == ( const Vector2d& v0, const Vector2d& v1 )
{
    return( v0.x() == v1.x() && v0.y() == v1.y() );
}

bool operator != ( const Vector2d& v0, const Vector2d& v1 )
{
    return !( v0 == v1 );
}
