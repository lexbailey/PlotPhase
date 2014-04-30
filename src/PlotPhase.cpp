/*
 ============================================================================
 Name        : PlotPhase.cpp
 Author      : Daniel Bailey
 Version     :
 Copyright   : Copyright 2014
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <gtkmm.h>
#include "uiwindow.h"
#include "glwindow.h"

using namespace std;

PlotPhaseGtkWindows *ui;
glwindow* myGLWindow;

void nonBlockGTKMain(){ //my non-blocking main loop
	while(gtk_events_pending())
	gtk_main_iteration();
}

static
void on_window1_destroy (){
		glutLeaveMainLoop();
}

void drawPhasor(double magnitude, double angle, double width, bool arrow){
	glRotated(angle, 0.0, 0.0, 1.0);
	if (arrow){
		glBegin(GL_QUADS);
			glVertex3d(magnitude-width,  width/2.0, 0.0);
			glVertex3d(magnitude-width, -width/2.0, 0.0);
			glVertex3d(0.0, -width/2.0, 0.0);
			glVertex3d(0.0,  width/2.0, 0.0);
		glEnd();

		glBegin(GL_TRIANGLES);
			glVertex3d(magnitude-width,  width, 0.0);
			glVertex3d(magnitude-width, -width, 0.0);
			glVertex3d(magnitude, 0.0, 0.0);
		glEnd();
	}
	else{
		glBegin(GL_QUADS);
			glVertex3d(magnitude,  width/2.0, 0.0);
			glVertex3d(magnitude, -width/2.0, 0.0);
			glVertex3d(0.0, -width/2.0, 0.0);
			glVertex3d(0.0,  width/2.0, 0.0);
		glEnd();
	}
}

void drawGLWindow(){

	double width = ui->scaleLine->get_adjustment()->get_value();
	bool arrow = ui->cbArrow->get_active();
	bool total = ui->cbTotal->get_active();
	bool names = ui->cbNames->get_active();

	for (int i = 0; i<= ui->phases.size()-1; i++){
		phaseData thisPhase = ui->phases.at(i);
		glPushMatrix();
		glColor3ub(thisPhase.colR, thisPhase.colG, thisPhase.colB);
		drawPhasor(thisPhase.magnitude, thisPhase.phase, width, arrow);
		glPopMatrix();
	}

	if (total){
		glPushMatrix();
		for (int i = 0; i<= ui->phases.size()-1; i++){
			phaseData thisPhase = ui->phases.at(i);
			glColor3ub(thisPhase.colR, thisPhase.colG, thisPhase.colB);
			drawPhasor(thisPhase.magnitude, thisPhase.phase, width, arrow);
			glTranslated(thisPhase.magnitude, 0.0, 0.0);
			glRotated(thisPhase.phase, 0.0, 0.0, -1.0);
		}
		glPopMatrix();
	}
}

void exitFunc(){
	delete ui;
	delete myGLWindow;
}

int main(int argc, char **argv) {
	cout << "Hello World" << endl;
	atexit(exitFunc);
	gtk_init(&argc, &argv);
	ui = new PlotPhaseGtkWindows(argc, argv);
	myGLWindow = new glwindow(argc, argv); //create the 3D view window (freeglut)
    myGLWindow->setDoneDrawFunc(nonBlockGTKMain);  //set callback that happens after every draw
	myGLWindow->setDrawFunc(drawGLWindow); //set the callback for the actual draw
	char testName[255];
	ui->addPhasor(10.0, 0.0, testName, 1.0, 0.0, 0.0);
	((Gtk::Widget*)ui->pWindow)->signal_hide().connect( sigc::ptr_fun(on_window1_destroy) );
	ui->pWindow->show();
	myGLWindow->show();
}
