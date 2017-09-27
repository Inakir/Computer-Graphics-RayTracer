#include <Windows.h>
#include <GL/glut.h>
#include <math.h>
#include "ScanConvert.h"
#include "PolygonDrawer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <climits>
#include <string>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* strtof */
#include <sstream> 

using namespace std;

/******************************************************************
	Notes:
	Image size is 400 by 400 by default.  You may adjust this if
		you want to.
	You can assume the window will NOT be resized.
	Call clearFramebuffer to clear the entire framebuffer.
	Call setFramebuffer to set a pixel.  This should be the only
		routine you use to set the color (other than clearing the
		entire framebuffer).  drawit() will cause the current
		framebuffer to be displayed.
	As is, your scan conversion should probably be called from
		within the display function.  There is a very short sample
		of code there now.
	You may add code to any of the subroutines here,  You probably
		want to leave the drawit, clearFramebuffer, and
		setFramebuffer commands alone, though.
  *****************************************************************/

float framebuffer[ImageH][ImageW][3];

Pt eye;
vector<Pt> lights;
float epsilon = .00001;
vector<sphere> spheres;
vector<infinteCylinder> cylinders;
vector<infintePlane> planes;
vector<ellipsoid> ellipsoids;

bool paintshape = true;
int scene;

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			eye.moveUp(1);
			clearFramebuffer();
			paintshape = true;
			break;
		case GLUT_KEY_DOWN:
			eye.moveDown(1);
			clearFramebuffer();
			paintshape = true;
			break;
		case GLUT_KEY_RIGHT:
			eye.moveRight(1);
			clearFramebuffer();
			paintshape = true;
			break;
		case GLUT_KEY_LEFT:
			eye.moveLeft(1);
			clearFramebuffer();
			paintshape = true;
			break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	paintshape = true;

	switch (key)
	{
	case '1':
	{
		scene = 1;
		lights.clear();
		spheres.clear();
		cylinders.clear();
		planes.clear();
		ellipsoids.clear();
		eye = Pt(200, 200, -100);
		Pt ka(.5, .5, .5);		//ambient light coefficient
		Pt A(.1, 0, 0);			//intensity of ambient light
		Pt kd1(.7, 0, 0);		//diffuse Reflection coefficient
		Pt ks1(.5, .5, .5);		//specular Reflection coefficient
		Pt kd2(.7, 0, .7);		//diffuse Reflection coefficient
		Pt ks2(.1, .5, .1);		//specular Reflection coefficient
		Pt source1(0, 50, 40);
		Pt source2(192, 210, -15);
		lights.push_back(source2);
		lights.push_back(source1);
		sphere s(50, 200, 70, 50, ka, A, kd1, ks1, 0);
		sphere s1(150, 300, 80, 50, ka, A, kd2, ks2, 0);
		spheres.push_back(s);
		spheres.push_back(s1);
		infintePlane plane(200, 300, 3000, 0, 0, -1, ka, A, kd1, ks1, 0); //back wall
		infintePlane plane1(0, 300, 110, 1, 0, -2, ka, A, kd1, ks1, 0); //left vertical line
		infintePlane plane2(400, 400, 110, 0, -1, 0, ka, A, kd1, ks1, 0);	//bottom angled horizontal line
		planes.push_back(plane);
		planes.push_back(plane1);
		planes.push_back(plane2);
		infinteCylinder c1(175, 200, 105, 15, 0, 1, 0, ka, A, kd2, ks2, 0);
		clearFramebuffer();
	}
		break;
	case '2':
	{
		scene = 2; 
		lights.clear();
		spheres.clear();
		cylinders.clear();
		planes.clear();
		ellipsoids.clear();
		eye = Pt(200, 200, -100);
		Pt ka(.5, .5, .5);		//ambient light coefficient
		Pt A(.1, 0, 0);			//intensity of ambient light
		Pt kd1(.7, 0, 0);		//diffuse Reflection coefficient
		Pt ks1(.5, .5, .5);		//specular Reflection coefficient
		Pt kd2(.7, 0, .7);		//diffuse Reflection coefficient
		Pt ks2(.1, .5, .1);		//specular Reflection coefficient
		Pt source1(0, 0, 30);
		lights.push_back(source1);
		sphere s(50, 200, 70, 50, ka, A, kd1, ks1, 0);
		sphere s1(175, 400, 100, 50, ka, A, kd2, ks2, 0);
		spheres.push_back(s);
		spheres.push_back(s1);
		infintePlane plane(200, 300, 3000, 0, 0, -1, ka, A, kd1, ks1, 0); //back wall
		infintePlane plane1(0, 300, 110, 1, 0, -2, ka, A, kd1, ks1, 0); //left vertical line
		infintePlane plane2(400, 400, 110, 0, -1, 0, ka, A, kd1, ks1, 0);	//bottom angled horizontal line
		planes.push_back(plane);
		planes.push_back(plane1);
		planes.push_back(plane2);
		infinteCylinder c1(175, 200, 105, 15, 0, 1, 0, ka, A, kd2, ks2, 0);
		cylinders.push_back(c1);
		clearFramebuffer();
	}
		break;
	case '3':
	{
		scene = 3;
		lights.clear();
		spheres.clear();
		cylinders.clear();
		planes.clear();
		ellipsoids.clear();
		
		eye = Pt(200, 200, -300);
		Pt ka(.5, .5, .5);		//ambient light coefficient
		Pt A(.1, 0, 0);			//intensity of ambient light
		Pt kd1(.7, 0, 0);		//diffuse Reflection coefficient
		Pt ks1(.5, .5, .5);		//specular Reflection coefficient
		Pt kd2(.7, 0, .7);		//diffuse Reflection coefficient
		Pt ks2(.1, .5, .1);		//specular Reflection coefficient
		Pt source1(0, 50, 40);
		Pt source2(192, 210, -15);
		Pt source3(0, 0, 30);
		lights.push_back(source1);
		lights.push_back(source2);
		lights.push_back(source3);
		sphere s(50, 200, 70, 50, ka, A, kd1, ks1, 0);
		sphere s2(100, 100, 70, 50, ka, A, kd1, ks1, 0);
		sphere s3(300, 300, 80, 50, ka, A, kd2, ks2, 0);
		sphere s4(175, 400, 100, 50, ka, A, kd2, ks2, 0);
		spheres.push_back(s);
		spheres.push_back(s2);
		spheres.push_back(s3);
		spheres.push_back(s4);
		infintePlane plane(200, 300, 3000, 0, 0, -1, ka, A, kd1, ks1, 0); //back wall
		infintePlane plane1(0, 300, 110, 1, 0, -2, ka, A, kd1, ks1, 0); //left vertical line
		infintePlane plane2(400, 400, 110, 0, -1, 0, ka, A, kd1, ks1, 0);	//bottom angled horizontal line
		planes.push_back(plane);
		planes.push_back(plane1);
		planes.push_back(plane2);
		infinteCylinder c1(200, 200, 105, 50, 0, 1, 0, ka, A, kd2, ks2, .5);
		infinteCylinder c2(175, 200, 105, 15, 0, 1, 0, ka, A, kd2, ks2, 0);
		cylinders.push_back(c1);
		cylinders.push_back(c2);
		clearFramebuffer();
	}
		break;
	case '4':
	{
		scene = 4;
		lights.clear();
		spheres.clear();
		cylinders.clear();
		planes.clear();
		ellipsoids.clear();
		eye = Pt(200, 200, -100);
		Pt ka1(.5, .5, .5);	Pt A1(.1, .3, 0); Pt kd11(.7, .6, 0); Pt ks11(.5, .5, .35); Pt kd21(.7, 0, .7);   Pt ks21(.11, .5, .1);
		Pt ka2(.5, 0, .5);	Pt A2(0, 0, .2);  Pt kd12(.7, .7, 0); Pt ks12(.5, .1, .5);  Pt kd22(.4, 0, .7);   Pt ks22(.1, .0, .1);
		Pt ka3(.5, .5, .0);	Pt A3(.1, 0, .4); Pt kd13(.7, 0, 0);  Pt ks13(.75, .5, .5); Pt kd23(.7, .2, 7);   Pt ks23(.1, .5, .61);
		Pt ka4(.5, .2, .7);	Pt A4(.1, .5, 0); Pt kd14(.7, 0, .3); Pt ks14(.5, .25, .5); Pt kd24(.0, .10, .7); Pt ks24(.1, .5, .1);

		Pt source1(-100, 310, 40); lights.push_back(source1);
		Pt source2(19.2, 310, -150); lights.push_back(source2);

		sphere s(50, 20, 700, 50, ka2, A3, kd11, ks11, 0); spheres.push_back(s);
		sphere s1(15, 300, 80, 50, ka3, A1, kd21, ks21, 0); spheres.push_back(s1);
		sphere s2(75, 700, 800, 50, ka3, A1, kd21, ks21, 0); spheres.push_back(s2);
		sphere s3(150, 300, 830, 50, ka3, A1, kd21, ks21, 0); spheres.push_back(s3);
		sphere s4(250, 30, 80, 50, ka3, A1, kd21, ks21, 0); spheres.push_back(s4);
		sphere s5(350, 320, 80, 50, ka3, A1, kd21, ks21, 0); spheres.push_back(s5);

		infintePlane plane(200, 300, 3000, 0, 0, -1, ka1, A3, kd12, ks12, 0); planes.push_back(plane);  //back wall 
		infintePlane plane1(0, 300, 110, 1, 0, -2, ka2, A3, kd13, ks14, 0);   planes.push_back(plane1);   //left vertical line
		infintePlane plane2(400, 400, 110, 0, -1, 0, ka3, A2, kd14, ks13, 0); planes.push_back(plane2); //bottom angled horizontal line


		infinteCylinder c1(175, 300, 105, 15, 0, 1, 0, ka2, A3, kd21, ks22, 0); cylinders.push_back(c1);
		infinteCylinder c3(500, 100, 105, 15, 0, 1, 0, ka2, A3, kd21, ks22, 0); cylinders.push_back(c3);
		clearFramebuffer();
	}
		break;
	case 'a':
		lights[0].moveLeft(10);
		clearFramebuffer();
		break;
	case 's':
		lights[0].moveDown(10);
		clearFramebuffer();
		break;
	case 'd':
		lights[0].moveRight(10);
		clearFramebuffer();
		break;
	case 'w':
		lights[0].moveUp(10);
		clearFramebuffer();
		break;
	case 'z':
		lights[0].moveIn(10);
		clearFramebuffer();
		break;
	case 'x':
		lights[0].moveOut(10);
		clearFramebuffer();
		break;
	case 'n':
		eye.moveIn(100);
		if (eye.z >= -10)
		{
			eye.moveIn(100);
		}
		clearFramebuffer();
		break;
	case 'm':
		eye.moveOut(100);
		clearFramebuffer();
		break;
	case 'p':
		cout<<lights[0].x << " " << lights[0].y << lights[0].z << endl;
		break;
	}
	glutPostRedisplay();
}

// Draws the scene
void drawit(void)
{
   glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
   glFlush();
}

// Clears framebuffer to black
void clearFramebuffer()
{
	int i,j;

	for(i=0;i<ImageH;i++) {
		for (j=0;j<ImageW;j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer 
void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y;

	if (R <= 1.0)
		if (R >= 0.0)
			framebuffer[y][x][0] = R;
		else
			framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (G <= 1.0)
		if (G >= 0.0)
			framebuffer[y][x][1] = G;
		else
			framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (B <= 1.0)
		if (B >= 0.0)
			framebuffer[y][x][2] = B;
		else
			framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
	
}

bool obstructed(Pt intersection, Pt source, normalized_vec light_dir, normalized_vec normal)
{
	Pt blocked;
	bool intersects = false;
	intersection.offset(epsilon, normal);
	float distance = getDistance(intersection, source);
	float temp_distance;
	float t;
	for (int s = 0; s < spheres.size(); s++)
	{
		spheres[s].getIntersect(intersection, light_dir, intersects, blocked, t);

		if (intersects)
		{
			temp_distance = getDistance(intersection, blocked);

			//if (temp_distance < distance)
			if (0 < t < 1)
			{
				return true;
			}
		}
	}
	for (int i = 0; i < planes.size(); i++)
	{
		planes[i].getIntersect(intersection, light_dir, intersects, blocked, t);

		if (intersects)
		{
			temp_distance = getDistance(intersection, blocked);

			//if (temp_distance < distance)
			if (0 < t < 1)
			{
				return true;
			}
		}
	}
	for (int i = 0; i < cylinders.size(); i++)
	{
		cylinders[i].getIntersect(intersection, light_dir, intersects, blocked, t);

		if (intersects)
		{
			temp_distance = getDistance(intersection, blocked);

			//if (temp_distance < distance)
			if (0 < t < 1)
			{
				return true;
			}
		}
	}
	return false;
}

normalized_vec getCylNorm(Pt p, Pt c, normalized_vec a, float r)
{
	Pt p_c(p.x - c.x, p.y - c.y, p.z - c.z);
	
	float nx = (p.x - c.x - dotProd(p_c, a)*a.x)/r;
	float ny = (p.y - c.y - dotProd(p_c, a)*a.y)/r;
	float nz = (p.z - c.z - dotProd(p_c, a)*a.z)/r;

	return normalized_vec(nx, ny, nz);
}

void ray(Pt p, normalized_vec dir, int count, float &r, float &b, float &g, float intensity)
{
	int index;
	int scenery_type;
	float distance;
	float temp_distance;
	Pt temp_intersection;
	Pt intersection;
	Pt ka;		//ambient light coefficient
	Pt A;		//intensity of ambient light
	Pt kd;		//diffuse Reflection coefficient
	Pt ks;		//specular Reflection coefficient
	bool intersects = false;
	normalized_vec light_dir;
	normalized_vec normal;
	normalized_vec eye_vec;
	normalized_vec temp_dir;
	float temp_t;
	float t = 2;

	scenery_type = -1;
	index = -1;
	distance = INT_MAX;
	for (int s = 0; s < spheres.size(); s++)
	{
		//cout << count;
		spheres[s].getIntersect(p, dir, intersects, temp_intersection, temp_t);
			
		if (intersects)
		{
			temp_distance = getDistance(p, temp_intersection);
			
			if (temp_distance < distance)
			if (temp_t < t)
			{
				intersection = temp_intersection;
				distance = temp_distance;
				t = temp_t;
				index = s;
				scenery_type = 1;
			}

			intersects = false;
		}
	}
	for (int i = 0; i < planes.size(); i++)
	{
		planes[i].getIntersect(p, dir, intersects, temp_intersection, t);

		if (intersects)
		{
			temp_distance = getDistance(p, temp_intersection);

			if (temp_distance < distance)
			//if (temp_t < t)
			{
				intersection = temp_intersection;
				distance = temp_distance;
				t = temp_t;
				index = i;
				scenery_type = 2;
			}

			intersects = false;
		}
	}
	for (int i = 0; i < cylinders.size(); i++)
	{
		cylinders[i].getIntersect(p, dir, intersects, temp_intersection, t);

		if (intersects)
		{
			temp_distance = getDistance(p, temp_intersection);

			if (temp_distance < distance)
			//if (temp_t < t)
			{
				intersection = temp_intersection;
				distance = temp_distance;
				t = temp_t;
				index = i;
				scenery_type = 3;
			}

			intersects = false;
		}
	}
	for (int i = 0; i < ellipsoids.size(); i++)
	{
		ellipsoids[i].getIntersect(p, dir, intersects, temp_intersection, t);

		if (intersects)
		{
			temp_distance = getDistance(p, temp_intersection);

			if (temp_distance < distance)
				//if (temp_t < t)
			{
				intersection = temp_intersection;
				distance = temp_distance;
				t = temp_t;
				index = i;
				scenery_type = 4;
			}

			intersects = false;
		}
	}
	if (scenery_type == 1) 
	{
		getDir(spheres[index].c, intersection, normal); //get normal at intersection point
		ka = spheres[index].ka;
		A  = spheres[index].A;
		kd = spheres[index].kd;
		ks = spheres[index].ks;
		if (spheres[index].R > 0 && count < 1)
		{
			normalized_vec reflected = dir;
			reflected.invert();
			float dot = dotProd(reflected, normal);
			reflected.x = 2 * dot *normal.x - reflected.x;
			reflected.y = 2 * dot *normal.y - reflected.y;
			reflected.z = 2 * dot *normal.z - reflected.z;
			ray(intersection, reflected, count++, r, b, g, intensity*spheres[index].R);
			cout << "1" << endl;
		}
	}
	else if (scenery_type == 2)
	{
		normal = planes[index].n; //get normal at intersection point
		ka = planes[index].ka;
		A  = planes[index].A;
		kd = planes[index].kd;
		ks = planes[index].ks;
		if (planes[index].R > 0 && count < 1)
		{
			normalized_vec reflected = dir;
			reflected.invert();
			float dot = dotProd(reflected, normal);
			reflected.x = 2 * dot *normal.x - reflected.x;
			reflected.y = 2 * dot *normal.y - reflected.y;
			reflected.z = 2 * dot *normal.z - reflected.z;
			ray(intersection, reflected, count++, r, b, g, intensity*planes[index].R);
		}
	}
	else if (scenery_type == 3)
	{
		getCylNorm(intersection, cylinders[index].c, cylinders[index].direction, cylinders[index].radius); //get normal at intersection point
		ka = cylinders[index].ka;
		A  = cylinders[index].A;
		kd = cylinders[index].kd;
		ks = cylinders[index].ks;
		if (cylinders[index].R > 0 && count < 3)
		{
			normalized_vec reflected = dir;
			reflected.invert();
			float dot = dotProd(reflected, normal);
			reflected.x = 2 * dot *normal.x - reflected.x;
			reflected.y = 2 * dot *normal.y - reflected.y;
			reflected.z = 2 * dot *normal.z - reflected.z;
			ray(intersection, reflected, count++, r, b, g, intensity*cylinders[index].R);

		}
	}
	else if (scenery_type == 4)
	{
		getDir(ellipsoids[index].c, intersection, normal); //get normal at intersection point
		ka = ellipsoids[index].ka;
		A  = ellipsoids[index].A;
		kd = ellipsoids[index].kd;
		ks = ellipsoids[index].ks;
		if (ellipsoids[index].R > 0 && count < 1)
		{
			normalized_vec reflected = dir;
			reflected.invert();
			float dot = dotProd(reflected, normal);
			reflected.x = 2 * dot *normal.x - reflected.x;
			reflected.y = 2 * dot *normal.y - reflected.y;
			reflected.z = 2 * dot *normal.z - reflected.z;
			ray(intersection, reflected, count++, r, b, g, intensity*ellipsoids[index].R);
		}
	}
	if (scenery_type > 0)
	{
		getDir(intersection, eye, eye_vec); //get vector from intersection to eye
		for (int l = 0; l < lights.size(); l++)
		{
			getDir(intersection, lights[l], light_dir); //get vector from intersection to light
			if (!obstructed(intersection, lights[l], light_dir, normal))
			{
				setColor(r, g, b, intersection, light_dir, normal, eye_vec, ka, A, kd, ks, intensity);
			}
		}
	}

}

void rayTrace(void)
{
	normalized_vec eyedir;
	float r, g, b;

	cout << "start" << endl;
	for (int i = 0; i < ImageH; i++)
	{
		for (int j = 0; j < ImageW; j++)
		{
			Pt pixel(i, j, 0);
			getDir(eye, pixel, eyedir, true); //get from eye

			r = 0; g = 0; b = 0;

			ray(pixel, eyedir, 0, r, b, g, 1);
			setFramebuffer(i, j, r, g, b);	
		}
	}

	drawit();
	cout << "done" << endl;
}

void display(void)
{
	if (paintshape)
	{
		rayTrace();
		paintshape = false;
	}
	
}

void init(void) {
	scene = 1;
	
	display();
}

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(ImageW,ImageH);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Inaki Rosa - Homework 5");
	init();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
