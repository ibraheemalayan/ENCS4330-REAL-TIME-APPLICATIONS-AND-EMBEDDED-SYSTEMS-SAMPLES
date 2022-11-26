//
// glrnRecordNormals.h - Version 1.0, 4/16/2017
//  Display recorded normals for OpenGL surface3 normal debugging
//  Developed for Math 155A, Winter 2017
//  Author: Sam Buss
//  All rights reserved.
//  May be used freely for academic purposes.
//  Software is as-is and without any warranties.
//

#ifndef GLRB_RECORD_NORMALS
#define GLRB_RECORD_NORMALS

#include <vector>

// Call this routine *BEFORE* calling glBegin where you plan to record normals
// Function: Saves the current modelview matrix
void glrnBegin();

// Call this routine every time you call glNormal..(), with exactly the
//    same parameters.  
// The parameter is the current normal.
// Function: Records the current normal.
void glrnNormal3f(float x, float y, float z);

// Call this routine every time you call glVertex..(), with exactly the
//    same parameters.  
// The parameter is the current vertex position.
// Records the vertex along with the normal for later rendering
void glrnVertex3f(float x, float y, float z);

// Call this routine to render the normals.
void glrnRenderNormals();

// Set the length of at which the normal vectors will be drawn
//   Use length equal to zero to disable drawing normals
void glrnSetDrawLength(float length);

// Call this routine to start a new pass (not necessary if glrnRenderNormals()
//   has been called.
void glrnInit();


#endif // GLRB_RECORD_NORMALS
