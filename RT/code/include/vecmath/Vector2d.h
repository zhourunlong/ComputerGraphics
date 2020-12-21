#pragma once

#include <cmath>

class Vector3d;

class Vector2d
{
public:
    
    static const Vector2d ZERO;
	static const Vector2d UP;
	static const Vector2d RIGHT;

    Vector2d( double f = 0.f );
    Vector2d( double x, double y );

	// copy constructors
    Vector2d( const Vector2d& rv );

	// assignment operators
	Vector2d& operator = ( const Vector2d& rv );

	// no destructor necessary

	// returns the ith element
    const double& operator [] ( int i ) const;
	double& operator [] ( int i );

    double& x();
	double& y();

	double x() const;
	double y() const;

    Vector2d xy() const;
	Vector2d yx() const;
	Vector2d xx() const;
	Vector2d yy() const;

	// returns ( -y, x )
    Vector2d normal() const;

    double abs() const;
    double absSquared() const;
    void normalize();
    Vector2d normalized() const;

    void negate();

	// ---- Utility ----
    operator const double* () const; // automatic type conversion for OpenGL 
    operator double* (); // automatic type conversion for OpenGL 
	void print() const;

	Vector2d& operator += ( const Vector2d& v );
	Vector2d& operator -= ( const Vector2d& v );
	Vector2d& operator *= ( double f );

    static double dot( const Vector2d& v0, const Vector2d& v1 );

	static Vector3d cross( const Vector2d& v0, const Vector2d& v1 );

	// returns v0 * ( 1 - alpha ) * v1 * alpha
	static Vector2d lerp( const Vector2d& v0, const Vector2d& v1, double alpha );

private:

	double m_elements[2];

};

// component-wise operators
Vector2d operator + ( const Vector2d& v0, const Vector2d& v1 );
Vector2d operator - ( const Vector2d& v0, const Vector2d& v1 );
Vector2d operator * ( const Vector2d& v0, const Vector2d& v1 );
Vector2d operator / ( const Vector2d& v0, const Vector2d& v1 );

// unary negation
Vector2d operator - ( const Vector2d& v );

// multiply and divide by scalar
Vector2d operator * ( double f, const Vector2d& v );
Vector2d operator * ( const Vector2d& v, double f );
Vector2d operator / ( const Vector2d& v, double f );

bool operator == ( const Vector2d& v0, const Vector2d& v1 );
bool operator != ( const Vector2d& v0, const Vector2d& v1 );
