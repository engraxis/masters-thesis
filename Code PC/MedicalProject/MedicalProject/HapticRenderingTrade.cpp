#include "HapticRenderingTrade.h"

std::mutex HapticRenderingTrade::ftfInsideIndexMutex;
std::mutex HapticRenderingTrade::forTorMutex;
std::mutex HapticRenderingTrade::hapRendRateMutex;

int HapticRenderingTrade::ftfInsideIndex = 0;
double HapticRenderingTrade::hapticFor = 0.0;
double HapticRenderingTrade::hapticTor = 0.0;
double HapticRenderingTrade::hapRendRate = 0.0;

HapticRenderingTrade::HapticRenderingTrade()
{
}

HapticRenderingTrade::~HapticRenderingTrade()
{
}

void HapticRenderingTrade::setftfInsideIndex(int val)
{
	ftfInsideIndexMutex.lock();
	ftfInsideIndex = val;
	ftfInsideIndexMutex.unlock();
}

int HapticRenderingTrade::getftfInsideIndex(void)
{
	ftfInsideIndexMutex.lock();
	int ret = ftfInsideIndex;
	ftfInsideIndexMutex.unlock();
	return ret;
}

void HapticRenderingTrade::setForTor(double arg1, double arg2)
{
	forTorMutex.lock();
	hapticFor = arg1;
	hapticTor = arg2;
	forTorMutex.unlock();
}

double * HapticRenderingTrade::getForTor(void)
{
	double * ret;
	ret = new double[2];
	forTorMutex.lock();
	ret[0] = hapticFor;
	ret[1] = hapticTor;
	forTorMutex.unlock();
	return ret;
}

void HapticRenderingTrade::setHapRendRate(double val)
{
	hapRendRateMutex.lock();
	hapRendRate = val;
	hapRendRateMutex.unlock();
}

double HapticRenderingTrade::getHapRendRate(void)
{
	hapRendRateMutex.lock();
	double ret = hapRendRate;
	hapRendRateMutex.unlock();
	return ret;
}
