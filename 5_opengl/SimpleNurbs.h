/*
 * SimpleNurbs.h
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
void mySpecialKeyFunc( int key, int x, int y );

void updateScene( void );

void initRendering();
void resizeWindow(int w, int h);

void KeyUp();
void KeyDown();
void KeyLeft();
void KeyRight();
void ResetAnimation();
void ZeroRotation();
void ShadeModelToggle();
void FillModeToggle();
void MoreUs();
void LessUs();
void MoreVs();
void LessVs();

