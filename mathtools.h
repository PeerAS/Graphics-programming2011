#ifndef MATHTOOLS_H
#define MATHTOOLS_H
#include <math.h>

struct Vector3D
{
	float x, y, z;
	Vector3D(float _x = 0.0, float _y = 0.0, float _z = 0.0)
	{x = _x; y = _y; z = _z;}
	void set(float _x, float _y, float _z)
	{x = _x; y = _y; z = _z;}
	void set(const Vector3D & v)
	{x = v.x; y = v.y; z = v.z;}

	Vector3D &operator+=(const Vector3D &v)
	{x += v.x; y += v.y; z += v.z; return *this;}

	Vector3D &operator-=(const Vector3D &v)
	{x -= v.x; y -= v.y; z -= v.z; return *this;}

	Vector3D &operator/=(const float f)
	{x /= f; y /= f; z /= f; return *this;}

	Vector3D &operator*=(const float f)
	{x *= f; y *= f; z *= f; return *this;}

	Vector3D &operator=(const Vector3D &v)
	{x = v.x; y = v.y; z = v.z; return *this;}

	Vector3D operator+(const Vector3D & v) const
	{return Vector3D(x + v.x, y + v.y, z + v.z);}

	Vector3D operator-(const Vector3D & v) const
	{return Vector3D(x - v.x, y - v.y, z - v.z);}
	Vector3D & operator-()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this; 
	}

	float operator*(const Vector3D & v) const
	{return x * v.x + y * v.y + z * v.z;} 
	float dot(const Vector3D & v) const
	{return x * v.x + y * v.y + z * v.z;} 

	Vector3D cross(const Vector3D & v) const
	{
		return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	Vector3D operator/(const float a) const
	{
		float ia = 1.0f / a;
		return Vector3D(x * ia, y * ia, z * ia); 
	} 

	Vector3D operator*(const float a) const
	{return Vector3D(x * a, y * a, z * a);} 
	friend Vector3D operator*(const float a, const Vector3D &v)
	{return Vector3D(a * v.x, a * v.y, a * v.z);}

	float length() const
	{return sqrt(x * x + y * y + z * z);}

	Vector3D hat() 
	{
		Vector3D ret;
		float idiv = 1.0f / sqrt(x * x + y * y + z * z);
		ret.x = x * idiv;
		ret.y = y * idiv;
		ret.z = z * idiv;
		return ret;
	}

	void normalize() 
	{
		float idiv = 1.0f / sqrt(x * x + y * y + z * z);
		x *= idiv;
		y *= idiv;
		z *= idiv;
	}
};

struct Point3D
{
	float x, y, z;
	Point3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
	{x = _x; y = _y; z = _z;}
	Point3D(float * pos)
	{x = *pos; y = *(pos + 1); z = *(pos + 2);}
	void set(float _x, float _y, float _z)
	{x = _x; y = _y; z = _z;}
	void set(const Point3D & p)
	{x = p.x; y = p.y; z = p.z;}
	Point3D & operator=(const Point3D & p)
	{x = p.x; y = p.y; z = p.z; return *this;}
	Point3D operator*(const float m) const
	{return Point3D(x * m, y * m, z * m);}
	friend Point3D operator*(const float m, const Point3D & p)
	{return Point3D(m * p.x, m * p.y, m * p.z);}

	Vector3D operator-(const Point3D & p) const
	{return Vector3D(x - p.x, y - p.y, z - p.z);}
	//omtyping
	operator Vector3D() const
	{return Vector3D(x, y, z);}

	float pdot(const Vector3D & p) const
	{return x * p.x + y * p.y + z * p.z;}

	Point3D operator-(const Vector3D & v) const
	{return Point3D(x - v.x, y - v.y, z - v.z);}
	Point3D operator+(const Vector3D & v) const
	{return Point3D(x + v.x, y + v.y, z + v.z);}

	Point3D operator+=(const Vector3D &v)
	{x += v.x; y += v.y; z += v.z; return *this;}
//	Point3D &operator+=(const Point3D &p)
//	{x += p.x; y += p.y; z += p.z; return *this;}

	Point3D &operator*=(const float f)
	{x *= f; y *= f; z *= f; return *this;}
	Point3D &operator/=(const float f)//for affine sum
	{x /= f; y /= f; z /= f; return *this;}
} ;

#endif