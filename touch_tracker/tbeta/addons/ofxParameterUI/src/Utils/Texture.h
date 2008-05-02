#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma comment(lib, "Opengl32.lib")					//Link to OpenGL32.lib so we can use OpenGL stuff

#include <stdio.h>										// Standard I/O header 

								// Header for OpenGL32 library

#ifdef _WIN32
	#include <windows.h>    // included in all Windows apps
	#include <winuser.h>    // Windows constants
	#include <GL/gl.h>
	#include <GL/glu.h>
#else
	#include <OpenGL/glu.h>
	#include <OpenGL/OpenGL.h>	
#endif


typedef	struct									
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;	

#endif