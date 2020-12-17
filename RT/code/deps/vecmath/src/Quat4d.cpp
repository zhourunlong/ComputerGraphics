#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>

#include "Quat4d.h"
#include "Vector3d.h"
#include "Vector4d.h"

//////////////////////////////////////////////////////////////////////////
// Public
//////////////////////////////////////////////////////////////////////////

// static
const Quat4d Quat4d::ZERO = Quat4d( 0, 0, 0, 0 );

// static
const Quat4d Quat4d::IDENTITY = Quat4d( 1, 0, 0, 0 );

Quat4d::Quat4d()
{
	m_elements[ 0 ] = 0;
	m_elements[ 1 ] = 0;
	m_elements[ 2 ] = 0;
	m_elements[ 3 ] = 0;
}

Quat4d::Quat4d( double w, double x, double y, double z )
{
	m_elements[ 0 ] = w;
	m_elements[ 1 ] = x;
	m_elements[ 2 ] = y;
	m_elements[ 3 ] = z;
}

Quat4d::Quat4d( const Quat4d& rq )
{
	m_elements[ 0 ] = rq.m_elements[ 0 ];
	m_elements[ 1 ] = rq.m_elements[ 1 ];
	m_elements[ 2 ] = rq.m_elements[ 2 ];
	m_elements[ 3 ] = rq.m_elements[ 3 ];
}

Quat4d& Quat4d::operator = ( const Quat4d& rq )
{
	if( this != ( &rq ) )
	{
		m_elements[ 0 ] = rq.m_elements[ 0 ];
		m_elements[ 1 ] = rq.m_elements[ 1 ];
		m_elements[ 2 ] = rq.m_elements[ 2 ];
		m_elements[ 3 ] = rq.m_elements[ 3 ];
	}
    return( *this );
}

Quat4d::Quat4d( const Vector3d& v )
{
	m_elements[ 0 ] = 0;
	m_elements[ 1 ] = v[ 0 ];
	m_elements[ 2 ] = v[ 1 ];
	m_elements[ 3 ] = v[ 2 ];
}

Quat4d::Quat4d( const Vector4d& v )
{
	m_elements[ 0 ] = v[ 0 ];
	m_elements[ 1 ] = v[ 1 ];
	m_elements[ 2 ] = v[ 2 ];
	m_elements[ 3 ] = v[ 3 ];
}

const double& Quat4d::operator [] ( int i ) const
{
	return m_elements[ i ];
}

double& Quat4d::operator [] ( int i )
{
	return m_elements[ i ];
}

double Quat4d::w() const
{
	return m_elements[ 0 ];
}

double Quat4d::x() const
{
	return m_elements[ 1 ];
}

double Quat4d::y() const
{
	return m_elements[ 2 ];
}

double Quat4d::z() const
{
	return m_elements[ 3 ];
}

Vector3d Quat4d::xyz() const
{
	return Vector3d
	(
		m_elements[ 1 ],
		m_elements[ 2 ],
		m_elements[ 3 ]
	);
}

Vector4d Quat4d::wxyz() const
{
	return Vector4d
	(
		m_elements[ 0 ],
		m_elements[ 1 ],
		m_elements[ 2 ],
		m_elements[ 3 ]
	);
}

double Quat4d::abs() const
{
	return sqrt( absSquared() );	
}

double Quat4d::absSquared() const
{
	return
	(
		m_elements[ 0 ] * m_elements[ 0 ] +
		m_elements[ 1 ] * m_elements[ 1 ] +
		m_elements[ 2 ] * m_elements[ 2 ] +
		m_elements[ 3 ] * m_elements[ 3 ]
	);
}

void Quat4d::normalize()
{
	double reciprocalAbs = 1.f / abs();

	m_elements[ 0 ] *= reciprocalAbs;
	m_elements[ 1 ] *= reciprocalAbs;
	m_elements[ 2 ] *= reciprocalAbs;
	m_elements[ 3 ] *= reciprocalAbs;
}

Quat4d Quat4d::normalized() const
{
	Quat4d q( *this );
	q.normalize();
	return q;
}

void Quat4d::conjugate()
{
	m_elements[ 1 ] = -m_elements[ 1 ];
	m_elements[ 2 ] = -m_elements[ 2 ];
	m_elements[ 3 ] = -m_elements[ 3 ];
}

Quat4d Quat4d::conjugated() const
{
	return Quat4d
	(
		 m_elements[ 0 ],
		-m_elements[ 1 ],
		-m_elements[ 2 ],
		-m_elements[ 3 ]
	);
}

void Quat4d::invert()
{
	Quat4d inverse = conjugated() * ( 1.0f / absSquared() );

	m_elements[ 0 ] = inverse.m_elements[ 0 ];
	m_elements[ 1 ] = inverse.m_elements[ 1 ];
	m_elements[ 2 ] = inverse.m_elements[ 2 ];
	m_elements[ 3 ] = inverse.m_elements[ 3 ];
}

Quat4d Quat4d::inverse() const
{
	return conjugated() * ( 1.0f / absSquared() );
}


Quat4d Quat4d::log() const
{
	double len =
		sqrt
		(
			m_elements[ 1 ] * m_elements[ 1 ] +
			m_elements[ 2 ] * m_elements[ 2 ] +
			m_elements[ 3 ] * m_elements[ 3 ]
		);

	if( len < 1e-6 )
	{
		return Quat4d( 0, m_elements[ 1 ], m_elements[ 2 ], m_elements[ 3 ] );
	}
	else
	{
		double coeff = acos( m_elements[ 0 ] ) / len;
		return Quat4d( 0, m_elements[ 1 ] * coeff, m_elements[ 2 ] * coeff, m_elements[ 3 ] * coeff );
	}
}

Quat4d Quat4d::exp() const
{
	double theta =
		sqrt
		(
			m_elements[ 1 ] * m_elements[ 1 ] +
			m_elements[ 2 ] * m_elements[ 2 ] +
			m_elements[ 3 ] * m_elements[ 3 ]
		);

	if( theta < 1e-6 )
	{
		return Quat4d( cos( theta ), m_elements[ 1 ], m_elements[ 2 ], m_elements[ 3 ] );
	}
	else
	{
		double coeff = sin( theta ) / theta;
		return Quat4d( cos( theta ), m_elements[ 1 ] * coeff, m_elements[ 2 ] * coeff, m_elements[ 3 ] * coeff );		
	}
}

Vector3d Quat4d::getAxisAngle( double* radiansOut )
{
	double theta = acos( w() ) * 2;
	double vectorNorm = sqrt( x() * x() + y() * y() + z() * z() );
	double reciprocalVectorNorm = 1.f / vectorNorm;

	*radiansOut = theta;
	return Vector3d
	(
		x() * reciprocalVectorNorm,
		y() * reciprocalVectorNorm,
		z() * reciprocalVectorNorm
	);
}

void Quat4d::setAxisAngle( double radians, const Vector3d& axis )
{
	m_elements[ 0 ] = cos( radians / 2 );

	double sinHalfTheta = sin( radians / 2 );
	double vectorNorm = axis.length();
	double reciprocalVectorNorm = 1.f / vectorNorm;

	m_elements[ 1 ] = axis.x() * sinHalfTheta * reciprocalVectorNorm;
	m_elements[ 2 ] = axis.y() * sinHalfTheta * reciprocalVectorNorm;
	m_elements[ 3 ] = axis.z() * sinHalfTheta * reciprocalVectorNorm;
}

void Quat4d::print()
{
	printf( "< %.4d + %.4d i + %.4d j + %.4d k >\n",
		m_elements[ 0 ], m_elements[ 1 ], m_elements[ 2 ], m_elements[ 3 ] );
}

// static
double Quat4d::dot( const Quat4d& q0, const Quat4d& q1 )
{
	return
	(
		q0.w() * q1.w() +
		q0.x() * q1.x() +
		q0.y() * q1.y() +
		q0.z() * q1.z()
	);
}

// static
Quat4d Quat4d::lerp( const Quat4d& q0, const Quat4d& q1, double alpha )
{
	return( ( q0 + alpha * ( q1 - q0 ) ).normalized() );
}

// static
Quat4d Quat4d::slerp( const Quat4d& a, const Quat4d& b, double t, bool allowFlip )
{
	double cosAngle = Quat4d::dot( a, b );

	double c1;
	double c2;

	// Linear interpolation for close orientations
	if( ( 1.0f - fabs( cosAngle ) ) < 0.01f )
	{
		c1 = 1.0f - t;
		c2 = t;
	}
	else
	{
		// Spherical interpolation
		double angle = acos( fabs( cosAngle ) );
		double sinAngle = sin( angle );
		c1 = sin( angle * ( 1.0f - t ) ) / sinAngle;
		c2 = sin( angle * t ) / sinAngle;
	}

	// Use the shortest path
	if( allowFlip && ( cosAngle < 0.0f ) )
	{
		c1 = -c1;
	}

	return Quat4d( c1 * a[ 0 ] + c2 * b[ 0 ], c1 * a[ 1 ] + c2 * b[ 1 ], c1 * a[ 2 ] + c2 * b[ 2 ], c1 * a[ 3 ] + c2 * b[ 3 ] );
}

// static
Quat4d Quat4d::squad( const Quat4d& a, const Quat4d& tanA, const Quat4d& tanB, const Quat4d& b, double t )
{
	Quat4d ab = Quat4d::slerp( a, b, t );
	Quat4d tangent = Quat4d::slerp( tanA, tanB, t, false );
	return Quat4d::slerp( ab, tangent, 2.0f * t * ( 1.0f - t ), false );
}

// static
Quat4d Quat4d::cubicInterpolate( const Quat4d& q0, const Quat4d& q1, const Quat4d& q2, const Quat4d& q3, double t )
{
	// geometric construction:
	//            t
	//   (t+1)/2     t/2
	// t+1        t	        t-1

	// bottom level
	Quat4d q0q1 = Quat4d::slerp( q0, q1, t + 1 );
	Quat4d q1q2 = Quat4d::slerp( q1, q2, t );
	Quat4d q2q3 = Quat4d::slerp( q2, q3, t - 1 );

	// middle level
	Quat4d q0q1_q1q2 = Quat4d::slerp( q0q1, q1q2, 0.5f * ( t + 1 ) );
	Quat4d q1q2_q2q3 = Quat4d::slerp( q1q2, q2q3, 0.5f * t );

	// top level
	return Quat4d::slerp( q0q1_q1q2, q1q2_q2q3, t );
}

// static
Quat4d Quat4d::logDifference( const Quat4d& a, const Quat4d& b )
{
	Quat4d diff = a.inverse() * b;
	diff.normalize();
	return diff.log();
}

// static
Quat4d Quat4d::squadTangent( const Quat4d& before, const Quat4d& center, const Quat4d& after )
{
	Quat4d l1 = Quat4d::logDifference( center, before );
	Quat4d l2 = Quat4d::logDifference( center, after );
	
	Quat4d e;
	for( int i = 0; i < 4; ++i )
	{
		e[ i ] = -0.25f * ( l1[ i ] + l2[ i ] );
	}
	e = center * ( e.exp() );

	return e;
}

// static
Quat4d Quat4d::fromRotationMatrix( const Matrix3d& m )
{
	double x;
	double y;
	double z;
	double w;

	// Compute one plus the trace of the matrix
	double onePlusTrace = 1.0f + m( 0, 0 ) + m( 1, 1 ) + m( 2, 2 );

	if( onePlusTrace > 1e-5 )
	{
		// Direct computation
		double s = sqrt( onePlusTrace ) * 2.0f;
		x = ( m( 2, 1 ) - m( 1, 2 ) ) / s;
		y = ( m( 0, 2 ) - m( 2, 0 ) ) / s;
		z = ( m( 1, 0 ) - m( 0, 1 ) ) / s;
		w = 0.25f * s;
	}
	else
	{
		// Computation depends on major diagonal term
		if( ( m( 0, 0 ) > m( 1, 1 ) ) & ( m( 0, 0 ) > m( 2, 2 ) ) )
		{
			double s = sqrt( 1.0f + m( 0, 0 ) - m( 1, 1 ) - m( 2, 2 ) ) * 2.0f;
			x = 0.25f * s;
			y = ( m( 0, 1 ) + m( 1, 0 ) ) / s;
			z = ( m( 0, 2 ) + m( 2, 0 ) ) / s;
			w = ( m( 1, 2 ) - m( 2, 1 ) ) / s;
		}
		else if( m( 1, 1 ) > m( 2, 2 ) )
		{
			double s = sqrt( 1.0f + m( 1, 1 ) - m( 0, 0 ) - m( 2, 2 ) ) * 2.0f;
			x = ( m( 0, 1 ) + m( 1, 0 ) ) / s;
			y = 0.25f * s;
			z = ( m( 1, 2 ) + m( 2, 1 ) ) / s;
			w = ( m( 0, 2 ) - m( 2, 0 ) ) / s;
		}
		else
		{
			double s = sqrt( 1.0f + m( 2, 2 ) - m( 0, 0 ) - m( 1, 1 ) ) * 2.0f;
			x = ( m( 0, 2 ) + m( 2, 0 ) ) / s;
			y = ( m( 1, 2 ) + m( 2, 1 ) ) / s;
			z = 0.25f * s;
			w = ( m( 0, 1 ) - m( 1, 0 ) ) / s;
		}
	}

	Quat4d q( w, x, y, z );
	return q.normalized();
}

// static
Quat4d Quat4d::fromRotatedBasis( const Vector3d& x, const Vector3d& y, const Vector3d& z )
{
	return fromRotationMatrix( Matrix3d( x, y, z ) );
}

// static
Quat4d Quat4d::randomRotation( double u0, double u1, double u2 )
{
	double z = u0;
	double theta = static_cast< double >( 2.f * M_PI * u1 );
	double r = sqrt( 1.f - z * z );
	double w = static_cast< double >( M_PI * u2 );

	return Quat4d
	(
		cos( w ),
		sin( w ) * cos( theta ) * r,
		sin( w ) * sin( theta ) * r,
		sin( w ) * z
	);
}

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////

Quat4d operator + ( const Quat4d& q0, const Quat4d& q1 )
{
	return Quat4d
	(
		q0.w() + q1.w(),
		q0.x() + q1.x(),
		q0.y() + q1.y(),
		q0.z() + q1.z()
	);
}

Quat4d operator - ( const Quat4d& q0, const Quat4d& q1 )
{
	return Quat4d
	(
		q0.w() - q1.w(),
		q0.x() - q1.x(),
		q0.y() - q1.y(),
		q0.z() - q1.z()
	);
}

Quat4d operator * ( const Quat4d& q0, const Quat4d& q1 )
{
	return Quat4d
	(
		q0.w() * q1.w() - q0.x() * q1.x() - q0.y() * q1.y() - q0.z() * q1.z(),
		q0.w() * q1.x() + q0.x() * q1.w() + q0.y() * q1.z() - q0.z() * q1.y(),
		q0.w() * q1.y() - q0.x() * q1.z() + q0.y() * q1.w() + q0.z() * q1.x(),
		q0.w() * q1.z() + q0.x() * q1.y() - q0.y() * q1.x() + q0.z() * q1.w()
	);
}

Quat4d operator * ( double f, const Quat4d& q )
{
	return Quat4d
	(
		f * q.w(),
		f * q.x(),
		f * q.y(),
		f * q.z()
	);
}

Quat4d operator * ( const Quat4d& q, double f )
{
	return Quat4d
	(
		f * q.w(),
		f * q.x(),
		f * q.y(),
		f * q.z()
	);
}
