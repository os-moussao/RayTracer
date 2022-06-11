#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

// #define double long double

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
	Vec normalize() { return *this = unity(); }
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
	Color operator + (Color &col2) { return Color(c+col2.c); }
	Color operator - (Color &col2) { return Color(c-col2.c); }
	double  normalize(double &rgb) { return rgb = rgb < 0? 0: rgb > 0xff? 0xff: rgb; }
	void get(ofstream &file) {
		normalize(c.x); normalize(c.y); normalize(c.z);
		file << c.x << " " << c.y << " " << c.z << "\n";
	}
};

class Light {
public:
	Vec o;
	Color color;
	Light(): color(0, 0, 0) {}
	Light(const Vec &o, double brightness = 0) : o(o), color(0, 0, 0) { color = Color(0xff,0xff,0xff) * brightness; }
};

class Sphere {
public:
	Vec c;
	double r;
	Color color;
	Sphere(const Vec &c, double r, const Color &color) : c(c), r(r), color(color) {}
	bool inside(Vec point) const { return (point-c)*(point-c) <= sqr(r); };
	bool intersects(Ray &ray, double &t) {
		double B = 2 * ((ray.o - c) * ray.d);
		double C = (ray.o-c)*(ray.o-c) - sqr(r);
		double A = ray.d*ray.d;
		double delta = sqr(B) - 4*A*C;
		if (delta < 0) return false;
		delta = sqrt(delta);
		t = min((-B - delta) / (2 * A), (-B + delta) / (2 * A)); // the closest intersection
		return true;
	}
};

const int H = 800;
const int W = 800;
const double WindowZ = 100.0;

Color white(0xff, 0xff, 0xff);
Color black(0, 0, 0);
Color red(0xff, 0, 0);
Color green(0, 0xff, 0);
Color blue(0, 0, 0xff);

int main() {
	ofstream ppm("pixel_map.ppm");
	ppm << "P3\n" << W << "\n" << H << "\n" << "255\n";

	Light light(Vec(-W/4, H/4, 100), 1.5);
	Vec camera(0, 0, 0);
	Color AmbientLight = white*0.2;
	Sphere sph(Vec(0,0,200), 150, red);
	for (int y = H/2; y > -H/2; y--) {
		for (int x = -W/2; x < W/2; x++) {
			Vec pixel(x, y, WindowZ);
			Color col = AmbientLight;
			Ray ray(pixel, pixel-camera);

			// check intersection
			double t;
			if (sph.intersects(ray, t)) {
				Vec p = ray.o + ray.d*t; // point of intersection
				Vec L = (light.o - p).normalize();
				Vec N = (p - sph.c).normalize();
				double _cos = L*N; // dot of unity vectors = cos
				_cos -= 0.15;
				col = light.color * _cos;
				col = sph.color + col; 
			}
			if (_dist(Vec(x, y, light.o.z), light.o) <= 10) white.get(ppm);
			else col.get(ppm);
		}
	}
}

// Scene:

// Camera : o(0, 0, 0) d(0, 0, 1)
// Window : c(0, 0, 100)
// Light : o(-200, 500, 200)   brightness(0.8)
// Ambiant light: 	0.2  255, 255, 255
// Shpere: c(0, -80, 250)  r = 125  Color = red