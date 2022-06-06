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

class Ray {
public:
	Vec o, d; // origin, direction
	Ray(const Vec &o, const Vec &d): o(o), d(d) { this->d.normalize(); }
};

class Color {
public:
	Vec c;
	Color(const Vec &c) : c(c) {}
	Color(double r, double g, double b) : c(r,g,b) {}
	Color operator * (double opacity) { return Color(c*opacity); }
	void get(ofstream &file) { file << c.x << " " << c.y << " " << c.z << "\n"; }
};

class Sphere {
public:
	Vec c;
	double r;
	Color color;
	Sphere(const Vec &c, double r, const Color &color) : c(c), r(r), color(color) {}
	bool inside(Vec point) const { return (point-c)*(point-c) <= sqr(r); };
};

const int H = 1000;
const int W = 1000;
Color white(0xff, 0xff, 0xff);
Color black(0, 0, 0);
Color red(0xff, 0, 0);
Color green(0, 0xff, 0);
Color blue(0, 0, 0xff);


int main() {
	ofstream ppm("pixel_map.ppm");
	ppm << "P3\n" << W << "\n" << H << "\n" << "255\n";

	Color AmbientLight = white*0.2;
	Sphere sph(Vec(0,-80,250), 200.0, red);
	for (int y = 500; y > -500; y--) {
		for (int x = -500; x < 500; x++) {
			Color col = AmbientLight;
			if (sph.inside(Vec(x, y, sph.c.z))) col = red;
			col.get(ppm);
		}
	}
}

// Scene:

// Camera : o(0, 0, 0) d(0, 0, 1)
// Window : c(0, 0, 100) 
// Ambiant light: 	0.2  255, 255, 255
// Shpere: c(0, -80, 250)  r = 200  Color =  red