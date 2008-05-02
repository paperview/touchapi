
/*
#if defined(_WIN32)
#include <windows.h>
#endif 
 */


#ifndef _TGA_TEXTURE_LOADER_H
#define _TGA_TEXTURE_LOADER_H

#include <stdio.h>										// Standard I/O header 
#include <stdlib.h>

#ifdef _WIN32

	#include <windows.h>
	#include <winuser.h>

	#ifdef A_USE_GLEW_LINKED
		#include <Gl/glew.h>
	#else
		#include <GL/gl.h>
		#include <GL/glu.h>
	#endif //A_USE_GLEW_LINKED

#else

	#ifdef A_USE_GLEW_LINKED
		#include <GL/glew.h>
	#else
		#include <OpenGL/glu.h>
		#include <OpenGL/glext.h>
		#include <OpenGL/OpenGL.h>
	#endif //A_USE_GLEW_LINKED

#endif //_WIN32

#include <string.h>

#include "Texture.h"


typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;


typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


// Should this be a singleton ?

class TGATextureLoader 
{

	public:

		TGATextureLoader();

		bool LoadTGAIntoTexture(Texture * texture, char * filename);
		bool LoadTGAIntoTexture(Texture * texture, char * filename, GLuint magFilter, GLuint minFilter );

		bool LoadTGAData(Texture * texture, char * filename);

		bool LoadTGAIntoTextureFromMem(Texture * texture, unsigned char* dat, unsigned int datLengh );
		bool LoadTGAIntoTextureFromMem(Texture * texture, unsigned char* dat, unsigned int datLengh, GLuint magFilter, GLuint minFilter );

	private:

		TGAHeader tgaheader;									// TGA header
		TGA tga;												// TGA image data

		GLubyte uTGAcompare[12];	// Uncompressed TGA Header
		GLubyte cTGAcompare[12];	// Compressed TGA Header

		bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA);	// Load an Uncompressed file
		bool LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA);		// Load a Compressed file

		bool LoadUncompressedTGAFromMem(Texture * texture, unsigned char* dat, unsigned int datLengh );
		bool LoadCompressedTGAFromMem(Texture * texture, unsigned char* dat, unsigned int datLengh );

		unsigned char* currDat;
		unsigned int currDatLength;
		int currDatIndex;

		bool currDatByteRead( unsigned char* dest, int length );
		unsigned char currDatNextByte();
};

#endif