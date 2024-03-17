#ifndef __KSHIMAGE_H__
#define __KSHIMAGE_H__

#include "CImg.h"



class lwtImage {
	public:
		char FName[500];
		int SizeX, SizeY;
		UINT TName;
		float width, height, Ox, Oy;
		float WSizeX, WSizeY;

		lwtImage();
		lwtImage(float wsizex, float wsizey);
		~lwtImage();
		void SwapImage(CImg<unsigned char>* Dest, CImg<unsigned char>* Src);
		bool FileExist(char* FName);
		bool Load(char* FName);
		void Display(void);
		void Display(float Ox, float Oy, float Oz, float width, float height);
		void Display(float Wwidth, float Wheight);
	private:
};


#endif