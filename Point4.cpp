#include <math.h>
#include "Point4.h"

Point4::Point4()
{
	x = 0;
	y = 0;
	z = 0;
	a = 0;
}

Point4::Point4(float p1, float p2, float p3)
{
	x = p1;
	y = p2;
	z = p3;
	a = 0;
}

Point4::Point4(float p1, float p2, float p3, float p4)
{
	x = p1;
	y = p2;
	z = p3;
	a = p4;
}

float Point4::mag() 
{
	return sqrt(mag2());
}

float Point4::mag2() 
{
	return (x*x + y*y + z*z + a*a);
}

Point4 Point4::norm()
{
	float m = mag();
	if (m == 0) return Point4();
	else return (*this)/m;
}

Point4 operator+(const Point4& p1, const Point4& p2)
{
	return Point4(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z, p1.a + p2.a);
}

Point4 operator*(const float p1, const Point4& p2)
{
	return Point4(p1*p2.x, p1*p2.y, p1*p2.z, p1*p2.a);
}

Point4 operator-(const Point4& p1, const Point4& p2)
{
	return p1 + -1*p2;
}

Point4 operator/(const Point4& p1, const float p2)
{
	return (1.0/p2)*p1;
}