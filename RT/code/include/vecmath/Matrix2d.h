#pragma once

#include <cstdio>

class Vector2d;

// 2x2 Matrix, stored in column major order (OpenGL style)
class Matrix2d
{
public:

    // Fill a 2x2 matrix with "fill", default to 0.
	Matrix2d( double fill = 0.f );
	Matrix2d( double m00, double m01,
		double m10, double m11 );

	// setColumns = true ==> sets the columns of the matrix to be [v0 v1]
	// otherwise, sets the rows
	Matrix2d( const Vector2d& v0, const Vector2d& v1, bool setColumns = true );

	Matrix2d( const Matrix2d& rm ); // copy constructor
	Matrix2d& operator = ( const Matrix2d& rm ); // assignment operator
	// no destructor necessary

	const double& operator () ( int i, int j ) const;
	double& operator () ( int i, int j );

	Vector2d getRow( int i ) const;
	void setRow( int i, const Vector2d& v );

	Vector2d getCol( int j ) const;
	void setCol( int j, const Vector2d& v );

	double determinant();
	Matrix2d inverse( bool* pbIsSingular = NULL, double epsilon = 0.f );

	void transpose();
	Matrix2d transposed() const;

	// ---- Utility ----
	operator double* (); // automatic type conversion for GL
	void print();

	static double determinant2x2( double m00, double m01,
		double m10, double m11 );

	static Matrix2d ones();
	static Matrix2d identity();
	static Matrix2d rotation( double degrees );

private:

	double m_elements[ 4 ];

};

// Scalar-Matrix multiplication
Matrix2d operator * ( double f, const Matrix2d& m );
Matrix2d operator * ( const Matrix2d& m, double f );

// Matrix-Vector multiplication
// 2x2 * 2x1 ==> 2x1
Vector2d operator * ( const Matrix2d& m, const Vector2d& v );

// Matrix-Matrix multiplication
Matrix2d operator * ( const Matrix2d& x, const Matrix2d& y );
