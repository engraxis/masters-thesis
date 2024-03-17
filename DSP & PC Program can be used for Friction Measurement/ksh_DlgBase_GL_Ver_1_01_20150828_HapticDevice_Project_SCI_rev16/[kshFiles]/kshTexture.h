#ifndef __KSHTEXTURE_H__
#define __KSHTEXTURE_H__

#include "CImg.h"
using namespace cimg_library;

#pragma comment(lib, "glaux.lib")

const bool TEXTURE_DEBUG = false;

//Define a new structure to hold the image information
typedef struct _ImageDataRec {
	int rowSpan;
	int sizeX;
	int sizeY;
	unsigned char *data;
} ImageDataRec;


class lwtTexture {
	public:
		lwtTexture();
		~lwtTexture();
		bool LoadTexture(char *mFileName, UINT &mTexture);
		bool FileExist(char *mFileName);
	private:
};


void CImageSwap(CImg<unsigned char>* Dest, CImg<unsigned char>* Src);

#endif
