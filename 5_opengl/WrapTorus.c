/*
 * WrapTorus.c
 *
 * Program to demonstrate how to use
 * triangular or polygonal patches to form the surface 
 * of a torus.   The triangular patches are
 * planar of course.  Because of the way the torus is
 * wrapped, the quadrilateral patches are only approximately
 * planar.
 *
 * The torus can be viewed in wireframe or in "filled
 *     polygon" mode. 
 *
 * The triangles or quadrilaterals are wrapped around the
 * torus in a single long strip.    See figure 13 Chapter I
 * in the book mentioned below.
 *
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
 * CONTROLLING RESOLUTION OF THE TORUS MESH
 *   Press "W" to increase the number wraps.
 *   Press "w" to decrease the number wraps.
 *   Press "N" to increase the number of segments per wrap.
 *   Press "n" to decrease the number of segments per wrap.
 *   Press "q" to toggle between quadrangles and triangles.
 *   
 * CONTROLLING THE ANIMATION:
 *   Press the "a" key to toggle the animation off and on.
 *   Press the "s" key to perform a single step of the animation.
 *   The left and right arrow keys controls the
 *		rate of rotation around the y-axis.
 *   The up and down arrow keys increase and decrease the rate of 
 *		rotation around the x-axis.  In order to reverse rotational
 *		direction you must zero or reset the torus ("0" or "r").
 *   Press the "r" key to reset the torus back to initial
 *		position, with no rotation.
 *   Press "0" (zero) to zero the rotation rates.
 *
 * COMMANDS SHOWING OPENGL FEATURES:
 *   Pressing "p" toggles between wireframe and polygon mode.
 *   Pressing "f" key toggles between flat and smooth shading.
 *		(BUT: flat or smooth shading has no effect in this version of the
 *		program, since Phong lighting is not used yet.  See LightTorus 
 *		for a use of this feature.)
 *
 *   There are no lights or material properties specified in
 *       the program, so the filled polygon mode looks completely
 *		 terrible.
 *
 */

#include "WrapTorus.h"
#include <windows.h>
#include <math.h>
#include <limits.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library

const float PI2 = 2.0*3.1415926535f;

GLenum runMode = GL_TRUE;

GLenum shadeModel = GL_FLAT;		// Toggles between GL_FLAT and GL_SMOOTH
GLenum polygonMode = GL_LINE;		// Toggles between GL_LINE and GL_FILL

// Variables controlling the animation
float RotX = 0.0;					// Rotational position around x-axis
float RotY = 0.0;					// Rotational position around y-axis
float RotIncrementX = 0.0;			// Rotational increment, x-axis
float RotIncrementY = 0.0;			// Rotational increment, y-axis
const float RotIncFactor = 1.5;		// Factor change in rotation rate per key stroke

// Variables controlling the fineness of the polygonal mesh
int NumWraps = 10;
int NumPerWrap = 8;

// Variables controlling the size of the torus
float MajorRadius = 3.0;
float MinorRadius = 1.0;

// Quad/Triangle toggling
int QuadMode = 1;

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
	case 'w':	// Decrement number of wraps around torus
		WrapLess();
		break;
	case 'W':	// Increment number of wraps around torus
		WrapMore();
		break;
	case 'n':	// Decrement number of polys per wrap
		NumPerWrapLess();
		break;
	case 'N':	// Increment number of polys per wrap
		NumPerWrapMore();
		break;
	case 'q':	// Toggle between triangles and Quadrilaterals
		QuadTriangleToggle();
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
		// Either increase downward rotation, or slow upward rotation
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
	else if ( RotIncrementX < 0.0) {
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
	else if ( RotIncrementX > 0.0) {
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

// Toggle quadrilaterial and triangle mode
void QuadTriangleToggle() {
	QuadMode = 1-QuadMode;
}

// Increment number of wraps
void WrapMore() {
	NumWraps++;
}

// Decrement number of wraps
void WrapLess() {
	if (NumWraps>4) {
		NumWraps--;
	}
}

// Increment number of segments per wrap
void NumPerWrapMore() {
	NumPerWrap++;	
}

// Decrement number segments per wrap
void NumPerWrapLess() {
	if (NumPerWrap>4) {
		NumPerWrap--;
	}
}

/*
 * Issue vertex command for segment number j of wrap number i.
 */
void putVert(float i, float j) {
	float wrapFrac = j/(float)NumPerWrap;
	float phi = PI2*wrapFrac;
	float theta = PI2*(i+wrapFrac)/(float)NumWraps;
	float y = MinorRadius*(float)sin(phi);
	float r = MajorRadius + MinorRadius*(float)cos(phi);
	float x = (float)sin(theta)*r;
	float z = (float)cos(theta)*r;
	glVertex3f(x,y,z);
}


void updateScene()
{
	int i, j;

	// clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel( shadeModel );	// Set the shading to flat or smooth.
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);	// Set to be "wire" or "solid"

	glPushMatrix();		// Save to use again next time.

	// Update the orientation of the torus, if the animation is running.
	if ( runMode ) {
		RotY += RotIncrementY;
		if ( fabs(RotY)>360.0 ) {
			RotY -= 360.0*((int)(RotY/360.0));
		}
		RotX += RotIncrementX;
		if ( fabs(RotX)>360.0f ) {
			RotX -= 360.0*((int)(RotX/360.0));
		}
	}
	// Set the orientation.
	glRotatef( RotX, 1.0, 0.0, 0.0);
	glRotatef( RotY, 0.0, 1.0, 0.0);

	// Draw the torus
	glColor3f( 1.0, 0.5, 1.0 );
	if ( QuadMode==1 ) {
		glBegin( GL_QUAD_STRIP );
	}
	else {
		glBegin (GL_TRIANGLE_STRIP );
	}

	for (i=0; i<NumWraps; i++ ) {
		for (j=0; j<NumPerWrap; j++) {
			putVert((float)i,(float)j);
			putVert((float)(i+1),(float)j);
		}
	}
	putVert(0.0,0.0);
	putVert(1.0,0.0);
	glEnd();


	// Draw the reference pyramid
	glTranslatef( -MajorRadius-MinorRadius-0.3, 0.0, 0.0);
	glScalef( 0.2, 0.2, 0.2 );
	glColor3f( 1.0, 1.0, 0.0 );
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f( -0.5, 0.0, sqrt(3.0)*0.5 );
	glVertex3f( -0.5f, 0.0, -sqrt(3.0)*0.5 );
	glVertex3f( 1.0, 0.0, 0.0);
	glVertex3f( 0.0, sqrt(2.0), 0.0);
	glVertex3f( -0.5, 0.0, sqrt(3.0)*0.5 );
	glVertex3f( -0.5, 0.0, -sqrt(3.0)*0.5 );
	glEnd();

	glPopMatrix();		// Restore to original matrix as set in resizeWindow()

	// flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();
}

// initialize OpenGL
void initRendering(void)
{
    glEnable( GL_DEPTH_TEST );
}

// Called when the window is resized
//		Sets up the projection view matrix (somewhat poorly, however)
void resizeWindow(int w, int h)
{
    float aspectRatio;
	glViewport( 0, 0, w, h );	// View port uses whole window
	h = (h == 0) ? 1 : h;
	aspectRatio = (float)w/(float)h;

	// Set up the projection view matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectRatio, 1.0, 30.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	// Move system 8 units away to be able to view from the origin.
	glTranslatef(0.0, 0.0, -8.0);

	// Tilt system 15 degrees downward in order to view from above 
	//   the xy-plane.
	glRotatef(15.0, 1.0,0.0,0.0);	 
}


// Main routine
//	 Set up OpenGL, hook up callbacks, and start the main loop.
int main( int argc, char** argv )
{
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Window position (from top corner), and size (width and hieght)
    glutInitWindowPosition( 10, 60 );
    glutInitWindowSize( 620, 160 );
    glutCreateWindow( "Wrapped Torus - no lighting" );

	// Initialize OpenGL rendering modes
    initRendering();

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );
	glutSpecialFunc( mySpecialKeyFunc );

	// Set up the callback function for resizing the window
    glutReshapeFunc( resizeWindow );

	// Call this for background processing
	glutIdleFunc( updateScene );
	// Call this whenever the window needs redrawing
    glutDisplayFunc( updateScene );
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

    return(0);	// This line is never reached.
}
