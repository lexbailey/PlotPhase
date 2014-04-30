/*
	This is the header file for glwindow.cpp
	This provides an openGL window through freeglut that will has panning, rotation and zoom built in.
*/

#ifndef GLWINDOW_INCLUDE
#define GLWINDOW_INCLUDE

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define PI 3.14159265
#define IN_RADIANS /57.2957795

//Use this to show the 'static' x axis and y axis.
//#define DEBUG_AXES

class glwindow{

	private:
		bool fullScreen;

		int zrot;
		int zrot2;
		
		int xrot;
		int xrot2;
		int yrot;
		int yrot2;
		
		float scale;
		float scale2;
		
		float xshift, yshift;
		float xshift2, yshift2;
		
		bool mousebuttons[3];
		int startx, starty;

		void (* drawCallBack)();
		void (* drawDoneCallBack)();

		void initLights();
	public:
		void Render();

		void reshape (int, int );

		void mouse(int, int, int, int);

		void motion(int, int);


		
		void setDrawFunc (void (*f)());
		void setDoneDrawFunc (void (*f)());

		glwindow(int argc, char **argv);
		void show();
};

void bounce_Render();

void bounce_reshape (int, int );

void bounce_mouse(int, int, int, int);

void bounce_motion(int, int);

#endif /*GLWINDOW_INCLUDE*/
