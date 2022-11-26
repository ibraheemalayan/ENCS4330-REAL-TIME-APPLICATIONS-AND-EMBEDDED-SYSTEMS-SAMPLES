/*
 * SimpleDraw.h
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
 * Version 1.1.  
 *    1.1. This header file unchanged from Version 1.0
 *    1.0. Original version released Spring 2003.
 */

// Function prototypes for SimpleDraw.c

void myKeyboardFunc( unsigned char key, int x, int y );

void drawScene(void);

void initRendering();
void resizeWindow(int w, int h);
