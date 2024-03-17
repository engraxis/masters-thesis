#include <gl/glut.h>
#include "kshImage.h"

/********************   Usage  *********************
lwtImage Image(WindowSizeX, WindowSizeY);
Image.Load("FileName");
Image.Display();
****************************************************/



lwtImage::lwtImage() {
	sprintf(FName, "");
	WSizeX=-1; WSizeY=-1;
	SizeX=-1; SizeY=-1;
	TName=-1;
}

lwtImage::lwtImage(float wsizex, float wsizey) {
	sprintf(FName, "");
	WSizeX=wsizex; WSizeY=wsizey;
	SizeX=-1; SizeY=-1;
	TName=-1;
}

lwtImage::~lwtImage() {

}

bool lwtImage::FileExist(char* FName) {
	FILE *File = NULL;

	if (!FName)
		return false;
	File = fopen(FName,"r");
	if (File==NULL)
		return false;
	else
		fclose(File);
	return true;
}

bool lwtImage::Load(char* FName) {
	CImg<unsigned char> cImage(FName);
	CImg<unsigned char> mImage(cImage.width, cImage.height, cImage.depth, cImage.dim);
	SwapImage(&mImage, &cImage);

	sprintf(lwtImage::FName, FName);
	SizeX=cImage.width; SizeY=cImage.height;
	
	if(FileExist(FName)) {
		glGenTextures(1, &TName);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, TName);		
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, mImage.width, mImage.height, GL_RGB, GL_UNSIGNED_BYTE, mImage.data);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);		
	}
	
	mImage.empty();
	cImage.empty();

	if(WSizeX>0) {
		if(SizeY/SizeX<WSizeY/WSizeX) {	
			width = WSizeX;
			height = SizeY*(WSizeX/SizeX);
			Ox=0; Oy=(WSizeY-height)/2;
		}
		else {
			width = SizeX*(WSizeY/SizeY);
			height = WSizeY;
			Ox=(WSizeX-width)/2; Oy=0;
		}
	}	
	return true;
}



void lwtImage::SwapImage(CImg<unsigned char>* Dest, CImg<unsigned char>* Src) {
	for(unsigned int i=0; i<Src->width; i++) {
		for(unsigned int j=0; j<Src->height; j++) {
			for(unsigned int k=0; k<Src->dim; k++) {
				Dest->data[Src->dim*(Src->width*j+i)+k] 
					= Src->data[Src->width*(Src->height-j-1)+i+k*Src->width*Src->height];
			}
		}
	}
}


void lwtImage::Display(float Ox, float Oy, float Oz, float width, float height) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(Ox, Oy, Oz);
	glTexCoord2f(0.0, 1.0); glVertex3f(Ox, Oy+height, Oz);
	glTexCoord2f(1.0, 1.0); glVertex3f(Ox+width, Oy+height, Oz);
	glTexCoord2f(1.0, 0.0); glVertex3f(Ox+width, Oy, Oz);
	glEnd();
}


void lwtImage::Display(float Wwidth, float Wheight) {
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );
    glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glPushAttrib( GL_TEXTURE_BIT | GL_DEPTH_TEST | GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

//	glViewport(0, 0, Wwidth, Wheight);
//	glViewport(Ox, Oy, width, height);
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
	glOrtho(-20, Wwidth+20, -20, Wheight+20, -1, 1);
//	glOrtho(0, Wwidth, 0, Wheight, -1, 1);
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
	
	if(SizeY/SizeX<Wheight/Wwidth) {	
		width = Wwidth;
		height = SizeY*(Wwidth/SizeX);
		Ox=0; Oy=(Wheight-height)/2;
	}
	else {
		width = SizeX*(Wheight/SizeY);
		height = Wheight;
		Ox=(Wwidth-width)/2; Oy=0;
	}
//	glRasterPos2i(Ox,Oy);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(Ox, Oy, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(Ox, Oy+height, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(Ox+width, Oy+height, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(Ox+width, Oy, 0);
	glEnd();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopClientAttrib();
    glPopAttrib();
}
/*
void lwtImage::Display(float Wwidth, float Wheight) {
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );
    glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glPushAttrib( GL_TEXTURE_BIT | GL_DEPTH_TEST | GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

//	glViewport(0, 0, Wwidth, Wheight);
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
	glOrtho(-20, Wwidth+20, -20, Wheight+20, -1, 1);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
	
	if(SizeY/SizeX<Wheight/Wwidth) {	
		width = Wwidth;
		height = SizeY*(Wwidth/SizeX);
		Ox=0; Oy=(Wheight-height)/2;
	}
	else {
		width = SizeX*(Wheight/SizeY);
		height = Wheight;
		Ox=(Wwidth-width)/2; Oy=0;
	}
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(Ox, Oy, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(Ox, Oy+height, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(Ox+width, Oy+height, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(Ox+width, Oy, 0);
	glEnd();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopClientAttrib();
    glPopAttrib();
}
*/

void lwtImage::Display(void) {
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );
    glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glPushAttrib( GL_TEXTURE_BIT | GL_DEPTH_TEST | GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

//	glViewport(0, 0, Wwidth, Wheight);
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
	glOrtho(-20, WSizeX+20, -20, WSizeY+20, -1, 1);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(Ox, Oy, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(Ox, Oy+height, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(Ox+width, Oy+height, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(Ox+width, Oy, 0);
	glEnd();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopClientAttrib();
    glPopAttrib();
}