#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "Vector4d.h"
#include "Vector2d.h"
#include "Vector3d.h"

inline Vector4d::Vector4d( double f )
{
	m_elements[ 0 ] = f;
	m_elements[ 1 ] = f;
	m_elements[ 2 ] = f;
	m_elements[ 3 ] = f;
}

inline Vector4d::Vector4d( double fx, double fy, double fz, double fw )
{
	m_elements[0] = fx;
	m_elements[1] = fy;
	m_elements[2] = fz;
	m_elements[3] = fw;
}

inline Vector4d::Vector4d( double buffer[ 4 ] )
{
	m_elements[ 0 ] = buffer[ 0 ];
	m_elements[ 1 ] = buffer[ 1 ];
	m_elements[ 2 ] = buffer[ 2 ];
	m_elements[ 3 ] = buffer[ 3 ];
}

inline Vector4d::Vector4d( const Vector2d& xy, double z, double w )
{
	m_elements[0] = xy.x();
	m_elements[1] = xy.y();
	m_elements[2] = z;
	m_elements[3] = w;
}

inline Vector4d::Vector4d( double x, const Vector2d& yz, double w )
{
	m_elements[0] = x;
	m_elements[1] = yz.x();
	m_elements[2] = yz.y();
	m_elements[3] = w;
}

inline Vector4d::Vector4d( double x, double y, const Vector2d& zw )
{
	m_elements[0] = x;
	m_elements[1] = y;
	m_elements[2] = zw.x();
	m_elements[3] = zw.y();
}

inline Vector4d::Vector4d( const Vector2d& xy, const Vector2d& zw )
{
	m_elements[0] = xy.x();
	m_elements[1] = xy.y();
	m_elements[2] = zw.x();
	m_elements[3] = zw.y();
}

inline Vector4d::Vector4d( const Vector3d& xyz, double w )
{
	m_elements[0] = xyz.x();
	m_elements[1] = xyz.y();
	m_elements[2] = xyz.z();
	m_elements[3] = w;
}

inline Vector4d::Vector4d( double x, const Vector3d& yzw )
{
	m_elements[0] = x;
	m_elements[1] = yzw.x();
	m_elements[2] = yzw.y();
	m_elements[3] = yzw.z();
}

inline Vector4d::Vector4d( const Vector4d& rv )
{
	m_elements[0] = rv.m_elements[0];
	m_elements[1] = rv.m_elements[1];
	m_elements[2] = rv.m_elements[2];
	m_elements[3] = rv.m_elements[3];
}

inline Vector4d& Vector4d::operator = ( const Vector4d& rv )
{
	if( this != &rv )
	{
		m_elements[0] = rv.m_elements[0];
		m_elements[1] = rv.m_elements[1];
		m_elements[2] = rv.m_elements[2];
		m_elements[3] = rv.m_elements[3];
	}
	return *this;
}

inline const double& Vector4d::operator [] ( int i ) const
{
	return m_elements[ i ];
}

inline double& Vector4d::operator [] ( int i )
{
	return m_elements[ i ];
}

inline double& Vector4d::x()
{
	return m_elements[ 0 ];
}

inline double& Vector4d::y()
{
	return m_elements[ 1 ];
}

inline double& Vector4d::z()
{
	return m_elements[ 2 ];
}

inline double& Vector4d::w()
{
	return m_elements[ 3 ];
}

inline double Vector4d::x() const
{
	return m_elements[0];
}

inline double Vector4d::y() const
{
	return m_elements[1];
}

inline double Vector4d::z() const
{
	return m_elements[2];
}

inline double Vector4d::w() const
{
	return m_elements[3];
}

inline Vector2d Vector4d::xy() const
{
	return Vector2d( m_elements[0], m_elements[1] );
}

inline Vector2d Vector4d::yz() const
{
	return Vector2d( m_elements[1], m_elements[2] );
}

inline Vector2d Vector4d::zw() const
{
	return Vector2d( m_elements[2], m_elements[3] );
}

inline Vector2d Vector4d::wx() const
{
	return Vector2d( m_elements[3], m_elements[0] );
}

inline Vector3d Vector4d::xyz() const
{
	return Vector3d( m_elements[0], m_elements[1], m_elements[2] );
}

inline Vector3d Vector4d::yzw() const
{
	return Vector3d( m_elements[1], m_elements[2], m_elements[3] );
}

inline Vector3d Vector4d::zwx() const
{
	return Vector3d( m_elements[2], m_elements[3], m_elements[0] );
}

inline Vector3d Vector4d::wxy() const
{
	return Vector3d( m_elements[3], m_elements[0], m_elements[1] );
}

inline Vector3d Vector4d::xyw() const
{
	return Vector3d( m_elements[0], m_elements[1], m_elements[3] );
}

inline Vector3d Vector4d::yzx() const
{
	return Vector3d( m_elements[1], m_elements[2], m_elements[0] );
}

inline Vector3d Vector4d::zwy() const
{
	return Vector3d( m_elements[2], m_elements[3], m_elements[1] );
}

inline Vector3d Vector4d::wxz() const
{
	return Vector3d( m_elements[3], m_elements[0], m_elements[2] );
}

inline double Vector4d::abs() const
{
	return sqrt( m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2] + m_elements[3] * m_elements[3] );
}

inline double Vector4d::absSquared() const
{
	return( m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2] + m_elements[3] * m_elements[3] );
}

inline void Vector4d::normalize()
{
	double norm = sqrt( m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2] + m_elements[3] * m_elements[3] );
	m_elements[0] = m_elements[0] / norm;
	m_elements[1] = m_elements[1] / norm;
	m_elements[2] = m_elements[2] / norm;
	m_elements[3] = m_elements[3] / norm;
}

inline Vector4d Vector4d::normalized() const
{
	double length = abs();
	return Vector4d
		(
			m_elements[0] / length,
			m_elements[1] / length,
			m_elements[2] / length,
			m_elements[3] / length
		);
}

inline void Vector4d::homogenize()
{
	if( m_elements[3] != 0 )
	{
		m_elements[0] /= m_elements[3];
		m_elements[1] /= m_elements[3];
		m_elements[2] /= m_elements[3];
		m_elements[3] = 1;
	}
}

inline Vector4d Vector4d::homogenized() const
{
	if( m_elements[3] != 0 )
	{
		return Vector4d
			(
				m_elements[0] / m_elements[3],
				m_elements[1] / m_elements[3],
				m_elements[2] / m_elements[3],
				1
			);
	}
	else
	{
		return Vector4d
			(
				m_elements[0],
				m_elements[1],
				m_elements[2],
				m_elements[3]
			);
	}
}

inline void Vector4d::negate()
{
	m_elements[0] = -m_elements[0];
	m_elements[1] = -m_elements[1];
	m_elements[2] = -m_elements[2];
	m_elements[3] = -m_elements[3];
}

inline Vector4d::operator const double* () const
{
	return m_elements;
}

inline Vector4d::operator double* ()
{
	return m_elements;
}

inline void Vector4d::print() const
{
	printf( "< %.4lf, %.4lf, %.4lf, %.4lf >\n",
		m_elements[0], m_elements[1], m_elements[2], m_elements[3] );
}

// static
inline double Vector4d::dot( const Vector4d& v0, const Vector4d& v1 )
{
	return v0.x() * v1.x() + v0.y() * v1.y() + v0.z() * v1.z() + v0.w() * v1.w();
}

// static
inline Vector4d Vector4d::lerp( const Vector4d& v0, const Vector4d& v1, double alpha )
{
	return alpha * ( v1 - v0 ) + v0;
}

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////

inline Vector4d operator + ( const Vector4d& v0, const Vector4d& v1 )
{
	return Vector4d( v0.x() + v1.x(), v0.y() + v1.y(), v0.z() + v1.z(), v0.w() + v1.w() );
}

inline Vector4d operator - ( const Vector4d& v0, const Vector4d& v1 )
{
	return Vector4d( v0.x() - v1.x(), v0.y() - v1.y(), v0.z() - v1.z(), v0.w() - v1.w() );
}

inline Vector4d operator * ( const Vector4d& v0, const Vector4d& v1 )
{
	return Vector4d( v0.x() * v1.x(), v0.y() * v1.y(), v0.z() * v1.z(), v0.w() * v1.w() );
}

inline Vector4d operator / ( const Vector4d& v0, const Vector4d& v1 )
{
	return Vector4d( v0.x() / v1.x(), v0.y() / v1.y(), v0.z() / v1.z(), v0.w() / v1.w() );
}

inline Vector4d operator - ( const Vector4d& v )
{
	return Vector4d( -v.x(), -v.y(), -v.z(), -v.w() );
}

inline Vector4d operator * ( double f, const Vector4d& v )
{
	return Vector4d( f * v.x(), f * v.y(), f * v.z(), f * v.w() );
}

inline Vector4d operator * ( const Vector4d& v, double f )
{
	return Vector4d( f * v.x(), f * v.y(), f * v.z(), f * v.w() );
}

inline Vector4d operator / ( const Vector4d& v, double f )
{
    return Vector4d( v[0] / f, v[1] / f, v[2] / f, v[3] / f );
}

inline bool operator == ( const Vector4d& v0, const Vector4d& v1 )
{
    return( v0.x() == v1.x() && v0.y() == v1.y() && v0.z() == v1.z() && v0.w() == v1.w() );
}

inline bool operator != ( const Vector4d& v0, const Vector4d& v1 )
{
    return !( v0 == v1 );
}
