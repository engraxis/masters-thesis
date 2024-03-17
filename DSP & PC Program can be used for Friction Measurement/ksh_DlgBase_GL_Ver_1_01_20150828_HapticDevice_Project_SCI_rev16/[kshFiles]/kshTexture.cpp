#include <windows.h>
#include <stdio.h>
#include <gl\glaux.h>
#include <gl\glut.h>

#include "kshTexture.h"


lwtTexture::lwtTexture() {
	//nothing
}

lwtTexture::~lwtTexture() {
	//nothing	
}

bool lwtTexture::FileExist(char *mFileName) {
	FILE *File = NULL;
	if (!mFileName)
		return false;
	File = fopen(mFileName,"r");

	if (File==NULL)
		return false;
	else 
		fclose(File);
	return true;
}

bool lwtTexture::LoadTexture(char *mFileName, UINT &mTexture) {	

	if(TEXTURE_DEBUG)
		MessageBox(NULL, mFileName, "Loading Texture", MB_OK);

	if (!FileExist(mFileName)) {	
		if (TEXTURE_DEBUG) 
			MessageBox(NULL, "File does not Exist", "Loading Texture", MB_OK);
		return false;	
	}	

	CImg<unsigned char> cImage(mFileName);
	CImg<unsigned char> mImage(cImage.width, cImage.height, cImage.depth, cImage.dim);
	CImageSwap(&mImage, &cImage);

	if(FileExist(mFileName)) {
		glGenTextures(1, &mTexture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, mTexture);		
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, mImage.width, mImage.height, GL_RGB, GL_UNSIGNED_BYTE, mImage.data);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);		
	}

	mImage.empty();
	cImage.empty();
	
	if (TEXTURE_DEBUG)
		MessageBox(NULL, "Done", "Loading Texture", MB_OK);
	
	return true;
}


void CImageSwap(CImg<unsigned char>* Dest, CImg<unsigned char>* Src) {
	for(unsigned int i=0; i<Src->width; i++) {
		for(unsigned int j=0; j<Src->height; j++) {
			for(unsigned int k=0; k<Src->dim; k++) {
				Dest->data[Src->dim*(Src->width*j+i)+k] 
					= Src->data[Src->width*(Src->height-j-1)+i+k*Src->width*Src->height];
			}
		}
	}
}