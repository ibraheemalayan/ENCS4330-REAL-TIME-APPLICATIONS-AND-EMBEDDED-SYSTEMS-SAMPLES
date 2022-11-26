/*
 * ConnectDots.h
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
 */

// Function prototypes

void myKeyboardFunc( unsigned char key, int x, int y );
void myMouseFunc( int button, int state, int x, int y );

void displayLines(void);
void removeFirstPoint();
void removeLastPoint();
void addNewPoint( float x, float y );

void initRendering();
void resizeWindow(int w, int h);



