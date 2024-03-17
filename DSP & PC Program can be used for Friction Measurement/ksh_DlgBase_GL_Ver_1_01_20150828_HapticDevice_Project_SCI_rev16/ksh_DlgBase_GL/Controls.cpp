

#include "Parameters.h"
//#include "Info_2DOF.h"

//#include "Kinematics_2DOF.h"

#include "kshMenuGL.h"

#include "Controls.h"


extern Timer *cFps;


// 제어 시간
double dControlTime, dControlTime1;

// TEST 변수
// control thread 우선순위 값
DWORD dwContThreadPri;
double temp_cnt1, temp_cnt2, temp_NowCnt1, dControl_CompTime;

//////////////////////////////////////////////////////////////////////////


/*
//////////////////////////////////////////////////////////////////////////
// Control loop
// THREAD_PRIORITY_TIME_CRITICAL 우선순위로 동작함.
// 인수1: Timer ID, returned by timeSetEvent
// 인수2: Reserved
// 인수3 (lpTimeProc): Pointer to the callback function that we want to be called periodically
// 인수4 (dwUser): User data passed to the callback function
// 인수5, 6 (dw1, dw2): Reserved
void CALLBACK TimeProc(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	//////////////////////////////////////////////////////////////////////////
	// Sampling rate 계산 (이전 루프의 sampling rate 측정함)

	// 경과 카운터 계산
	cFps->inc();
	// 주파수 계산
	// 1초간 카운터 값 (QPF) / 변화한 카운터값
	// (cnt / sec) / (cnt) = (1 / sec) - 주파수
	Control_Rate = cFps->getFreq() / cFps->queryInc();


	//TEST
	temp_NowCnt1 = cFps->getCnt();

	


//	cFps->timestamp();
//	Control_Rate = cFps->fpsInstant();
	//////////////////////////////////////////////////////////////////////////
	// coding part //

	// 제어 시간 계산

	dControlTime = dControlTime + 1 / Control_Rate;

	dControlTime1 = dControlTime1 + 1 / Control_Rate;

	// 스레드 우선순위 체크
	// 약 5초마다
	if (dControlTime1 > 5.0)
	{
		dwContThreadPri = GetThreadPriority(GetCurrentThread());

		dControlTime1 = 0.0;		
	}



	

	// Data Save 관련 함수들

	// 데이터 입력 변수에 데이터 저장
	//Buf_Data[0] = dtime;
	

	//////////////////////////////////////////////////////////////////////////
	// SCI Data 수신 test
	Buf_Data[0] = fHD_Pos[0];
	Buf_Data[1] = fHD_Pos[1];
	Buf_Data[2] = fHD_Vel[0];
	Buf_Data[3] = fHD_Vel[1];


	// Test: trash data
	for (int i = 4; i < NUM_OF_DATA-1; i++)
		Buf_Data[i] = 0.0;


	
		
	DataSave(Buf_Data);
//	Data_Save_File();



	dControl_CompTime = cFps->queryCalTime() * 1000.0;
	
}
*/