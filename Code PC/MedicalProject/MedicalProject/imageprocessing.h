#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include "qtimer.h"
#include <thread>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\photo\photo.hpp"
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "AdditionalFiles\frangi.h"
#include "SerialCommunicationTrade.h"
#include "imageprocessingtrade.h"
#include "gsl/gsl_multifit.h"
#include "HapticRenderingTrade.h"

class ImageProcessing : public QObject
{
	Q_OBJECT

public:
	ImageProcessing(QObject *parent);
	~ImageProcessing();

public:
	static void initializeImageProcessing(void);
	void createImgProcThread(void);

	static ImageProcessingTrade * oImgTradeAngio;	//These pointers are public because they are being accessed from childwindow.cpp
	static ImageProcessingTrade * oImgTradeGWSeg;
	static ImageProcessingTrade * oImgTradeCrp;
	static ImageProcessingTrade * oImgTradeXRay;


private:
	#define BUILDIN_CAM 0
	#define WEBCAM 1
	#define TANGENTSTARTY 40
	#define TANGENTENDY 20
	#define TANGENT_X 61
	#define NO_OF_FT_FIELDS 20
	#define	wndSize 31
	#define PIXEL_YX(img, y, x)	(unsigned char &)img->imageData[(y)*img->widthStep + (x)]

	bool quitImgProcThread;

	static CvCapture * capture;
	static IplImage * bgrImg;
	static IplImage * imgAngio;
	static IplImage * imgXRay;
	static IplImage * staticAngio;
	static CvPoint gwTipLocFImg;
	static CvPoint gwTipLocCImg;
	static double innerProdAng;
	static double innerProdRes;
	static CvScalar white;

	static int noOfEndPoints;
	static bool isCrpPossible;
	static CvRect ROI;
	static IplImage * tipCrpd;
	static IplImage * tipCrpd_Prev;
	static IplImage * tipCrpd_Diff;
	static double func_val[5];
	static int tipBend_XYIndex;
	static double TipBend_X[32];
	static double TipBend_Y[32];
	static double recordedSamples[5];
	static int nearVFIndex;
	static bool isAngioAcq;
	static double ftFieldVect_CompX[NO_OF_FT_FIELDS];
	static double ftFieldVect_CompY[NO_OF_FT_FIELDS];
	static double ftFieldVectLen[NO_OF_FT_FIELDS];
	static double ftFieldVectAngD[NO_OF_FT_FIELDS];
	static double ftFieldVectAngR[NO_OF_FT_FIELDS];
	static char ftFieldType;
	static char ftfType[NO_OF_FT_FIELDS];
	static double ftfStartPosX[NO_OF_FT_FIELDS];
	static double ftfStartPosY[NO_OF_FT_FIELDS];
	static double ftfEndPosX[NO_OF_FT_FIELDS];
	static double ftfEndPosY[NO_OF_FT_FIELDS];
	static double mstrTwist_PrevForIP;
	static int ftfCurrentIndex;

	static double ipAng_FrstFilt;
	static double ipRes_FrstFilt;
	static double ipAng_ScndFilt;
	static double ipRes_ScndFilt;

private: 
	static cv::Mat frangiFilter(IplImage *);
	static IplImage * thinningGonzalez(IplImage *);
	static void gwTipDetection(IplImage *, char);					//char: 'C' for cropped image, 'F' for full image
	static void setImageRotation(void);
	static double curveFtng_Tangent(void);
	static bool polynomialfit(int, int, double *, double *, double *);
	static double jitterFilter(double);							//For IIR - Low Pass Filter
	static void jitterFilter(void);								//Our custom filter to remove abrupt signal

	std::thread* myImgProcThread;
	void imgProThread(void);
	static void mouseCallBack(int , int, int, int, void * );

public:
	void terminateAll(void);
};

#endif // IMAGEPROCESSING_H
