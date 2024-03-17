#include "imageprocessingtrade.h"

#pragma region StaticVariabless
std::mutex ImageProcessingTrade::updateStatusMutex;
std::mutex ImageProcessingTrade::ipAngMutex;
std::mutex ImageProcessingTrade::ipResMutex;
std::mutex ImageProcessingTrade::gwPosMutex;
std::mutex ImageProcessingTrade::ftDrawMutex;
std::mutex ImageProcessingTrade::tipBendAngMutex;
std::mutex ImageProcessingTrade::ftfAngleMutex;
std::mutex ImageProcessingTrade::ftfMaxIndexMutex;
std::mutex ImageProcessingTrade::ftfTypeMutex;
std::mutex ImageProcessingTrade::ftfStrtPosProfile_XMutex;
std::mutex ImageProcessingTrade::ftfStrtPosProfile_YMutex;
std::mutex ImageProcessingTrade::ftfTypeProfileMutex;
std::mutex ImageProcessingTrade::ftfAngProfileMutex;
std::mutex ImageProcessingTrade::imgProcRateMutex;

bool   ImageProcessingTrade::updateStatus = false;
double ImageProcessingTrade::ipAng = 0.0;
double ImageProcessingTrade::ipRes = 0.0;
double ImageProcessingTrade::gwPos[2] = { 0.0, };
bool   ImageProcessingTrade::ftDrawStatus = false;
double ImageProcessingTrade::tipBendAng = 0.0;
double ImageProcessingTrade::ftfAngle = 0.0;
int    ImageProcessingTrade::ftfMaxIndex = 0;
char   ImageProcessingTrade::ftfType = 'X';
double ImageProcessingTrade::ftfStrtPosProfile_X[20] = { 0.0, };
double ImageProcessingTrade::ftfStrtPosProfile_Y[20] = { 0.0, };
char   ImageProcessingTrade::ftfTypeProfile[20] = { };
double ImageProcessingTrade::ftfAngProfile[20] = { 0.0, };
double ImageProcessingTrade::imgProcRate;
#pragma endregion

ImageProcessingTrade::ImageProcessingTrade()
{
	imgData = NULL;
}

ImageProcessingTrade::~ImageProcessingTrade()
{
	if (imgData != NULL) {
		cvReleaseImage(&imgData);
	}
}

void ImageProcessingTrade::writeImage(IplImage * data)
{
	imgMutex.lock();
	if (imgData != NULL) {
		cvReleaseImage(&imgData);
	}

	imgData = cvCreateImage(cvSize(data->width, data->height), 8, 1);
	cvCopy(data, imgData);
	imgMutex.unlock();
}

IplImage * ImageProcessingTrade::getImageAccess(void)
{
	imgMutex.lock();
	return imgData;
}

void ImageProcessingTrade::releaseAccess(void)
{
	imgMutex.unlock();
}

void ImageProcessingTrade::setUpdateStatus(bool val)
{
	updateStatusMutex.lock();
	updateStatus = val;
	updateStatusMutex.unlock();
}

bool ImageProcessingTrade::getUpdateStatus(void)
{
	updateStatusMutex.lock();
	bool ret = updateStatus;
	updateStatusMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setIPAngVal(double val)
{
	ipAngMutex.lock();
	ipAng = val;
	ipAngMutex.unlock();
}

double ImageProcessingTrade::getIPAngVal(void)
{
	ipAngMutex.lock();
	double ret = ipAng;
	ipAngMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setIPResVal(double val)
{
	ipResMutex.lock();
	ipRes = val;
	ipResMutex.unlock();
}

double ImageProcessingTrade::getIPResVal(void)
{
	ipResMutex.lock();
	double ret = ipRes;
	ipResMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setGWPos(double val1, double val2)
{
	gwPosMutex.lock();
	gwPos[0] = val1;
	gwPos[1] = val2;
	gwPosMutex.unlock();
}

double * ImageProcessingTrade::getGWPos(void)
{
	gwPosMutex.lock();
	double * ret = new double[2];
	ret[0] = gwPos[0];
	ret[1] = gwPos[1];
	gwPosMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTDrawStatus(bool val)
{
	ftDrawMutex.lock();
	ftDrawStatus = val;
	ftDrawMutex.unlock();
}

bool ImageProcessingTrade::getFTDrawStatus(void)
{
	ftDrawMutex.lock();
	bool ret = ftDrawStatus;
	ftDrawMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setTipBendAng(double val)
{
	tipBendAngMutex.lock();
	tipBendAng = val;
	tipBendAngMutex.unlock();
}

double ImageProcessingTrade::getTipBendAng(void)
{
	tipBendAngMutex.lock();
	double ret = tipBendAng;
	tipBendAngMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFMaxIndex(int val)
{
	ftfMaxIndexMutex.lock();
	ftfMaxIndex = val;
	ftfMaxIndexMutex.unlock();
}

int ImageProcessingTrade::getFTFMaxIndex(void)
{
	ftfMaxIndexMutex.lock();
	int ret = ftfMaxIndex;
	ftfMaxIndexMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFType(char val)
{
	ftfTypeMutex.lock();
	ftfType = val;
	ftfTypeMutex.unlock();
}

char ImageProcessingTrade::getFTFType(void)
{
	ftfTypeMutex.lock();
	char ret = ftfType;
	ftfTypeMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFAngle(double val)
{
	ftfAngleMutex.lock();
	ftfAngle = val;
	ftfAngleMutex.unlock();
}

double ImageProcessingTrade::getFTFAngle(void)
{
	ftfAngleMutex.lock();
	double ret = ftfAngle;
	ftfAngleMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFStrtPosProfile_X(double * val)
{
	ftfStrtPosProfile_XMutex.lock();
	for (int i = 0; i < 20; i++)
		ftfStrtPosProfile_X[i] = val[i];
	ftfStrtPosProfile_XMutex.unlock();
}

double * ImageProcessingTrade::getFTFStrtPosProfile_X(void)
{
	double * ret;
	ftfStrtPosProfile_XMutex.lock();
	ret = new double[20];
	for (int i = 0; i < 20; i++)
		ret[i] = ftfStrtPosProfile_X[i];
	ftfStrtPosProfile_XMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFStrtPosProfile_Y(double * val)
{
	ftfStrtPosProfile_YMutex.lock();
	for (int i = 0; i < 20; i++)
		ftfStrtPosProfile_Y[i] = val[i];
	ftfStrtPosProfile_YMutex.unlock();
}

double * ImageProcessingTrade::getFTFStrtPosProfile_Y(void)
{
	double * ret;
	ftfStrtPosProfile_YMutex.lock();
	ret = new double[20];
	for (int i = 0; i < 20; i++)
		ret[i] = ftfStrtPosProfile_Y[i];
	ftfStrtPosProfile_YMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFTypeProfile(char * val)
{
	ftfTypeProfileMutex.lock();
	for (int i = 0; i < 20; i++)
		ftfTypeProfile[i] = val[i];
	ftfTypeProfileMutex.unlock();
}

char * ImageProcessingTrade::getFTFTypeProfile(void)
{
	char * ret;
	ftfTypeProfileMutex.lock();
	ret = new char[20];
	for (int i = 0; i < 20; i++)
		ret[i] = ftfTypeProfile[i];
	ftfTypeProfileMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setFTFAngleProfile(double * val)
{
	ftfAngProfileMutex.lock();
	for (int i = 0; i < 20; i++)
		ftfAngProfile[i] = val[i];
	ftfAngProfileMutex.unlock();
}

double * ImageProcessingTrade::getFTFAngleProfile(void)
{
	double * ret;
	ftfAngProfileMutex.lock();
	ret = new double[20];
	for (int i = 0; i < 20; i++)
		ret[i] = ftfAngProfile[i];
	ftfAngProfileMutex.unlock();
	return ret;
}

void ImageProcessingTrade::setImgProcRate(double val)
{
	imgProcRateMutex.lock();
	imgProcRate = val;
	imgProcRateMutex.unlock();
}

double ImageProcessingTrade::getImgProcRate(void)
{
	imgProcRateMutex.lock();
	double ret = imgProcRate;
	imgProcRateMutex.unlock();
	return ret;
}
