#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

template<class T>
double sqr(const T &x) {
	return x*x;
}

template<class T1, class T2>
double _dist(const T1 &a, const T2 &b) {
	return sqrt(sqr(a.x-b.x)+sqr(a.y-b.y)+sqr(a.z-b.z));
}

class Vec {
public:
	double x, y, z;
	Vec(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
	Vec(const Vec &v) : x(v.x), y(v.y),z(v.z) {}
	Vec operator + (const Vec &v) { return Vec(x+v.x, y+v.y, z+v.z); };
	Vec operator - (const Vec &v) { return Vec(x-v.x, y-v.y, z-v.z); };
	Vec operator / (double f) { return Vec(x/f, y/f, z/f); };
	double mag() { return _dist(*this, Vec(0,0,0)); }
	Vec unity() { return *this/mag(); }
	Vec operator * (double f) { return Vec(x*f, y*f, z*f); };
	double operator * (const Vec &v) { return x*v.x + y*v.y + z*v.z; } // dot
	void normalize() { *this = unity(); }
};

// Scene:

// Camera : o(0, 0, 0) d(0, 0, 1)
// Window : c(0, 0, 100) 
// Ambiant light: 	0.2  255, 255, 255
// Shpere: c(0, -80, 250)  r = 200  Color =  red