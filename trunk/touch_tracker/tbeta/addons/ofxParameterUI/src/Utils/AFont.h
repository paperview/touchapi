
#ifndef _A_FONT_H
#define _A_FONT_H

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


#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>


using std::string;
using namespace std;

class AFont
{

	public:
		/**** Functions ****/

		AFont( int** fontDefData, GLuint fontTextureID, int _texWidth, int _texHeight, float extraSpacing);

		AFont(int** fontDefData, char* fontTextureFileName, float extraSpacing);


		~AFont();

		void bind( void );
		void unBind( void );

		void printChar( char c);
		void printCharCentered( char c);

		void printCharCenteredXZ( char c);

		void printTextLine ( char* textLine); // will not handle linebreaks
		void printTextLine ( char* textLine, float extraSpacing); 
		void printText ( string textLine); // will handle linebreaks

		void printText2D( char* _text, float _x, float _y, float _size );

		float stringWidth( char* _text, float _size );

		/**** Variables ****/
		float* charWidth;
		float* charHeight;
		float*** charPoints;

		float extraSpacing;

		unsigned int listBase;
		unsigned int listBaseCentered;
		unsigned int listBaseCenteredXZ;

		unsigned int testList;

		unsigned int fontTextureID;

	private:	
		/**** Functions ****/
		void commonInit( void );
		void readAndParseFontDefFile( char* fontDefFileName );
		void createCharPointData( void );
		void cleanupFontDefData( int** fdef ); // takes what we get from the .h file and processes it into the next stage
		void actOnTextCommand( char* buf );
		void createTextureFromData();

		
#ifdef _WIN32	
		
		void loadBitmap(char *fileName);
		void loadBitmapFromResource(unsigned int textureBitmapResID);	
#else
		unsigned int loadFontTextureQuicktime(char *fileName, int& texWidth, int& texHeight );		
		
#endif
		
		

		void createDisplayLists();


		/**** Variables ****/
		int textureWidth, textureHeight;
		int charHeightPixels;

		unsigned char *imageData;
		int maxDataLength;
		int baseLine;
		float extraSpacingPixels;
		char* fdFileName;
		string textureFileName;
		int** fontDefData;

};

#endif

