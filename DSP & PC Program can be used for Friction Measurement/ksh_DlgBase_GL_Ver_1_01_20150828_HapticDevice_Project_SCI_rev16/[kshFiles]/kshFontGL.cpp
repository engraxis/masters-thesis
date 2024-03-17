#include <GL/glut.h>
#include "kshFontGL.h"




/*>>>>>>>>>>>>>>>>>>>>>>>    Usage   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
char buffer[500];		
beginRenderText(WSIZEX, WSIZEY); 
	glColor3f(YELLOW);
	sprintf(buffer, "ROTX=%3.1f, ROTY=%3.1f", ROTX, ROTY);
	renderText(500, 715, BITMAP_FONT_TYPE_HELVETICA_12, buffer);
endRenderText();
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/




void beginRenderText(int nWindowWidth, int nWindowHeight) {
    // Push back and cache the current state of pixel alignment.
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );
    glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Push back and cache the current state of depth testing and lighting
    // and then disable them.
    glPushAttrib( GL_TEXTURE_BIT | GL_DEPTH_TEST | GL_LIGHTING );

    glDisable( GL_TEXTURE_2D );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    // Push back the current matrices and go orthographic for text rendering.
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, (double)nWindowWidth, (double)nWindowHeight, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
}


void endRenderText(void) {
    // Pop everything back to what ever it was set to before we started 
    // rendering text to the screen.

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopClientAttrib();

    glPopAttrib();
}


void renderText(float x, float y, BitmapFontType fontType, char *string) {
    glRasterPos2f(x, y);

    const BitmapFontData* font = getBitmapFontDataByType( fontType );
    const unsigned char* face;
    char *c = '\0';

    for(c = string; *c != '\0'; ++c) {
        // Find the character face that we want to draw.
        face = font->Characters[*c-1];

        glBitmap(face[0], font->Height,    // The bitmap's width and height
                  font->xorig, font->yorig, // The origin in the font glyph
                  (float)(face[0]), 0.0,    // The raster advance -- inc. x,y
                  (face+1));               // The packed bitmap data...
    }
}



const BitmapFontData* getBitmapFontDataByType(BitmapFontType font) {
    if( font == BITMAP_FONT_TYPE_8_BY_13 )
        return &FontFixed8x13;
    if( font == BITMAP_FONT_TYPE_9_BY_15 )
        return &FontFixed9x15;
    if( font == BITMAP_FONT_TYPE_HELVETICA_10 )
        return &FontHelvetica10;
    if( font == BITMAP_FONT_TYPE_HELVETICA_12 )
        return &FontHelvetica12;
    if( font == BITMAP_FONT_TYPE_HELVETICA_18 )
        return &FontHelvetica18;
    if( font == BITMAP_FONT_TYPE_TIMES_ROMAN_10 )
        return &FontTimesRoman10;
    if( font == BITMAP_FONT_TYPE_TIMES_ROMAN_24 )
        return &FontTimesRoman24;

    return 0;
}