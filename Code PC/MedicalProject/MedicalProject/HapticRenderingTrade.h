#pragma once
#include <mutex>

class HapticRenderingTrade
{

public:
	HapticRenderingTrade();
	~HapticRenderingTrade();

	static std::mutex ftfInsideIndexMutex;
	static std::mutex forTorMutex;
	static std::mutex hapRendRateMutex;

	static int ftfInsideIndex;
	static double hapticFor;
	static double hapticTor;
	static double hapRendRate;

	static void setftfInsideIndex(int);
	static int getftfInsideIndex(void);

	static void setForTor(double, double);
	static double * getForTor(void);

	static void setHapRendRate(double);
	static double getHapRendRate(void);

};

