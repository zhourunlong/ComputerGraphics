#ifndef VECTOR_3d_H
#define VECTOR_3d_H

#include <iostream>

class Vector2d;

class Vector3d
{
public:

	static const Vector3d ZERO;
	static const Vector3d UP;
	static const Vector3d RIGHT;
	static const Vector3d FORWARD;

    Vector3d( double f = 0.f );
    Vector3d( double x, double y, double z );

	Vector3d( const Vector2d& xy, double z );
	Vector3d( double x, const Vector2d& yz );

	// copy constructors
    Vector3d( const Vector3d& rv );

	// assignment operators
    Vector3d& operator = ( const Vector3d& rv );

	// no destructor necessary

	// returns the ith element
    const double& operator [] ( int i ) const;
    double& operator [] ( int i );

    double& x();
	double& y();
	double& z();

	double x() const;
	double y() const;
	double z() const;

	Vector2d xy() const;
	Vector2d xz() const;
	Vector2d yz() const;

	Vector3d xyz() const;
	Vector3d yzx() const;
	Vector3d zxy() const;

	double length() const;
    double squaredLength() const;

	void normalize();
	Vector3d normalized() const;

	Vector2d homogenized() const;

	void negate();

	// ---- Utility ----
    operator const double* () const; // automatic type conversion for OpenGL
    operator double* (); // automatic type conversion for OpenGL 
	void print() const;	

	Vector3d& operator += ( const Vector3d& v );
	Vector3d& operator -= ( const Vector3d& v );
    Vector3d& operator *= ( double f );

    static double dot( const Vector3d& v0, const Vector3d& v1 );
	static Vector3d cross( const Vector3d& v0, const Vector3d& v1 );
    
    // computes the linear interpolation between v0 and v1 by alpha \in [0,1]
	// returns v0 * ( 1 - alpha ) * v1 * alpha
	static Vector3d lerp( const Vector3d& v0, const Vector3d& v1, double alpha );

	// computes the cubic catmull-rom interpolation between p0, p1, p2, p3
    // by t \in [0,1].  Guarantees that at t = 0, the result is p0 and
    // at p1, the result is p2.
	static Vector3d cubicInterpolate( const Vector3d& p0, const Vector3d& p1, const Vector3d& p2, const Vector3d& p3, double t );

private:

	double m_elements[ 3 ];

};

// component-wise operators
Vector3d operator + ( const Vector3d& v0, const Vector3d& v1 );
Vector3d operator - ( const Vector3d& v0, const Vector3d& v1 );
Vector3d operator * ( const Vector3d& v0, const Vector3d& v1 );
Vector3d operator / ( const Vector3d& v0, const Vector3d& v1 );

// unary negation
Vector3d operator - ( const Vector3d& v );

// multiply and divide by scalar
Vector3d operator * ( double f, const Vector3d& v );
Vector3d operator * ( const Vector3d& v, double f );
Vector3d operator / ( const Vector3d& v, double f );

bool operator == ( const Vector3d& v0, const Vector3d& v1 );
bool operator != ( const Vector3d& v0, const Vector3d& v1 );

inline std::ostream &operator<<(std::ostream &os, const Vector3d &v) {
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}

#endif // VECTOR_3d_H
