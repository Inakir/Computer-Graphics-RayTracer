#ifndef POLYGON_DRAWER_H
#define POLYGON_DRAWER_H

#include <vector>
#include "ScanConvert.h"

using namespace std;

class normalized_vec
{
public:
	float x, y, z, mag;
	
	normalized_vec() {};
	normalized_vec(float nx, float ny, float nz)
	{
		float magnitude = sqrt(nx*nx + ny*ny + nz*nz);
		x = nx / magnitude;
		y = ny / magnitude;
		z = nz / magnitude;
	}

	normalized_vec(float nx, float ny, float nz, bool checker)
	{
		x = nx;
		y = ny;
		z = nz;
	}

	normalized_vec mult(float m)
	{
		return normalized_vec(x*m, y*m, z*m);
	}

	void normalize()
	{
		float magnitude = sqrt(x*x + y*y + z*z);
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}

	void invert()
	{
		x = -x;
		y = -y;
		z = -z;
	}
};


class Pt
{
public:
	float x, y, z;
	Pt() {};
	Pt(float nx, float ny, float nz)
	{
		x = nx;
		y = ny;
		z = nz;
	}

	void offset(float m, normalized_vec n)
	{
		x *= m*n.x;
		y *= m*n.y;
		z *= m*n.z;
	}

	void convert(float h, float w)
	{
		x = x*2/(w-1) - 1;
		y = -y*2/(h-1) + 1;
		z = 1;
	}

	void moveIn(float m) { z += m; }
	void moveOut(float m) { z -= m; }

	void moveUp(float m) { y += m; }
	void moveDown(float m) { y -= m; }

	void moveLeft(float m) { x += m; }
	void moveRight(float m) { x -= m; }
};


float dotProd(normalized_vec a, normalized_vec b);
float dotProd(normalized_vec a, Pt b);
float dotProd(Pt a, normalized_vec b);
float dotProd(Pt a, Pt b);
float getDistance(Pt a, Pt b);
void setColor(float &r, float &g, float &b, Pt intersection, normalized_vec L, normalized_vec N, normalized_vec E, Pt ka, Pt A, Pt kd, Pt ks, float Intensity);
void getDir(Pt origin, Pt end, normalized_vec &dir);
void getDir(Pt origin, Pt end, normalized_vec &dir, bool check);
void closest(Pt p, Pt a, Pt b, Pt &intersection);

class infintePlane
{
public:
	Pt o;
	normalized_vec n;
	Pt ka;		//ambient light coefficient
	Pt A;			//intensity of ambient light
	Pt kd;		//diffuse Reflection coefficient
	Pt ks;		//specular Reflection coefficient
	float R;		//Reflection coefficient

	infintePlane(){}

	infintePlane(float px, float py, float pz, float nx, float ny, float nz, Pt KA, Pt i, Pt KD, Pt KS, float r)
	{
		o = Pt(px, py, pz);
		n = normalized_vec(nx, ny, nz);

		ka = KA;
		A = i;
		kd = KD;
		ks = KS;
		R = r;
	}

	infintePlane(Pt to, normalized_vec tn, Pt KA, Pt i, Pt KD, Pt KS, float RR)
	{
		o = to;
		n = tn;

		ka = KA;
		A = i;
		kd = KD;
		ks = KS;
		R = RR;
	}

	void getIntersect(Pt p, normalized_vec v, bool &intersect, Pt &intersection, float &t_val)
	{
		Pt o_p(o.x - p.x, o.y - p.y, o.z - p.z);

		float t = dotProd(n, o_p) / dotProd(n, v);
		if (t <= 0)
		{
			intersect = false;
			t_val = -1;
		}
		else
		{
			intersect = true;
			float fx = p.x + v.x*t;
			float fy = p.y + v.y*t;
			float fz = p.z + v.z*t;

			intersection.x = fx;
			intersection.y = fy;
			intersection.z = fz;

			t_val = t;
		}
	}
};

class sphere 
{
public:
	Pt c;
	double radius;
	Pt ka;		//ambient light coefficient
	Pt A;			//intensity of ambient light
	Pt kd;		//diffuse Reflection coefficient
	Pt ks;		//specular Reflection coefficient
	float R;		//Reflection coefficient


	sphere(float cx, float cy, float cz, float r, Pt KA, Pt i, Pt KD, Pt KS, float RR)
	{
		c = Pt(cx, cy, cz);
		radius = r;

		ka = KA;
		A = i;
		kd = KD;
		ks = KS;
		R = RR;
	}

	void getIntersect(Pt p, normalized_vec dir, bool &intersect, Pt &intersection, float &t_val)
	{
		Pt p_c(p.x - c.x, p.y - c.y, p.z - c.z);

		float a = dotProd(dir, dir);
		float b = 2 * dotProd(dir, p_c);
		float c = dotProd(p_c, p_c) - radius*radius;

		float check = b*b - 4 * a*c;

		if (check < 0) {
			intersect = false;
			t_val = -1;
		}
		else
		{
			intersect = true;
			float t_minus = (-b - sqrt(check)) / (2 * a);
			float t_plus =  (-b + sqrt(check)) / (2 * a);

			if (t_minus > 0)
			{
				intersection.x = p.x + dir.x*t_minus;
				intersection.y = p.y + dir.y*t_minus;
				intersection.z = p.z + dir.z*t_minus; 
				t_val = t_minus;
			}
			else
			{
				intersection.x = p.x + dir.x*t_plus;
				intersection.y = p.y + dir.y*t_plus;
				intersection.z = p.z + dir.z*t_plus;
				t_val = t_plus;
			}
			
		}
	}
};

class infinteCylinder 
{
public:
	Pt c;
	normalized_vec direction;
	double radius; 
	Pt ka;		//ambient light coefficient
	Pt A;			//intensity of ambient light
	Pt kd;		//diffuse Reflection coefficient
	Pt ks;		//specular Reflection coefficient
	float R;		//Reflection coefficient
	infintePlane plane;


	infinteCylinder(float cx, float cy, float cz, double r, float dx, float dy, float dz, Pt KA, Pt i, Pt KD, Pt KS, float RR)
	{
		c = Pt(cx, cy, cz);
		direction = normalized_vec(dx, dy, dz, false);
		radius = r;
		
		ka = KA;
		A = i;
		kd = KD;
		ks = KS;
		R = RR;		//Reflection coefficient

		plane = infintePlane(c, direction, ka, A, kd, ks, R);
	}

	void getIntersect(Pt p, normalized_vec v, bool &intersect, Pt &intersection, float &t_val)
	{
		Pt phat;
		normalized_vec vhat;

		phat.x = p.x;
		phat.y = c.y;
		phat.z = p.z;

		vhat.x = v.x;
		vhat.y = 0;
		vhat.z = v.z;

		Pt p_c(phat.x - c.x, phat.y - c.y, phat.z - c.z);
		float a = dotProd(vhat, vhat);
		float b = 2 * dotProd(vhat, p_c);
		float c = dotProd(p_c, p_c) - radius*radius;

		float check = b*b - 4 * a*c;

		if (check < 0) {
			intersect = false;
			t_val = -1;
		}
		else
		{
			float t_plus = (-b + sqrt(check)) / (2 * a);

			if (check == 0) {
				intersection.x = p.x + vhat.x*t_plus;
				intersection.y = p.y + vhat.y*t_plus;
				intersection.z = p.z + vhat.z*t_plus;

				t_val = t_plus;
				intersect = true;
			}
			else {
				float t_minus = (-b - sqrt(check)) / (2 * a);
				float ax = p.x + vhat.x*t_plus;
				float ay = p.y + vhat.y*t_plus;
				float az = p.z + vhat.z*t_plus;
				Pt aPt(ax, ay, az);

				float bx = p.x + vhat.x*t_minus;
				float by = p.y + vhat.y*t_minus;
				float bz = p.z + vhat.z*t_minus;
				Pt bPt(bx, by, bz);

				closest(p, aPt, bPt, intersection);
				t_val = t_minus;

				intersect = true;
			}
		}
	}
};

class ellipsoid
{
public:
	Pt c;
	double radius;
	Pt ka;		//ambient light coefficient
	Pt A;			//intensity of ambient light
	Pt kd;		//diffuse Reflection coefficient
	Pt ks;		//specular Reflection coefficient
	float R;

	float ea = 3;
	float eb = 1;
	float ec = 1;
	ellipsoid(float cx, float cy, float cz, float r, Pt KA, Pt i, Pt KD, Pt KS, float RR)
	{
		c = Pt(cx, cy, cz);
		radius = r;

		ka = KA;
		A = i;
		kd = KD;
		ks = KS;
		R = RR;
	}

	void getIntersect(Pt p, normalized_vec dir, bool &intersect, Pt &intersection, float &t_val)
	{
		Pt p_c(p.x - c.x, p.y - c.y, p.z - c.z);

		float a = dotProd(dir, dir)/(ea*ea);
		float b = 2 * dotProd(dir, p_c);
		float c = dotProd(p_c, p_c);

		float check = b*b - 4 * a*c;

		if (check < 0) {
			intersect = false;
		}
		else
		{
			float t_plus = (-b + sqrt(check)) / (2 * a);

			if (check == 0) {
				intersection.x = p.x + dir.x*t_plus;
				intersection.y = p.y + dir.y*t_plus;
				intersection.z = p.z + dir.z*t_plus;

				t_val = t_plus;
				intersect = true;
			}
			else {
				float t_minus = (-b - sqrt(check)) / (2 * a);
				float ax = p.x + dir.x*t_plus;
				float ay = p.y + dir.y*t_plus;
				float az = p.z + dir.z*t_plus;
				Pt aPt(ax, ay, az);

				float bx = p.x + dir.x*t_minus;
				float by = p.y + dir.y*t_minus;
				float bz = p.z + dir.z*t_minus;
				Pt bPt(bx, by, bz);

				closest(p, aPt, bPt, intersection);
				t_val = t_minus;

				intersect = true;
			}
		}
		if (t_val < 0 || t_val > 1)
		{
			//intersect = false;
		}
	}
};

#endif