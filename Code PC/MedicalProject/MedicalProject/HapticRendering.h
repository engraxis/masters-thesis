#pragma once

#include <thread>
#include "imageprocessing.h"
#include "SerialCommunicationTrade.h"
#include "HapticRenderingTrade.h"
#include <Windows.h>

class HapticRendering
{
private:
	std::thread * myHapticThread;
	bool quitHapticThread;

	#define IP_ARR_SIZE 4000

	static double currIPAng;	static double currIPRes;
	static double preIPAng;		static double preIPRes;
	static char trendOne;		static char trendOneb;
	static char trendTwo;		static char trendTwob;
	static double ipArr[IP_ARR_SIZE];
	static bool isSgnChanged;
	static double ipPOI;
	static int ipArrIndex;
	static void chkIPSgnChange(void);
	void hapticThread(void);
	static double torGain[2];
	static double torGainLinearized[5];
	static double stiffnessVal[5];
	

public:
	HapticRendering();
	~HapticRendering();

	static bool isHapticThreadRunning;
	

	void createHapticThread(void);
	void endHapticThread(void);
	
};

