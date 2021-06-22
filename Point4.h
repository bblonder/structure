#ifndef _Point4_H_
#define _Point4_H_


class Point4
{
	public:
		Point4();
		Point4(float p1, float p2, float p3);
		Point4(float p1, float p2, float p3, float p4);
		
		float mag();
		float mag2();
		Point4 norm();
		
		float x;
		float y;
		float z;
		float a;
};

Point4 operator+(const Point4& p1, const Point4& p2);
Point4 operator*(const float p1, const Point4& p2);
Point4 operator-(const Point4& p1, const Point4& p2);
Point4 operator/(const Point4& p1, const float p2);

#endif