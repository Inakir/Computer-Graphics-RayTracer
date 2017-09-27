#include "PolygonDrawer.h"
#include "ScanConvert.h"
#include <algorithm>
#include <math.h>
#include "ScanConvert.h"

float dotProd(normalized_vec a, normalized_vec b)
{
	float dot = a.x*b.x + a.y*b.y + a.z*b.z;
	return dot;
}

float dotProd(normalized_vec a, Pt b)
{
	float dot = a.x*b.x + a.y*b.y + a.z*b.z;
	return dot;
}

float dotProd(Pt a, normalized_vec b)
{
	float dot = a.x*b.x + a.y*b.y + a.z*b.z;
	return dot;
}

float dotProd(Pt a, Pt b)
{
	float dot = a.x*b.x + a.y*b.y + a.z*b.z;
	return dot;
}

float getDistance(Pt a, Pt b)
{
	return sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z));
}

void setColor(float &r, float &g, float &b, Pt intersection, normalized_vec L, normalized_vec N, normalized_vec E, Pt ka, Pt A, Pt kd, Pt ks,float intensity)
{
	Pt C(1, 1, 1);			//intensity of point light source

	float LN = dotProd(L, N);
	
	float Rx = 2 * LN*N.x - L.x;
	float Ry = 2 * LN*N.y - L.y;
	float Rz = 2 * LN*N.z - L.z;
	normalized_vec R;
	R.x = Rx;
	R.y = Ry;
	R.z = Rz;

	float RE = dotProd(R, E);
	int n = 5;


	//ambience
	r += intensity*ka.x * A.x;
	g += intensity*ka.y * A.y;
	b += intensity*ka.z * A.z;

	if (LN > 0)
	{
		r+=intensity*(C.x) * (kd.x * LN + ks.x*pow(RE, n));
		g+=intensity*(C.y) * (kd.y * LN + ks.y*pow(RE, n));
		b+=intensity*(C.z) * (kd.z * LN + ks.z*pow(RE, n));
	}
}

void normalize(normalized_vec &n)
{
	float magnitude = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
	n.x = n.x / magnitude;
	n.y = n.y / magnitude;
	n.z = n.z / magnitude;
}

void getDir(Pt origin, Pt end, normalized_vec &dir)
{
	float nx = end.x - origin.x;
	float ny = end.y - origin.y;
	float nz = end.z - origin.z;

	float magnitude = sqrt(nx*nx + ny*ny + nz*nz);
	dir.x = nx / magnitude;
	dir.y = ny / magnitude;
	dir.z = nz / magnitude;
}

void getDir(Pt origin, Pt end, normalized_vec &dir, bool check)
{
	float nx = end.x - origin.x;
	float ny = end.y - origin.y;
	float nz = end.z - origin.z;
	
	//float magnitude = sqrt(nx*nx + ny*ny + nz*nz);
	dir.x = nx;
	dir.y = ny;
	dir.z = nz;
}


void closest(Pt p, Pt a, Pt b, Pt &intersection)
{
	float amag = sqrt((a.x - p.x)*(a.x - p.x) + (a.y - p.y)*(a.y - p.y) + (a.z - p.z)*(a.z - p.z));
	float bmag = sqrt((b.x - p.x)*(b.x - p.x) + (b.y - p.y)*(b.y - p.y) + (b.z - p.z)*(b.z - p.z));
	
	if (amag < bmag)
	{
		intersection.x = a.x;
		intersection.y = a.y;
		intersection.z = a.z;
	}
	else
	{
		intersection.x = b.x;
		intersection.y = b.y;
		intersection.z = b.z;
	}
}
