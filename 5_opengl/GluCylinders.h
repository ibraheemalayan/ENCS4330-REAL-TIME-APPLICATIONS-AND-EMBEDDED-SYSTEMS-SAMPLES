/*
 * GluCylinders.h   -   Version 1.0
 *
 * Author: Samuel R. Buss
 *
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * 
 * Samples of how to create cylinders using gluQuadric...
 *	Rationale: GLUT does not support these cylinder types, so this
 *		gives an example of how to use them.
 */

// Function prototypes for GluCylinders.c

void myKeyboardFunc( unsigned char key, int x, int y );
void mySpecialKeyFunc( int key, int x, int y );

void drawScene(void);

void initRendering();
void resizeWindow(int w, int h);

// ********************************************************
// These are four general purpose routines for generating
//	cylinders, with or without caps.
// *********************************************************
void drawGluCylinder( double height, double radius, int slices, int stacks );
void drawGluSlantCylinder( double height, double radiusBase, double radiusTop, int slices, int stacks );
void drawGluCylinderWithCaps( double height, double radius, int slices, int stacks );
void drawGluSlantCylinderWithCaps( double height, double radiusBase, double radiusTop, int slices, int stacks );

