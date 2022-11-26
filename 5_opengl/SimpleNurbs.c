/**************************************************
 * SimpleNurbs.c
 *
 * Program to demonstrate how to form a single
 *    NURBS patch -- or to be more precise, 
 *    a rational Bezier patch. 
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 * USAGES: There are a number of keyboard commands that control 
 * the animation.  They must be typed into the graphics window, 
 * and are listed below:
 *
 * CONTROLLING RESOLUTION OF THE NURBS MESH
 *   Press "U" to increase the number samples in U direction.
 *   Press "u" to decrease the number samples in U direction.
 *   Press "V" to increase the number samples in V direction.
 *   Press "v" to decrease the number samples in U direction.
 *   
 * CONTROLLING THE ANIMATION:
 *   Press the "a" key to toggle the animation off and on.
 *   Press the "s" key to perform a single step of the animation.
 *   The left and right arrow keys controls the
 *		rate of rotation around the y-axis.
 *   The up and down arrow keys increase and decrease the rate of 
 *		rotation around the x-axis.  In order to reverse rotational
 *		direction you must zero or reset the patch ("0" or "r").
 *   Press the "r" key to reset back to initial
 *		position, with no rotation.
 *   Press "0" (zero) to zero the rotation rates.
 *
 * COMMANDS SHOWING OPENGL FEATURES:
 *   Pressing "p" toggles between wireframe and polygon mode.
 *   Pressing "f" key toggles between flat and smooth shading.
 *
 * FOR CONTROL POINTS AT INFINITY.  Change the value of the
 *   variable USE_CNTLPT_AT_INFINITY from 0 to 1 and recompile.
 *   Should be identical in functionality.
 *
 **/

#include <stdlib.h>
#include "SimpleNurbs.h"
#include <math.h>
#include <limits.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library

// Set next variable to 1 to use 3x4 patch with control points at infinity
//	Set it to 0 to use 4x4 patch without control points at infinity.
//  You must recompile to switch modes
#define USE_CNTLPT_AT_INFINITY 0

// Control points for the 4x4 Bezier patch. (No points at infinity.)
const float circularStrip[4][4][4] = {
	{ {-2, -1, 0, 1}, { -0.6667, -0.3333, 1.3333, 0.3333},
							{ 0.6667, -0.3333, 1.3333, 0.3333}, { 2, -1, 0, 1 } },
	{ {-3, 0, 0, 1}, { -1, 0, 2, 0.3333 }, 
							{ 1, 0, 2, 0.3333}, { 3, 0, 0, 1}},
	{ {-1.5, 0.5, 0, 1}, {-0.5, 0.1667, 1, 0.3333}, 
							{0.5, 0.1667, 1, 0.3333}, {1.5, 0.5, 0, 1}},			
	{ {-2, 1, 0, 1}, { -0.6667,  0.3333, 1.3333, 0.3333},
							{ 0.6667,  0.3333, 1.3333, 0.3333}, { 2,  1, 0, 1 } }
};

// Control points for the 3x4 Bezier patch. (With points at infinity.)
const float circularStripInfty[4][3][4] = {
	{ {-2, -1, 0, 1}, { 0, 0, 2, 0}, { 2, -1, 0, 1 } },
	{ {-3, 0, 0, 1}, { 0, 0, 3, 0}, { 3, 0, 0, 1}},
	{ {-1.5, 0.5, 0, 1}, { 0, 0, 1.5, 0}, {1.5, 0.5, 0, 1}},			
	{ {-2, 1, 0, 1}, { 0, 0, 2, 0}, { 2,  1, 0, 1 } }
};

// Variables controlling the fineness of the polygonal mesh
int NumUs = 4;
int NumVs = 4;

const float PI2 = 2.0f*3.1415926535f;

GLenum shadeModel = GL_FLAT;		// Toggles between GL_FLAT and GL_SMOOTH
GLenum polygonMode = GL_LINE;		// Toggles between GL_LINE and GL_FILL

// Variables controlling the animation
GLenum runMode = GL_TRUE;
float RotX = 0.0f;					// Rotational position around x-axis
float RotY = 0.0f;					// Rotational position around y-axis
float RotIncrementX = 0.0;			// Rotational increment, x-axis
float RotIncrementY = 0.0;			// Rotational increment, y-axis
const float RotIncFactor = 1.5;	// Factor change in rot rate per key stroke

// Lighting values
float ambientLight[4] = {0.2, 0.2, 0.2, 1.0};
float Lt0amb[4] = {0.1, 0.1, 0.1, 1.0};
float Lt0diff[4] = {0.6, 0.6, 0.6, 1.0};
float Lt0spec[4] = {1.0, 1.0, 1.0, 1.0};
float Lt0pos[4] = {1.0, 0.0, 1.0, 0.0};			// Directional light

float Lt1amb[4] = {0.1, 0.1, 0.1, 1.0};
float Lt1diff[4] = {0.6, 0.6, 0.6, 1.0};
float Lt1spec[4] = {1.0, 1.0, 1.0, 1.0};
float Lt1pos[4] = {0.0, 1.0, 1.0, 0.0};			// Directional light

// Material values
float Noemit[4] = {0.0, 0.0, 0.0, 1.0};
float Matspec[4] = {1.0, 1.0, 1.0, 1.0};
float Matnonspec[4] = {0.8, 0.05, 0.4, 1.0};
float Matshiny = 50.0;

// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void myKeyboardFunc( unsigned char key, int x, int y ) 
{
	switch ( key ) {
	case 'a':
		runMode = !runMode;
		break;
	case 's':
		runMode = GL_TRUE;
		updateScene();
		runMode = GL_FALSE;
		break;
	case 27:	// Escape key
		exit(1);
	case 'r':	// Reset the animation (resets everything)
		ResetAnimation();
		break;
	case '0':	// Zero the rotation rates
		ZeroRotation();
		break;
	case 'f':	// Shade mode toggles from flat to smooth
		ShadeModelToggle();
		break;
	case 'p':	// Polygon mode toggles between fill and line
		FillModeToggle();
		break;
	case 'u':	// Decrement number of U's
		LessUs();
		break;
	case 'U':	// Increment number of U's
		MoreUs();
		break;
	case 'v':	// Decrement number of V's
		LessVs();
		break;
	case 'V':	// Increment number of V's
		MoreVs();
		break;
	}
}

// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void mySpecialKeyFunc( int key, int x, int y ) 
{
	switch ( key ) {
	case GLUT_KEY_UP:		
		// Either increase upward rotation, or slow downward rotation
		KeyUp();
		break;
	case GLUT_KEY_DOWN:
		// Either increase downwardward rotation, or slow upward rotation
		KeyDown();
		break;
	case GLUT_KEY_LEFT:
		// Either increase left rotation, or slow down rightward rotation.
		KeyLeft();
		break;
	case GLUT_KEY_RIGHT:
		// Either increase right rotation, or slow down leftward rotation.
		KeyRight();
		break;
	}
}

// The routines below are coded so that the only way to change from 
//	one direction of rotation to the opposite direction is to first 
//  reset the animation, 

void KeyUp() {
    if ( RotIncrementX == 0.0 ) {
		RotIncrementX = -0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementX < 0.0f) {
		RotIncrementX *= RotIncFactor;
	}
	else {
		RotIncrementX /= RotIncFactor;
	}	
}

void KeyDown() {
    if ( RotIncrementX == 0.0 ) {
		RotIncrementX = 0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementX > 0.0f) {
		RotIncrementX *= RotIncFactor;
	}
	else {
		RotIncrementX /= RotIncFactor;
	}	
}

void KeyLeft() {
    if ( RotIncrementY == 0.0 ) {
		RotIncrementY = -0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementY < 0.0) {
		RotIncrementY *= RotIncFactor;
	}
	else {
		RotIncrementY /= RotIncFactor;
	}	
}

void KeyRight()
{
    if ( RotIncrementY == 0.0 ) {
		RotIncrementY = 0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementY > 0.0) {
		RotIncrementY *= RotIncFactor;
	}
	else {
		RotIncrementY /= RotIncFactor;
	}	
}


// Resets position and sets rotation rate back to zero.
void ResetAnimation() {
	RotX = RotY = RotIncrementX = RotIncrementY = 0.0;
}

// Sets rotation rates back to zero.
void ZeroRotation() {
	RotIncrementX = RotIncrementY = 0.0;
}

// Toggle between smooth and flat shading
void ShadeModelToggle() {
	if ( shadeModel == GL_FLAT ) {
		shadeModel = GL_SMOOTH;
	}
	else {
		shadeModel = GL_FLAT;
	}
}

// Toggle between line mode and fill mode for polygons.
void FillModeToggle() {
	if ( polygonMode == GL_LINE ) {
		polygonMode = GL_FILL;
	}
	else {
		polygonMode = GL_LINE;
	}
}

// Increment number of U's
void MoreUs() {
	NumUs++;
}

// Decrement number of U's
void LessUs() {
	if (NumUs>4) {
		NumUs--;
	}
}

// Increment number of V's
void MoreVs() {
	NumVs++;
}

// Decrement number of V's
void LessVs() {
	if (NumVs>4) {
		NumVs--;
	}
}

void updateScene( void )
{
	// Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel( shadeModel );	// Set the shading to flat or smooth.
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);	// Set to be "wire" or "solid"

	// Bezier Patch Materials
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Matnonspec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Matspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Matshiny);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);

	glPushMatrix();		// Save to use again next time.

	// Update the orientation, if the animation is running.
	if ( runMode ) {
		RotY += RotIncrementY;
		if ( fabs(RotY)>360.0 ) {
			RotY -= 360.0*((int)(RotY/360.0));
		}
		RotX += RotIncrementX;
		if ( fabs(RotX)>360.0 ) {
			RotX -= 360.0*((int)(RotX/360.0));
		}
	}
	// Set the orientation.
	glRotatef( RotX, 1.0, 0.0, 0.0);
	glRotatef( RotY, 0.0, 1.0, 0.0);

	// Draw the Bezier patch
 	glEnable(GL_MAP2_VERTEX_4);
	if ( USE_CNTLPT_AT_INFINITY==0 ) {
  		glMap2f(GL_MAP2_VERTEX_4, 0,1,4,4, 0,1,16,4, &circularStrip[0][0][0] );
	}
	else {
		glMap2f(GL_MAP2_VERTEX_4, 0,1,4,3, 0,1,12,4, &circularStripInfty[0][0][0] );

	}
	glMapGrid2f( NumUs, 0,1, NumVs, 0, 1);
	glEvalMesh2(GL_FILL, 0,NumUs, 0, NumVs);

	glPopMatrix();		// Restore to original matrix as set in resizeWindow()

	// Flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();
}

// Initialize OpenGL
void initRendering()
{
    glEnable( GL_DEPTH_TEST );
	glEnable( GL_AUTO_NORMAL );

	glEnable(GL_LIGHTING);		// Enable lighting calculations
	glEnable(GL_LIGHT0);		// Turn on lights (unnecessary here, since also in Animate()
	glEnable(GL_LIGHT1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);	// Ambient light

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);		// Phong light backsides

	// Next line is suspicious, perhaps compensating for OpenGL bug	
	glFrontFace(GL_CW);					// Is needed for Bezier Patches (OpenGL bug??)

	// Light 0 
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
	glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// Light 1 
	glLightfv(GL_LIGHT1, GL_AMBIENT, Lt1amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Lt1diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Lt1spec);
	glLightfv(GL_LIGHT1, GL_POSITION, Lt1pos);
}

// Called when the window is resized
//		Sets up the projection view matrix (somewhat poorly, however)
void resizeWindow(int w, int h)
{
    float aspectRatio;
	glViewport( 0, 0, w, h );	// View port uses whole window
	h = (w == 0) ? 1 : h;
	aspectRatio = (float)w/(float)h;

	// Set up the proection view matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectRatio, 1.0, 30.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	// Move system 10 units away to be able to view from the origin.
	glTranslatef(0.0, 0.0, -10.0);

	// Tilt system 15 degrees downward in order to view from above 
	//   the xy-plane.
	glRotatef(15.0, 1.0,0.0,0.0);	 
}


// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop
int main( int argc, char** argv )
{
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Window position (from top corner), and size (width and hieght)
    glutInitWindowPosition( 10, 60 );
    glutInitWindowSize( 620, 160 );
    glutCreateWindow( "Nurbs demo" );

	// Initialize OpenGL rendering modes
    initRendering();
	resizeWindow(620,160);

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );
	glutSpecialFunc( mySpecialKeyFunc );

	// Set up the callback function for resizing windows
    glutReshapeFunc( resizeWindow );

	// Call this for background processing
	glutIdleFunc( updateScene );
	// Call this whenever window needs redrawing
    glutDisplayFunc( updateScene );
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

    return(0);	// This line is never reached.
}
