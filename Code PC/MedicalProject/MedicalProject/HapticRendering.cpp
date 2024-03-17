#include "HapticRendering.h"

#pragma region StaticVariables
bool HapticRendering::isHapticThreadRunning = false;
double HapticRendering::currIPAng = 0.0;
double HapticRendering::currIPRes = 0.0;
double HapticRendering::preIPAng = 0.0;
double HapticRendering::preIPRes = 0.0;

char HapticRendering::trendOne;
char HapticRendering::trendOneb;
char HapticRendering::trendTwo;
char HapticRendering::trendTwob;
double HapticRendering::ipArr[IP_ARR_SIZE] = { 0.0, };
bool HapticRendering::isSgnChanged = false;
double HapticRendering::ipPOI = 0.0;
int HapticRendering::ipArrIndex = 0;
double HapticRendering::torGain[2] = { 5.0, 2.0 };
double HapticRendering::torGainLinearized[5] = { 2.0, 4.0, 7.0, 11.0, 17.0 };
double HapticRendering::stiffnessVal[5] = { 0.6, 0.04, 0.03, 0.02, 0.01 };
#pragma endregion

HapticRendering::HapticRendering()
{
}

HapticRendering::~HapticRendering()
{
}

void HapticRendering::createHapticThread(void)
{
	quitHapticThread = false;
	myHapticThread = new std::thread(&HapticRendering::hapticThread, this);	//Creating standard thread
}

void HapticRendering::endHapticThread(void)
{
	if (isHapticThreadRunning)
	{
		quitHapticThread = true;
		if (quitHapticThread)
		{
			_sleep(100);				//Wait until thread loop is finished
			myHapticThread->join();		//Wait until thread is ended
		}
	}
}

void HapticRendering::hapticThread(void)
{
	char mstrTwistDir = 'X';			//
	double mstrTwistPos = 0.0;			//
	double mstrTransPos = 0.0;			//
	double slvTwistPos = 0.0;			//
	double slvTransPos = 0.0;			//
	double prevMstrTwistPos = 0.0;		//
	double prevSlvTwistPos = 0.0;		//
	bool isClutchActive = false;		//
	double FOR[3] = { 0.0, };			//
	double TOR[3] = { 0.0, };			//

	const int maxFTF = 20;
	int ftfMaxIndex = 0;							//<-------------------Get it
	int ftfInsideIndex = 0;
	int prevftfInsideIndex = 0;
	double ftfStartPos_x[maxFTF] = { 0.0, };	//<-------------------Get it
	double ftfStartPos_y[maxFTF] = { 0.0, };	//<-------------------Get it
	char ftfType[maxFTF];
	double ftfDistance_x[maxFTF] = { 0.0, };	//
	double ftfDistance_y[maxFTF] = { 0.0, };	//
	double ftfDistance[maxFTF] = { 0.0 };		//
	double ftfAngle[maxFTF] = { 0.0, };
	char ftfInsideField = 'X';					//
	double gwTipPos_X = 0.0;					//
	double gwTipPos_Y = 0.0;					//

	char enterDir = 'X';

	bool bOptimalPosAcq = false;
	int qualifierCnt = 0;
	bool isAligned = false;
	int torqueAlgoStep = 0;
	char rotDirStep1 = 'X';
	char mstrTwistDirCom = 'X';
	double ipResStep1 = 0.0;	double ipAngStep1 = 0.0;
	double ipResStep2 = 0.0;	double ipAngStep2 = 0.0;
	double ipResStep3a = 0.0;	double ipAngStep3a = 0.0;
	double ipResStep3b = 0.0;	double ipAngStep3b = 0.0;
	double ipResStep3mid = 0.0;
	trendOne = 'X';				trendTwo = 'X';
	trendOneb = 'X';			trendTwob = 'X';
	
	ipArrIndex = 0;
	isSgnChanged = false;
	char sameDir = 'X';
	int indicator = 1.0;
	char mstrTwistDirComCont = 'X';
	bool isTipInsideField = false;
	bool isTipInsideFieldStep1 = false;
	int runCnt = 1;
	bool runTorAlgo = false;
	double mstrTwistPosStep1 = 0.0;	double mstrTwistPosStep2 = 0.0;
	bool bRunFurther = false;
	ipPOI = 0.0;
	double optimalMstrTwistPos = 0.0;
	double REF_POS_Twist = 0.0;
	double REF_POS_Translate = -28.2;

	unsigned char sndMsg[16] = { 0, };

	isHapticThreadRunning = true;

	SYSTEMTIME time;
	unsigned long int timeStart = 0;
	unsigned long int timeEnd = 0;
	unsigned long int timeElapsed = 0;

	while (!quitHapticThread)
	{
		GetSystemTime(&time);
		timeStart = (time.wSecond * 1000) + time.wMilliseconds;
		_sleep(1);
		
		isClutchActive = SerialCommunicationTrade::getClutchStatus();
		if (ImageProcessingTrade::getFTDrawStatus())				//If ft field is drawn
		{
			if (ImageProcessingTrade::getUpdateStatus())			//If new graphic update is available 
			{
#pragma region ReadVariables
				ImageProcessingTrade::setUpdateStatus(false);

				currIPAng = ImageProcessingTrade::getIPAngVal();
				currIPRes = ImageProcessingTrade::getIPResVal();

				float * tmpMtrPos = SerialCommunicationTrade::getMotorPositions();
				mstrTwistPos = (double)tmpMtrPos[0];
				mstrTransPos = (double)tmpMtrPos[1];
				slvTwistPos = (double)tmpMtrPos[2];
				slvTransPos = (double)tmpMtrPos[3];
				delete tmpMtrPos;

				double * tmpGWPos = ImageProcessingTrade::getGWPos();
				gwTipPos_X = tmpGWPos[0];
				gwTipPos_Y = tmpGWPos[1];
				delete tmpGWPos;

				//Read following variables only one time
				static bool readFTFProfiles = true;
				if (readFTFProfiles)
				{
					double * getFTFStartPosX = ImageProcessingTrade::getFTFStrtPosProfile_X();
					double * getFTFStartPosY = ImageProcessingTrade::getFTFStrtPosProfile_Y();
					char * getFTFType = ImageProcessingTrade::getFTFTypeProfile();
					double * getFTFAng = ImageProcessingTrade::getFTFAngleProfile();

					for (int i = 0; i < 20; i++)
					{
						ftfStartPos_x[i] = getFTFStartPosX[i];
						ftfStartPos_y[i] = getFTFStartPosY[i];
						ftfType[i] = getFTFType[i];
						ftfAngle[i] = getFTFAng[i];
					}
					delete getFTFStartPosX;
					delete getFTFStartPosY;
					delete getFTFType;
					delete getFTFAng;
					ftfMaxIndex = ImageProcessingTrade::getFTFMaxIndex();
					readFTFProfiles = false;
				}
#pragma endregion

				//Direction of rotation of Master Twisting
				if (mstrTwistPos > prevMstrTwistPos)
					mstrTwistDir = 'P';
				else if (mstrTwistPos < prevMstrTwistPos)
					mstrTwistDir = 'N';
				else
					mstrTwistDir = 'X';

				


				if (isClutchActive)	//If clutch button is pressed, don't render force/torque
				{
					FOR[2] = 0.0;
					TOR[2] = 0.0;
				}

				//Finding all of the distances
				for (int loop = 0; loop <= ftfMaxIndex; loop++)
				{
					ftfDistance_x[loop] = gwTipPos_X - ftfStartPos_x[loop];
					ftfDistance_y[loop] = gwTipPos_Y - ftfStartPos_y[loop];
					ftfDistance[loop]   = (double)sqrtf(float(ftfDistance_x[loop] * ftfDistance_x[loop]) + float(ftfDistance_y[loop] * ftfDistance_y[loop]));
				}

				//Check if guidewire is inside the force/torque feild 
				ftfInsideField = 'N';
				for (int loop = 0; loop <= ftfMaxIndex; loop++)
				{
					if (ftfDistance[loop] <= 30.0)
					{
						ftfInsideIndex = loop;
						HapticRenderingTrade::setftfInsideIndex(ftfInsideIndex);
						ImageProcessingTrade::setFTFType(ftfType[ftfInsideIndex]);
						ImageProcessingTrade::setFTFAngle(ftfAngle[ftfInsideIndex]);
						ftfInsideField = 'Y';
						break;
					}
					else
					{
						ftfInsideIndex = 0;
						if (ftfInsideIndex != prevftfInsideIndex)
							enterDir = 'X';
					}
				}
				prevftfInsideIndex = ftfInsideIndex;
				//If guidewire tip tangent in 2D is in first or second quadrant			
				//Finding the entry point in the field
				if (ftfInsideField == 'Y')
				{
					if (enterDir != 'Y') //Either 'N' or 'X'
					{
						if ((gwTipPos_Y - ftfStartPos_y[ftfInsideIndex]) > 0.0) //If guidewire tip is entering from top
						{
							enterDir = 'N';
						}
					}
					if (enterDir != 'N')
					{
						if (((ftfStartPos_y[ftfInsideIndex] - (gwTipPos_Y))) > 0.0) //If guidewire tip is entering from bottom
						{
							enterDir = 'Y';
						}
					}
				}

				//If guidewire is not inside the force/torque field circle, reset everything
				if (ftfInsideField == 'N')
				{
					bOptimalPosAcq = false;
					qualifierCnt = 0;
					isAligned = false;
					torqueAlgoStep = 0;
					rotDirStep1 = 'X';
					mstrTwistDirCom = 'X';
					ipResStep1 = 0.0;	ipResStep2 = 0.0;	ipResStep3a = 0.0;	ipResStep3b = 0.0;
					trendOne = 'X';		trendTwo = 'X';		trendOneb = 'X';	trendTwob = 'X';
					for (int loop = 0; loop < IP_ARR_SIZE; loop++)
						ipArr[loop] = 0.0;
					ipArrIndex = 0;
					isSgnChanged = false;
					sameDir = 'X';
					indicator = 1.0;
					mstrTwistDirComCont = 'X';
					isTipInsideField = false;
					isTipInsideFieldStep1 = false;
					runCnt = 1;
					FOR[2] = 0.0;	TOR[2] = 0.0;
					enterDir = 'X';
				}
				else if (ftfDistance[ftfInsideIndex] <= 30.0 && runCnt == 1)
				{
					isTipInsideField = true;
					isTipInsideFieldStep1 = true;
					runCnt = 2;
				}
				if (isTipInsideFieldStep1 == true)
				{
					runTorAlgo = true;
					isTipInsideFieldStep1 = false;
				}

				if (runTorAlgo == true)
				{
					torqueAlgoStep = 1;
					runTorAlgo = false;
					ipAngStep1 = currIPAng;
					ipResStep1 = currIPRes;
					TOR[2] = 0.0;
					mstrTwistDirCom = 'X';
					mstrTwistPosStep1 = mstrTwistPos;
					indicator = 0.0;
				}
#pragma region Step1
				if (torqueAlgoStep == 1)
				{
					indicator = 0.0;
					bool bManualMasterTwistMoved = false;
					if (abs(currIPAng - ipAngStep1) > 2 && abs(mstrTwistPos - mstrTwistPosStep1) >= 0.45) //When there is significant change in the tangent then run further
					{
						if (mstrTwistPos > mstrTwistPosStep1)
						{
							rotDirStep1 = 'P';
						}
						else if (mstrTwistPos < mstrTwistPosStep1)
						{
							rotDirStep1 = 'N';
						}
						bManualMasterTwistMoved = true;
						ipAngStep2 = currIPAng;
						ipResStep2 = currIPRes;
					}
					ipArr[ipArrIndex] = currIPRes; //Array will be used to check sign during rotation
					ipArrIndex++;

					if (bManualMasterTwistMoved == true)
					{
						//Check whether the inner product sign change occurred or not
						chkIPSgnChange();
						if (isSgnChanged) //If the sign has been changed then:
						{
							if (ftfType[ftfInsideIndex] == 'A')	//Current Virtual Fixture is attractive
							{
								if (ipResStep2 > ipPOI)
								{
									mstrTwistDirCom = rotDirStep1;
									sameDir = 'Y';
								}
								else if (ipResStep2 < ipPOI)
								{
									sameDir = 'N';
									if (rotDirStep1 == 'P')
										mstrTwistDirCom = 'N';
									else if (rotDirStep1 == 'N')
										mstrTwistDirCom = 'P';
								}
								else
								{
									sameDir = 'X';
									mstrTwistDirCom = 'X';
								}
							}
							else if (ftfType[ftfInsideIndex] == 'R') //Current Virtual Fixture is repulsive
							{
								if (ipResStep2 < ipPOI)
								{
									sameDir = 'Y';
									mstrTwistDirCom = rotDirStep1;
								}
								else if (ipResStep2 > ipPOI)
								{
									sameDir = 'N';
									if (rotDirStep1 == 'P')
										mstrTwistDirCom = 'N';
									else if (rotDirStep1 == 'N')
										mstrTwistDirCom = 'P';
								}
								else
								{
									mstrTwistDirCom = 'X';
									sameDir = 'X';
								}
							}
							torqueAlgoStep = 2;
							qualifierCnt = 0;
							bRunFurther = false;
							mstrTwistPosStep2 = mstrTwistPos;
							ipResStep3a = currIPRes;
							ipResStep3b = currIPRes;
						}
						else if (!isSgnChanged)
						{
							if (ftfType[ftfInsideIndex] == 'A')	//Current Virtual Fixture is attractive
							{
								if (ipResStep2 > ipResStep1)
								{
									mstrTwistDirCom = rotDirStep1;
									sameDir = 'Y';
								}
								else if (ipResStep2 < ipResStep1)
								{
									sameDir = 'N';
									if (rotDirStep1 == 'P')
										mstrTwistDirCom = 'N';
									else if (rotDirStep1 == 'N')
										mstrTwistDirCom = 'P';
								}
								else
								{
									mstrTwistDirCom = 'X';
									sameDir = 'X';
								}
							}
							else if (ftfType[ftfInsideIndex] == 'R') //Current Virtual Fixture is repulsive
							{
								if (ipResStep2 < ipResStep1)
								{
									sameDir = 'Y';
									mstrTwistDirCom = rotDirStep1;
								}
								else if (ipResStep2 > ipResStep1)
								{
									sameDir = 'N';
									if (rotDirStep1 == 'P')
										mstrTwistDirCom = 'N';
									else if (rotDirStep1 == 'N')
										mstrTwistDirCom = 'P';
								}
								else
								{
									sameDir = 'X';
									mstrTwistDirCom = 'X';
								}
							}
							torqueAlgoStep = 2;
							qualifierCnt = 0;
							bRunFurther = false;
							mstrTwistPosStep2 = mstrTwistPos;
							ipResStep3a = currIPRes;
							ipResStep3b = currIPRes;
						}
					}
				}
#pragma endregion
#pragma region Step2
				//Continuously check the inner product and disable the CorrectDirection by X to stop rendering the torque
				if (torqueAlgoStep == 2)
				{
					indicator = 1.0;
					if (!bRunFurther) //To overcome the back lash when commanded to rotate in the opposite direction
					{
						if (sameDir == 'Y')
						{
							bRunFurther = true;
							ipResStep3a = currIPRes;
							ipResStep3b = currIPRes;
						}
						else if (sameDir == 'N')
						{
							if (mstrTwistPos >= mstrTwistPosStep2)
							{
								if (abs(mstrTwistPos - mstrTwistPosStep2) >= 0.8)
								{
									bRunFurther = true;
									ipResStep3a = currIPRes;
									ipResStep3b = currIPRes;
								}
							}
							if (mstrTwistPos <= mstrTwistPosStep2)
							{
								if (abs(mstrTwistPosStep2 - mstrTwistPos) >= 0.8)
								{
									bRunFurther = true;
									ipResStep3a = currIPRes;
									ipResStep3b = currIPRes;
								}
							}
						}
						else if (sameDir == 'X')
						{
							bRunFurther = false;
							torqueAlgoStep = 3;
							optimalMstrTwistPos = mstrTwistPos;
						}
					}

					if (bRunFurther)
					{
						bool bQualifierCountOK = false;

						ipResStep3a = ipResStep3b;
						ipResStep3b = currIPRes;

						if (ftfType[0] == 'A') //Attractive Virtual Fixture: Keep rotating until the inner product starts to decrease
						{
							//	if(QualifierCount == 0)
							{
								if (ipResStep3b >= ipResStep3a) //New inner product is greater than previous inner product : Keep rotating
									qualifierCnt = 0;
								else
								{
									ipResStep3mid = ipResStep3b; //InnerProductResult_Step3Mid
									qualifierCnt++;
								}
								if (ipResStep3b >= 0.6)
									bQualifierCountOK = true;
								if (ipResStep3b >= 0.8)
									qualifierCnt = 1;
							}
						}

						if (ftfType[0] == 'R') //Attractive Virtual Fixture: Keep rotating until the inner product starts to decrease
						{
							if (qualifierCnt == 0)
							{
								if (ipResStep3b <= ipResStep3a) //New inner product is greater than previous inner product : Keep rotating
									qualifierCnt = 0;
								else
								{
									ipResStep3mid = ipResStep3b; //InnerProductResult_Step3Mid
									qualifierCnt++;
								}
								if (ipResStep3b <= 0.4)
									bQualifierCountOK = true;
							}
						}


						if (qualifierCnt >= 1 && bQualifierCountOK == true) //Check for successive two results and make decision
						{
							qualifierCnt = 0;
							isAligned = true;
							optimalMstrTwistPos = mstrTwistPos;//Storing the motor position

							if (mstrTwistDirCom == 'P')
								optimalMstrTwistPos = optimalMstrTwistPos - 1.8;//1.4;
							if (mstrTwistDirCom == 'N')
								optimalMstrTwistPos = optimalMstrTwistPos + 1.8;//1.4;

							bOptimalPosAcq = true;
						}
					}

					//If the desired bend is not acquired yet, algorithm will keep applying the torque
					if (mstrTwistDirCom == 'P')
					{
						mstrTwistDirComCont = 'P';
						if (ftfType[0] == 'A')
							TOR[2] = ((1.0 - currIPRes) * torGain[1]) + 0.2;
						else if (ftfType[0] == 'R')
							TOR[2] = ((currIPRes)* torGain[1]) + 0.2;
					}
					if (mstrTwistDirCom == 'N')
					{
						mstrTwistDirComCont = 'N';
						if (ftfType[0] == 'A')
							TOR[2] = -1.0 * (((1.0 - currIPRes) * torGain[1]) + 0.2);
						else if (ftfType[0] == 'R')
							TOR[2] = -1.0 * (((currIPRes)* torGain[1]) + 0.2);
					}
					//else
					//	TOR[2] = 0.0;
				}
#pragma endregion
				if (bOptimalPosAcq)
				{
					indicator = 0.0;
					torqueAlgoStep = 3;
				}
#pragma region step3
				if (torqueAlgoStep == 3) //Now if the optimal/desired position is achieved 
				{
					double K_Torque_Stop = 0.0;
					indicator = 0.0;
					mstrTwistDirCom = 'X'; //Now if the optimal/desired position is achieved: By Default

					if (mstrTwistPos > optimalMstrTwistPos) //If rotated CW
					{
						if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.1)
						{
							mstrTwistDirCom = 'N';
						}

						if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.1)
						{
							K_Torque_Stop = torGainLinearized[0];
							if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.2)
							{
								K_Torque_Stop = torGainLinearized[1];
								if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.25)
								{
									K_Torque_Stop = torGainLinearized[2];
									if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.3)
									{
										K_Torque_Stop = torGainLinearized[3];
										if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.35)
										{
											K_Torque_Stop = torGainLinearized[3];
											if (abs(mstrTwistPos - optimalMstrTwistPos) >= 0.4)
												K_Torque_Stop = torGainLinearized[4];
										}
									}
								}
							}
						}
					}
					else if (mstrTwistPos < optimalMstrTwistPos) //If rotated CCW
					{
						if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.1)
						{
							mstrTwistDirCom = 'P';
						}
						if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.1)
						{
							K_Torque_Stop = torGainLinearized[0];
							if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.2)
							{
								K_Torque_Stop = torGainLinearized[1];
								if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.25)
								{
									K_Torque_Stop = torGainLinearized[2];
									if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.3)
									{
										K_Torque_Stop = torGainLinearized[3];
										if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.35)
										{
											K_Torque_Stop = torGainLinearized[3];
											if (abs(optimalMstrTwistPos - mstrTwistPos) >= 0.4)
												K_Torque_Stop = torGainLinearized[4];
										}
									}
								}
							}
						}

					}
					else
					{
						mstrTwistDirCom = 'X';
					}

					if (mstrTwistDirCom == 'P')
					{
						if (ftfType[ftfInsideIndex] == 'A')
							TOR[2] = ((1.0 - currIPRes) * K_Torque_Stop) + 0.3;
						else if (ftfType[ftfInsideIndex] == 'R')
							TOR[2] = ((currIPRes)* K_Torque_Stop) + 0.3;
					}
					else if (mstrTwistDirCom == 'N')
					{
						if (ftfType[ftfInsideIndex] == 'A')
							TOR[2] = -1.0 * (((1.0 - currIPRes) * K_Torque_Stop) + 0.3);
						else if (ftfType[ftfInsideIndex] == 'R')
							TOR[2] = -1.0 * (((currIPRes)* K_Torque_Stop) + 0.3);
					}
				}
#pragma endregion
				if (isAligned == false && isTipInsideField == true)
				{
					FOR[0] = 0.0;
					FOR[1] = 0.0;
					FOR[2] = 0.0;
					double currStiffness;
					if (ftfDistance[ftfInsideIndex] >= 0)
					{
						currStiffness = stiffnessVal[0];
						if (ftfDistance[ftfInsideIndex] >= 5)
						{
							currStiffness = stiffnessVal[1];
							if (ftfDistance[ftfInsideIndex] >= 10)
							{
								currStiffness = stiffnessVal[2];
								if (ftfDistance[ftfInsideIndex] >= 15)
								{
									currStiffness = stiffnessVal[3];
									if (ftfDistance[ftfInsideIndex] >= 25)
										currStiffness = stiffnessVal[4];
								}
							}
						}
					}
					FOR[2] = -1.0*(ftfDistance[ftfInsideIndex] * currStiffness); //+0.5 to overcome friction
				}

				//As additional checks...
				if (isTipInsideField == false)
				{
					TOR[2] = 0.0;
					FOR[2] = 0.0;
					mstrTwistDirCom = 'X';
				}

				if (isAligned == true)
				{
					FOR[2] = 0.0;
				}

				if (enterDir == 'Y') //Entered the field from top to down
				{
					TOR[2] = 0.0;
					FOR[2] = 0.0;
					mstrTwistDirCom = 'X';
				}

				
				if (isClutchActive)
				{
					REF_POS_Twist = mstrTwistPos;
				}

				if (!isClutchActive)
				{
					static double Error_Translate = 0.0;
					static double ErrorChange_Translate = 0.0;
					static double ErrorPrev_Translate = 0.0;
					static double Kp_Translate = 0.3;
					static double Kd_Translate = 0.02;
					//Position Control: Translate to Reference Position 
					Error_Translate = REF_POS_Translate - mstrTransPos;
					ErrorChange_Translate = (Error_Translate - ErrorPrev_Translate) / 0.001;
					FOR[2] = Kp_Translate * Error_Translate + Kd_Translate * ErrorChange_Translate;
					ErrorPrev_Translate = Error_Translate;

					static double Error_Twist = 0.0;
					static double ErrorChange_Twist = 0.0;
					static double ErrorPrev_Twist = 0.0;
					static double Kp_Twist = 0.490;
					static double Kd_Twist = 0.250;
					//Position Conrol: Twisting position control to same position
					Error_Twist = REF_POS_Twist - mstrTwistPos;
					ErrorChange_Twist = (Error_Twist - ErrorPrev_Twist) / 0.001;
					TOR[2] = Kp_Twist * Error_Twist + Kd_Twist * ErrorChange_Twist;
					ErrorPrev_Twist = Error_Twist;
				}

				
				preIPAng = currIPAng;
				preIPRes = currIPRes;
			}//If new graphic update is available
		}//ftField is drawn
		
		sndMsg[0] = 'F';
		sndMsg[1] = '&';
		sndMsg[2] = 'T';
		sndMsg[3] = 0;
		if (mstrTwistDirCom == 'P' || mstrTwistDirCom == 'N')
		{
			sndMsg[3] = 1;
		}
		if (mstrTwistDirCom == 'X')
		{
			TOR[2] = 0.0;
			sndMsg[3] = 0;
		}
		if (FOR[2] > 4.0) //Saturating the force value to avoid any accident
			FOR[2] = 4.0;
		else if (FOR[2] < -4.0)
			FOR[2] = -4.0;
		int forceVal = 0;
		forceVal = FOR[2] * 1000;//Repulsive Force with -ve sign
		sndMsg[4] = forceVal >> 16;
		sndMsg[5] = forceVal >> 8;
		sndMsg[6] = forceVal;

		double torCalib = 0.0;
		if (TOR[2] > 0.0)
		{
			TOR[2] = TOR[2] - 0.2 - torCalib; //Requires re-calibration of DAC
											  //			TOR[2] = TOR_Aux - 0.2; //Requires re-calibration of DAC
		}
		else if (TOR[2] < 0.0)
		{
			TOR[2] = TOR[2] + 0.8 + torCalib;
		}
		if (TOR[2] > 4.0) //Saturating the torque value to avoid any accident
			TOR[2] = 4.0;
		else if (TOR[2] < -4.0)
			TOR[2] = -4.0;

		int torVal = 0.0;
		torVal = TOR[2] * 1000.0;
		sndMsg[7] = torVal >> 16;
		sndMsg[8] = torVal >> 8;
		sndMsg[9] = torVal;

		sndMsg[10] = 10;
		sndMsg[11] = 20;
		sndMsg[12] = 30;
		sndMsg[13] = 40;
		sndMsg[14] = 50;
		sndMsg[15] = 60;

		SerialCommunicationTrade::setDataPacket(sndMsg);
		HapticRenderingTrade::setForTor(FOR[2], TOR[2]);

		GetSystemTime(&time);
		timeEnd = (time.wSecond * 1000) + time.wMilliseconds;
		timeElapsed = timeEnd - timeStart;
		
		static int cnt = 0;
		const int NumOfSamples = 50;
		static unsigned long int timeBuf[NumOfSamples] = { 0, };
		timeBuf[cnt] = timeElapsed;
		cnt++;
		if (cnt == NumOfSamples)
		{
			for (int loop = 0; loop < NumOfSamples; loop++)
				timeElapsed += timeBuf[loop];
			timeElapsed /= NumOfSamples;
			cnt = 0;
			HapticRenderingTrade::setHapRendRate(1000.0 / ((double)timeElapsed));
		}
	}//while loop
}//end of function

void HapticRendering::chkIPSgnChange(void)
{
		//To judge that whether it also crossed the maximum value and trend starts decreasing
	//Everynew value is added in next index value
	isSgnChanged = false;
	bool bFirstTrendAcq = false;
	char cFirstTrend = 'X';
	char cTrendOne = 'X';	char cTrendOneb = 'X';
	char cTrendTwo = 'X';	char cTrendTwob = 'X';
	ipPOI = 0.0;
	for(int iLOOP = 0; iLOOP < (ipArrIndex-1);iLOOP++)
	{
		if(ipArr[iLOOP] > ipArr[iLOOP+1])
		{
			//if(InnerProductArray[iLOOP+1] > InnerProductArray[iLOOP+2])// && InnerProductArray[iLOOP] - InnerProductArray[iLOOP+2]
			{
				//if(InnerProductArray[iLOOP+2] > InnerProductArray[iLOOP+3])
				{
					cTrendOne = 'D';
				}
			}
		}
		if(ipArr[iLOOP] < ipArr[iLOOP+1])
		{
			//if(InnerProductArray[iLOOP+1] < InnerProductArray[iLOOP+2])
			{
				//if(InnerProductArray[iLOOP+2] > InnerProductArray[iLOOP+3])
				{
					cTrendTwo = 'I';
				}
			}
		}
		if(!bFirstTrendAcq)
		{
			if(cTrendOne == 'D')
			{
				cFirstTrend = 'D';
				bFirstTrendAcq = true;
			}
			if(cTrendTwo == 'I')
			{
				cFirstTrend = 'I';
				bFirstTrendAcq = true;
			}
			cTrendOneb = cFirstTrend;
		}
		if(cTrendOne != 'X' && cTrendTwo != 'X')
		{
			if(cTrendOneb == 'I')
				cTrendTwob = 'D';
			else if(cTrendOneb == 'D')
				cTrendTwob = 'I';
			cTrendOne = 'X';
			cTrendTwo = 'X';
			isSgnChanged = true;
			break;
		}
	}	
	if(isSgnChanged)
	{
		if(cFirstTrend == 'I') //Find the Maximum Value
		{
			for(int iLOOP = 0; iLOOP < (ipArrIndex-1);iLOOP++)
			{
				if(ipArr[iLOOP] > ipPOI)
					ipPOI = ipArr[iLOOP];
			}
		}
		else if(cFirstTrend == 'D') //Find the minimum Value
		{
			for(int iLOOP = 0; iLOOP < (ipArrIndex-2);iLOOP++)
			{
				if(ipArr[iLOOP] < ipPOI)
					ipPOI = ipArr[iLOOP];
			}
		}
	}
	return;
}