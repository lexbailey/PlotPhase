#include "glwindow.h"

/*
	This provides an openGL window through freeglut that has panning, rotation and zoom built in.
*/

glwindow *glWindow_ME;

void glwindow::initLights() 
{
  glDepthFunc(GL_LEQUAL);           // the type of depth test to do
  glEnable(GL_DEPTH_TEST);          // enables depth testing
  glShadeModel(GL_SMOOTH);          // enables smooth color shading
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);  
  glEnable (GL_POLYGON_SMOOTH);  
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_COLOR_MATERIAL);
  /*
  glEnable(GL_LIGHTING);
  
  glEnable(GL_LIGHT0);   
  GLfloat light_position[] = { -4.0, -8.0, 4.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  GLfloat light_diff[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
  
  glEnable(GL_LIGHT1); 
  GLfloat light_position1[] = { 5.0, 2.0, 8.0, 1.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  GLfloat light_amb1[] = { 0.3, 0.3, 0.3, 1.0 };
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb1);
      
  GLfloat light_position2[] = { 7.0, -5.0, 2.0, 1.0 };
  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
  GLfloat light_diff2[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diff2);
  */
}

void glwindow::Render()
{
/*      Enable depth testing
        */
        glEnable(GL_DEPTH_TEST);

        /*      Heres our rendering. Clears the screen
                to black, clear the color and depth
                buffers, and reset our modelview matrix.
        */
        glClearColor(0.8f, 0.8f, 0.7f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
		
        glPushMatrix();
        glLoadIdentity();
		glTranslatef(0,0,-50.0f); //move back a little
			#ifdef DEBUG_AXES
			glBegin(GL_LINES);
				glColor3f(0.0,0.0,1);
				glVertex3f(0,0,0);
				glVertex3f(0,0,1000);
				glColor3f(1,0,0);
				glVertex3f(0,0,0);
				glVertex3f(1000,0,0);
				glColor3f(0,1,0);
				glVertex3f(0,0,0);
				glVertex3f(0,1000,0);
			glEnd();
			#endif
		
			//glRotatef(90,-1.0f, 0.0f, 0.0f); //make the z axis become the up down. (like it is on a 3D printer)
				glScalef(scale, scale, scale);
					glTranslatef(xshift2, yshift2, 0.0f); //move back a little
					glRotatef(zrot2, 0.0f, 0.0f, 1.0f);
					glRotatef(xrot2, 1.0f, 0.0f, 0.0f);
					glRotatef(yrot2, 0.0f, 1.0f, 0.0f);							
							//#################################################
							//## Start draw                                  ##
							//#################################################
								//the bed
								//glColor3f(0.5,0.5,0.5);
								//glBegin(GL_QUADS);
								//	glVertex3f(-100,-100,0);
								//	glVertex3f(100,-100,0);
								//	glVertex3f(100,100,0);
								//	glVertex3f(-100,100,0);
								//glEnd();
								//Call draw function
								if (drawCallBack!=NULL){
									drawCallBack();
								}
							//#################################################
							//##  End draw                                   ##
							//#################################################
        glPopMatrix();
        glFlush();
	glutSwapBuffers();
	
	if (drawDoneCallBack!=NULL){
		drawDoneCallBack();
	}	
}

void glwindow::reshape (int width, int height) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 1000.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the near and far planes
	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
}

void glwindow::mouse(int button, int state, int x, int y){

	//printf("Mouse... button:%d state:%d x:%d y:%d\n", button, state, x, y);
	mousebuttons[button] = (state == 0);
	if (mousebuttons[0]||mousebuttons[1]||mousebuttons[2]){
		startx = x;
		starty = y;
	}	
	if (!mousebuttons[1]){
		zrot = zrot2;
		xrot = xrot2;
		yrot = yrot2;		
	}
	if (!mousebuttons[0]){
		xshift = xshift2;
		yshift = yshift2;
	}
	if (!mousebuttons[2]){
		scale2 = scale;
	}
	if (button == 4){
		scale /= 1.1;
		scale2 = scale;
		glutPostRedisplay();
	}
	if (button == 3){
		scale *= 1.1;
		scale2 = scale;
		glutPostRedisplay();
	}
 }

//If anybody wants something to do then improve this function
//The rotating is a little bit off and the panning isn't constant
void glwindow::motion(int x, int y){

	//printf("Motion: x:%d y:%d\n", x, y);
	if (mousebuttons[1]){
		//This is the rotation section, it needs to be better
		int differencex = startx - x;
		zrot2 = (zrot - differencex/5)%360;
		
		int differencey = (starty - y);
		float xfactor = -cos(zrot2 IN_RADIANS);
		float yfactor = sin(zrot2 IN_RADIANS);
		xrot2 = (xrot + ((differencey/5)*xfactor));
		yrot2 = (yrot + ((differencey/5)*yfactor));
		
		glutPostRedisplay();
	}
	else{
		if(mousebuttons[0]){
			//panning! this should relate to the current zoom level so that
			//the object moves as far as the mouse moves rather than running
			//away from the cursor with the slightest movement when zoomed in.
			xshift2 = xshift - ((startx - x)/5);
			yshift2 = yshift + ((starty - y)/5);
			glutPostRedisplay();
		}
		if(mousebuttons[2]){
			//This is fine, just zooming!
			scale = scale2* pow(1.1, ((starty - y)/50.0f));
			glutPostRedisplay();
		}
	}
}

void glwindow::setDrawFunc (void (*mf)()){
	drawCallBack = mf;
}

void glwindow::setDoneDrawFunc (void (*mf)()){
	drawDoneCallBack = mf;
}

glwindow::glwindow(int argc, char **argv){
	fullScreen = false;

	zrot = 0;
	zrot2 = 0;
	
	xrot = 0;
	xrot2 = 0;
	yrot = 0;
	yrot2 = 0;
		
	scale = 0.2;
	scale2 = 0.2;
		
	xshift = 0;
	yshift = 0;
	xshift2 = 0;
	yshift2 = 0;
		
	mousebuttons[0] = false;
	mousebuttons[1] = false;
	mousebuttons[2] = false;
	startx = 0;
	starty = 0;
	
	glWindow_ME = this;

	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode (GLUT_DOUBLE); // Set up a double display buffer
	glutInitWindowSize (700, 500); // Set the width and height of the window
	glutInitWindowPosition (100, 100); // Set the position of the window	
	glutCreateWindow ("PlotPhase"); // Set the title for the window
	glutDisplayFunc(bounce_Render); 
	glutIdleFunc(bounce_Render); 
	glutReshapeFunc(bounce_reshape); // Tell GLUT to use the method "reshape" for reshaping
	glutMouseFunc(bounce_mouse);
	glutMotionFunc(bounce_motion);
	initLights();
	if (fullScreen) { glutFullScreen();}
}

void glwindow::show(){
	glutMainLoop(); // Enter GLUT's main loop
}

void bounce_Render(){glWindow_ME->Render();};

void bounce_reshape (int x, int y){glWindow_ME->reshape(x,y);};

void bounce_mouse(int button, int state, int x, int y){glWindow_ME->mouse(button, state, x, y);};

void bounce_motion(int x, int y){glWindow_ME->motion(x, y);};

