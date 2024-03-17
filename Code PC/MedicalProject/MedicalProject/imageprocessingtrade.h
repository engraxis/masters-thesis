#ifndef IMAGEPROCESSINGTRADE_H
#define IMAGEPROCESSINGTRADE_H

#include <QObject>
#include <mutex>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\photo\photo.hpp"
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <Windows.h>

class ImageProcessingTrade 
{
public:
	ImageProcessingTrade();
	~ImageProcessingTrade();

private:
	std::mutex imgMutex;
	IplImage * imgData;


	//Following variables are related to virtual fixtures
	static std::mutex ftfTypeMutex;
	static std::mutex ftfMaxIndexMutex;
	static int ftfMaxIndex;
	static char ftfType;

	static char ftfTypeProfile[20];
	static double ftfStrtPosProfile_X[20];
	static double ftfStrtPosProfile_Y[20];
	static double ftfAngProfile[20];

	static std::mutex ftfTypeProfileMutex;
	static std::mutex ftfStrtPosProfile_XMutex;
	static std::mutex ftfStrtPosProfile_YMutex;
	static std::mutex ftfAngProfileMutex;
	static std::mutex updateStatusMutex;
	static std::mutex ipAngMutex;
	static std::mutex ipResMutex;
	static std::mutex gwPosMutex;
	static std::mutex ftDrawMutex;
	static std::mutex tipBendAngMutex;
	static std::mutex ftfAngleMutex;
	static std::mutex imgProcRateMutex;

	static bool updateStatus;
	static double ipAng;
	static double ipRes;
	static double gwPos[2];
	static bool ftDrawStatus;
	static double tipBendAng;
	static double ftfAngle;
	static double imgProcRate;
	

public:
	void writeImage(IplImage *);
	IplImage * getImageAccess(void);
	void releaseAccess(void);

	/**
	* Thread safe method to set the update status.
	* The update status indicates.....
	*/
	static void setUpdateStatus(bool);
	static bool getUpdateStatus(void);

	static void setIPAngVal(double);
	static double getIPAngVal(void);

	static void setIPResVal(double);
	static double getIPResVal(void);

	static void setGWPos(double, double);
	static double * getGWPos(void);

	static void setFTDrawStatus(bool);
	static bool getFTDrawStatus(void);

	static void setTipBendAng(double);
	static double getTipBendAng(void);

	static void setFTFMaxIndex(int);
	static int getFTFMaxIndex(void);

	static void setFTFType(char);
	static char getFTFType(void);

	static void setFTFAngle(double);
	static double getFTFAngle(void);

	static void setFTFStrtPosProfile_X(double *);
	static double * getFTFStrtPosProfile_X(void);

	static void setFTFStrtPosProfile_Y(double *);
	static double * getFTFStrtPosProfile_Y(void);

	static void setFTFTypeProfile(char *);
	static char * getFTFTypeProfile(void);

	static void setFTFAngleProfile(double *);
	static double * getFTFAngleProfile(void);

	static void setImgProcRate(double);
	static double getImgProcRate(void);
};

#endif // IMAGEPROCESSINGTRADE_H
