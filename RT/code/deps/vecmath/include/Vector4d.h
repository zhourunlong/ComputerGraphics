#ifndef VECTOR_4d_H
#define VECTOR_4d_H

class Vector2d;
class Vector3d;

class Vector4d
{
public:

	Vector4d( double f = 0.f );
	Vector4d( double fx, double fy, double fz, double fw );
	Vector4d( double buffer[ 4 ] );

	Vector4d( const Vector2d& xy, double z, double w );
	Vector4d( double x, const Vector2d& yz, double w );
	Vector4d( double x, double y, const Vector2d& zw );
	Vector4d( const Vector2d& xy, const Vector2d& zw );

	Vector4d( const Vector3d& xyz, double w );
	Vector4d( double x, const Vector3d& yzw );

	// copy constructors
	Vector4d( const Vector4d& rv );

	// assignment operators
	Vector4d& operator = ( const Vector4d& rv );

	// no destructor necessary

	// returns the ith element
	const double& operator [] ( int i ) const;
	double& operator [] ( int i );

	double& x();
	double& y();
	double& z();
	double& w();

	double x() const;
	double y() const;
	double z() const;
	double w() const;

	Vector2d xy() const;
	Vector2d yz() const;
	Vector2d zw() const;
	Vector2d wx() const;

	Vector3d xyz() const;
	Vector3d yzw() const;
	Vector3d zwx() const;
	Vector3d wxy() const;

	Vector3d xyw() const;
	Vector3d yzx() const;
	Vector3d zwy() const;
	Vector3d wxz() const;

	double abs() const;
	double absSquared() const;
	void normalize();
	Vector4d normalized() const;

	// if v.z != 0, v = v / v.w
	void homogenize();
	Vector4d homogenized() const;

	void negate();

	// ---- Utility ----
	operator const double* () const; // automatic type conversion for OpenGL
	operator double* (); // automatic type conversion for OpenG
	void print() const; 

	static double dot( const Vector4d& v0, const Vector4d& v1 );
	static Vector4d lerp( const Vector4d& v0, const Vector4d& v1, double alpha );

private:

	double m_elements[ 4 ];

};

// component-wise operators
Vector4d operator + ( const Vector4d& v0, const Vector4d& v1 );
Vector4d operator - ( const Vector4d& v0, const Vector4d& v1 );
Vector4d operator * ( const Vector4d& v0, const Vector4d& v1 );
Vector4d operator / ( const Vector4d& v0, const Vector4d& v1 );

// unary negation
Vector4d operator - ( const Vector4d& v );

// multiply and divide by scalar
Vector4d operator * ( double f, const Vector4d& v );
Vector4d operator * ( const Vector4d& v, double f );
Vector4d operator / ( const Vector4d& v, double f );

bool operator == ( const Vector4d& v0, const Vector4d& v1 );
bool operator != ( const Vector4d& v0, const Vector4d& v1 );

#endif // VECTOR_4d_H
