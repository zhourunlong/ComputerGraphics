#ifndef MATRIX4d_H
#define MATRIX4d_H

#include <cstdio>

class Matrix2d;
class Matrix3d;
class Quat4d;
class Vector3d;
class Vector4d;

// 4x4 Matrix, stored in column major order (OpenGL style)
class Matrix4d
{
public:

    // Fill a 4x4 matrix with "fill".  Default to 0.
	Matrix4d( double fill = 0.f );
	Matrix4d( double m00, double m01, double m02, double m03,
		double m10, double m11, double m12, double m13,
		double m20, double m21, double m22, double m23,
		double m30, double m31, double m32, double m33 );
	
	// setColumns = true ==> sets the columns of the matrix to be [v0 v1 v2 v3]
	// otherwise, sets the rows
	Matrix4d( const Vector4d& v0, const Vector4d& v1, const Vector4d& v2, const Vector4d& v3, bool setColumns = true );
	
	Matrix4d( const Matrix4d& rm ); // copy constructor
	Matrix4d& operator = ( const Matrix4d& rm ); // assignment operator
	Matrix4d& operator/=(double d);
	// no destructor necessary

	const double& operator () ( int i, int j ) const;
	double& operator () ( int i, int j );

	Vector4d getRow( int i ) const;
	void setRow( int i, const Vector4d& v );

	// get column j (mod 4)
	Vector4d getCol( int j ) const;
	void setCol( int j, const Vector4d& v );

	// gets the 2x2 submatrix of this matrix to m
	// starting with upper left corner at (i0, j0)
	Matrix2d getSubmatrix2x2( int i0, int j0 ) const;

	// gets the 3x3 submatrix of this matrix to m
	// starting with upper left corner at (i0, j0)
	Matrix3d getSubmatrix3x3( int i0, int j0 ) const;

	// sets a 2x2 submatrix of this matrix to m
	// starting with upper left corner at (i0, j0)
	void setSubmatrix2x2( int i0, int j0, const Matrix2d& m );

	// sets a 3x3 submatrix of this matrix to m
	// starting with upper left corner at (i0, j0)
	void setSubmatrix3x3( int i0, int j0, const Matrix3d& m );

	double determinant() const;
	Matrix4d inverse( bool* pbIsSingular = NULL, double epsilon = 0.f ) const;

	void transpose();
	Matrix4d transposed() const;

	// ---- Utility ----
	operator double* (); // automatic type conversion for GL
	operator const double* () const; // automatic type conversion for GL
	
	void print();

	static Matrix4d ones();
	static Matrix4d identity();
	static Matrix4d translation( double x, double y, double z );
	static Matrix4d translation( const Vector3d& rTranslation );
	static Matrix4d rotateX( double radians );
	static Matrix4d rotateY( double radians );
	static Matrix4d rotateZ( double radians );
	static Matrix4d rotation( const Vector3d& rDirection, double radians );
	static Matrix4d scaling( double sx, double sy, double sz );
	static Matrix4d uniformScaling( double s );
	static Matrix4d lookAt( const Vector3d& eye, const Vector3d& center, const Vector3d& up );
	static Matrix4d orthographicProjection( double width, double height, double zNear, double zFar, bool directX );
	static Matrix4d orthographicProjection( double left, double right, double bottom, double top, double zNear, double zFar, bool directX );
	static Matrix4d perspectiveProjection( double fLeft, double fRight, double fBottom, double fTop, double fZNear, double fZFar, bool directX );
	static Matrix4d perspectiveProjection( double fovYRadians, double aspect, double zNear, double zFar, bool directX );
	static Matrix4d infinitePerspectiveProjection( double fLeft, double fRight, double fBottom, double fTop, double fZNear, bool directX );

	// Returns the rotation matrix represented by a quaternion
	// uses a normalized version of q
	static Matrix4d rotation( const Quat4d& q );

	// returns an orthogonal matrix that's a uniformly distributed rotation
	// given u[i] is a uniformly distributed random number in [0,1]
	static Matrix4d randomRotation( double u0, double u1, double u2 );

private:

	double m_elements[ 16 ];

};

// Matrix-Vector multiplication
// 4x4 * 4x1 ==> 4x1
Vector4d operator * ( const Matrix4d& m, const Vector4d& v );

// Matrix-Matrix multiplication
Matrix4d operator * ( const Matrix4d& x, const Matrix4d& y );

#endif // MATRIX4d_H
