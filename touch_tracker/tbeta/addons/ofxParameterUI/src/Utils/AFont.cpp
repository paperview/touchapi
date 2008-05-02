
#include "AFont.h"



AFont::AFont( int** unprocessedFontDefData, GLuint _fontTextureID, int _texWidth, int _texHeight, float extraSpacing)
{

	commonInit();
	extraSpacingPixels = extraSpacing;

	cleanupFontDefData( unprocessedFontDefData );
	createCharPointData();


	fontTextureID = _fontTextureID;
	textureWidth = _texWidth;
	textureHeight = _texHeight;


	createDisplayLists();
}



void AFont::bind( void ) 
{
	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GEQUAL, 0.1825f); //	glAlphaFunc(GL_GEQUAL, 0.0625);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glBindTexture(GL_TEXTURE_2D, fontTextureID);
}


void AFont::unBind( void ) 
{
	glDisable(GL_BLEND);
	glAlphaFunc(GL_ALWAYS, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);  
}


void AFont::commonInit( void ) 
{

	maxDataLength = 256;
	int i,j,k;


	// init storage for the data we read in
	fontDefData = new int*[maxDataLength];
	for( i = 0; i < maxDataLength; i++) 
	{ 
		fontDefData[i] = new int[5]; 
		for( j = 0; j < 4; j++) 
		{ 
			fontDefData[i][j] = 0;
		}
		fontDefData[i][4] = 9999; // then we can see if there is a character there
	}

	charPoints = new float**[maxDataLength];
	for( i = 0; i < maxDataLength; i++) 
	{ 
		charPoints[i] = new float*[4]; 
		for( j = 0; j < 4; j++) 
		{ 
			charPoints[i][j] = new float[2]; 
			for (k=0; k < 2; k++) 
			{
				charPoints[i][j][k] = 0.0f;
			}
		}
	}

	charWidth = new float[maxDataLength];
	for( i = 0; i < maxDataLength; i++) { charWidth[i] = 0.0f; }

	// do wee need heights ?
	charHeight = new float[maxDataLength];
	for( i = 0; i < maxDataLength; i++) { charHeight[i] = 0.0f; }

}



void AFont::createDisplayLists() 
{

	listBase = glGenLists(maxDataLength);
	listBaseCentered = glGenLists(maxDataLength);
	listBaseCenteredXZ = glGenLists(maxDataLength);

	float textureWidthf = (float)textureWidth;
	float textureHeightf = (float)textureHeight;

	float tmpBot, tmpTop, tmpLeft, tmpRight, tmpWidth, tmpHeight;
	float texBot, texTop, texLeft, texRight;

	int i;
	for( i = 0; i < maxDataLength; i++) 
	{ 
		
		if (fontDefData[i][4] != 9999) 
		{

			texBot   = 1.0f-((float)fontDefData[i][1]/textureHeightf);
			texTop   = 1.0f-((float)fontDefData[i][0]/textureHeightf);
			texLeft  = (float)fontDefData[i][2]/textureWidthf;
			texRight = (float)fontDefData[i][3]/textureWidthf;

			//printf(" %f, %f, %f, %f %i \n", texBot, texTop, texLeft, texRight, textureWidth);

			// create normal list
			glNewList(listBase+i, GL_COMPILE);
				glBegin(GL_QUADS);

					glTexCoord2d(texLeft, texBot); 
					glVertex3f(charPoints[i][0][0], charPoints[i][0][1],  0.0f);
					
					glTexCoord2d(texLeft, texTop); 
					glVertex3f(charPoints[i][1][0], charPoints[i][1][1],  0.0f);

					glTexCoord2d(texRight, texTop); 
					glVertex3f(charPoints[i][2][0], charPoints[i][2][1],  0.0f);

					glTexCoord2d(texRight, texBot); 
					glVertex3f(charPoints[i][3][0], charPoints[i][3][1],  0.0f);
				glEnd();
				glTranslatef(charWidth[i] + extraSpacing, 0.0f, 0.0f);
			glEndList();


			// create centered list
			if (fontDefData[i][4] == 0) { // no kerning
				tmpWidth = charPoints[i][2][0];
			} else {
				tmpWidth = (charPoints[i][2][0] - charPoints[i][0][0]); // right - left, left is negative(?);
			}

			tmpHeight = charPoints[i][1][1] - charPoints[i][0][1];

			tmpLeft  = -(tmpWidth/2.0f);
			tmpRight = (tmpWidth/2.0f);

			tmpBot   = -(tmpHeight/2.0f);
			tmpTop   = (tmpHeight/2.0f);

			glNewList(listBaseCentered+i, GL_COMPILE);
				glBegin(GL_QUADS);

					glTexCoord2d(texLeft, texBot); 
					glVertex3f(tmpLeft, tmpBot,  0.0f);
					
					glTexCoord2d(texLeft, texTop); 
					glVertex3f(tmpLeft, tmpTop,  0.0f);

					glTexCoord2d(texRight, texTop);
					glVertex3f(tmpRight, tmpTop,  0.0f);

					glTexCoord2d(texRight, texBot); 
					glVertex3f(tmpRight, tmpBot,  0.0f);
				glEnd();
				//glTranslatef(charWidth[i], 0.0f, 0.0f);
			glEndList();


			glNewList(listBaseCenteredXZ+i, GL_COMPILE);
				glBegin(GL_QUADS);

					glTexCoord2d(texLeft, texBot); 
					glVertex3f(tmpLeft, 0.0f,  tmpBot);
					
					glTexCoord2d(texLeft, texTop); 
					glVertex3f(tmpLeft, 0.0f,  tmpTop);

					glTexCoord2d(texRight, texTop);
					glVertex3f(tmpRight, 0.0f,  tmpTop);

					glTexCoord2d(texRight, texBot); 
					glVertex3f(tmpRight, 0.0f,  tmpBot);
				glEnd();
				//glTranslatef(charWidth[i], 0.0f, 0.0f);
			glEndList();

		}

	}

	// build the SPACE list
	
	float tmpW = (float)fontDefData[32][3] / (float)charHeightPixels;
	charWidth[32] = tmpW;

	glNewList(listBase+32, GL_COMPILE);
		glTranslatef(tmpW, 0.0f, 0.0f);
	glEndList();


	glNewList(listBaseCentered+32, GL_COMPILE);
		glTranslatef(tmpW, 0.0f, 0.0f);
	glEndList();

	glNewList(listBaseCenteredXZ+32, GL_COMPILE);
		glTranslatef(tmpW, 0.0f, 0.0f);
	glEndList();

}

//-------------------printText2D -----------------------
//	draws text in 2D mode
//--------------------------------------------------------
void AFont::printText2D( char* _text, float _x, float _y, float _size )
{
	bind();

	glPushMatrix();
		glTranslatef( _x, _y, 0.0f );
		glScaled( _size, -_size, 0.0 );
		printTextLine( _text );
	glPopMatrix();

	unBind();
}

//-------------------printChar -----------------------
//	draws a char
//--------------------------------------------------------
void AFont::printChar( char c ) 
{
	glCallList( listBase+c );
}

//-------------------printCharCentered -----------------------
//	draws a char with the centered listbase
//--------------------------------------------------------
void AFont::printCharCentered( char c) 
{

	glCallList( listBaseCentered+c);
}

//-------------------printCharCenteredXZ -----------------------
//	draws a char with the centered XZ listbase
//--------------------------------------------------------
void AFont::printCharCenteredXZ( char c) 
{

	glCallList( listBaseCenteredXZ+c);
}



//-------------------printTextLine -----------------------
//	draws text from a char array, does not check for \n 
//--------------------------------------------------------
void AFont::printTextLine( char *str ) 
{

	int strLength = strlen(str);
	glPushMatrix();
		glPushAttrib(GL_LIST_BIT);
			glListBase(listBase);
			glCallLists( strLength, GL_UNSIGNED_BYTE, str);
		glPopAttrib();
	glPopMatrix();

}

//-------------------stringWidth -----------------------
//	get the width of a string at a particular size
//--------------------------------------------------------
float AFont::stringWidth( char* _text, float _size )
{
	float tmpW = 0.0f;
	int strLength = strlen(_text);
	for( int i = 0; i < strLength; i++ )
	{
		tmpW += charWidth[_text[i]] + extraSpacing;
	}

	return tmpW * _size;
}


void AFont::readAndParseFontDefFile( char* fontDefFileName ) 
{

	// create in stream
	char buffer[256];
	ifstream examplefile (fontDefFileName);
	if (! examplefile.is_open())
		{ cout << "Error opening file"; exit (1); }

	// loop trough lines
	while (!examplefile.eof())
	{
		examplefile.getline (buffer,100);
		actOnTextCommand(buffer);
	}

}

void AFont::cleanupFontDefData( int** fdef ) 
{

	baseLine = fdef[0][0];
	int dataAmount = fdef[0][1];
	int c = 0;
	for (int i = 1; i < dataAmount; i++) 
	{
		c = fdef[i][0];
		fontDefData[c][0] = fdef[i][1];
		fontDefData[c][1] = fdef[i][2];
		fontDefData[c][2] = fdef[i][3];
		fontDefData[c][3] = fdef[i][4];
		fontDefData[c][4] = fdef[i][5];	
	}

	delete fdef;
}

////////////////////////////////////////////////////////////////
// creates the data we need from the coordinates in fontDefData
void AFont::createCharPointData( void ) 
{

	//
	//*** ORDER:  top, bot, left, right, kern ***//
	//


	charHeightPixels = 0; 	// fint out char height in pixels, then loop trough data to create normalized charWidths

	// we're not sure what the fist valid character is, so loop until we find it
	// maybe just put this in the file in the future
	int i = 0;
	while (charHeightPixels == 0) 
	{
		if (i > maxDataLength) { printf("none of the characters have a height!"); break;}
		charHeightPixels = fontDefData[i][1]-fontDefData[i][0];
		i++;
	}

	//printf(">>>>> %i\n", charHeightPixels);

	//*** create charWidths and charHeights ( all 1.0f for now) ***//
	float charHeightPixelsF = (float)charHeightPixels;
	for( i = 0; i < maxDataLength; i++) 
	{ 
		if (fontDefData[i][4] != 9999) 
		{
			charWidth[i] = (float)(fontDefData[i][3] - fontDefData[i][2]) / charHeightPixelsF;
			charHeight[i] = 1.0f;
		} 
		else 
		{
			charWidth[i] = 	0.0f;
			charHeight[i] = 0.0f;			
		}
		//printf("%i: %f,%f\n", i, charWidth[i], charHeight[i] );
	}

	extraSpacing = extraSpacingPixels/charHeightPixelsF;


	/*** create vertex points to use later in the display list, we use the baseline info 
		 to find out how far down we need to put it, as all height are 1.0f for now, 
		 we just precompute points are in opengl QUAD order ***/

	float tmpBot = -((float)baseLine/charHeightPixelsF);
	float tmpTop = 1.0f+tmpBot;
	float tmpLeft = 0.0f;
	float tmpRight = 0.0f;
	float tmpKernOffset = 0.0f;
	//printf("%f, %f", tmpTop, tmpBot);	
	for( i = 0; i < maxDataLength; i++) 
	{ 

		if (fontDefData[i][4] != 9999) 
		{

			if (fontDefData[i][4] != 0) 
			{

				tmpKernOffset = (float)(fontDefData[i][4]) / charHeightPixelsF;

				charWidth[i] -= tmpKernOffset;
				tmpLeft = -tmpKernOffset;
				tmpRight = charWidth[i]; 
			} 
			else 
			{
				tmpLeft = 0.0f;
				tmpRight = charWidth[i]; 
			}

			// left bottom
			charPoints[i][0][0] = tmpLeft;
			charPoints[i][0][1] = tmpBot;

			// left top
			charPoints[i][1][0] = tmpLeft;
			charPoints[i][1][1] = tmpTop;

			// right top
			charPoints[i][2][0] = tmpRight;
			charPoints[i][2][1] = tmpTop;

			// right bottom
			charPoints[i][3][0] = tmpRight;
			charPoints[i][3][1] = tmpBot;
		} 
	}

}


void AFont::actOnTextCommand( char* buf ) 
{

	char *token[60];
	int a = 0;
	token[a++] = strtok(buf," ");
	while(token[a++] = strtok(NULL," "));
	a--;

	//cout << "amount:" << a << endl;

	// ascii code, top, bot, left, right, kerning
	int c, top, bot, left, right, kern;

	if (a > 1) {

		switch(token[0][0]) 
		{
			
			case 67: // C (character)

				c	  = atoi ( token[1] );
				top   = atoi ( token[2] ); 
				bot   = atoi ( token[3] ); 
				left  = atoi ( token[4] ); 
				right = atoi ( token[5] ); 
				kern  = atoi ( token[6] ); 

				fontDefData[c][0] = top;
				fontDefData[c][1] = bot;
				fontDefData[c][2] = left;
				fontDefData[c][3] = right;
				fontDefData[c][4] = kern;

				//cout << "char:" << c << "   top:" << top << "   bot:" << bot << "   left:" << left << "   right:" << right << "   kern:" << kern << endl;

				break;
			case 84: // T (texture filename)
				textureFileName = token[1];
				//cout << "textureFileName: " << textureFileName << endl;
				break;
			case 66: // B (baseLine)
				baseLine = atoi ( token[1] );
				//cout << "baseLine:" << baseLine << endl;
				break;	
			case 47: // / (comment)
				// Just break
				break;
			default:
				break;			

		}
	}
}

