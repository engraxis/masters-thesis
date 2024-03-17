#ifndef __CONTROLS_H_
#define __CONTROLS_H_

#include <windows.h>

#include "kshFPS.h"


// sprintf_s 사용을 위해
#include <stdio.h>

#include <math.h>

// avg filter 사용을 위해.		// Sensor offset 계산.
#include <ksh_Func.h>


#include "Data_Save.h"



#define INDEX	0


// 반올림함수 매크로
#define ROUND(x)	(floor((x) + 0.5))


//////////////////////////////////////////////////////////////////////////
//				extern (외부 선언 - 다른 파일에 선언됨)					//
//////////////////////////////////////////////////////////////////////////
// 외부 선언을 알려주는 문장

////////			WinMain.h				//////////////////////

extern void CALLBACK TimeProc(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);







//////////////////////////////////////////////////////////////////////////
//						MMTimer Thread Setting Parameters					//
//////////////////////////////////////////////////////////////////////////
// Multimedia timer 관련 파라메터!
//FpsTracker* cFps = new FpsTracker(10);

// Control rate
double Control_Rate, Control_Rate_Set = 1000.0;


//////////////////////////////////////////////////////////////////////////

// 저장 데이터 저장 변수 (index 가 data 한개를 차지하므로!)
//double Buf_Data[NUM_OF_DATA-1];






#endif






