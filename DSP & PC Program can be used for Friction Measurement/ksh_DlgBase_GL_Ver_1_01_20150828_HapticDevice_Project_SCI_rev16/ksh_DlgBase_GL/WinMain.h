#ifndef __ZMAIN_H_
#define __ZMAIN_H_



// Serial communication alternative
#include "CommThread.h"					// windows.h 보다 먼저 선언되어야 함.



#include <windows.h>
#include "resource.h"

#include <gl\gl.h>


#include "kshMenuGL.h"

// sprint_s 사용
#include <stdio.h>



// Serial communication
#include "SerialPort.h"


#define GRAPHIC_UPDATE_PERIOD   30





//////////////////////////////////////////////////////////////////////////
/////////////////////// Multimedia Timer /////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
//#define TARGET_RESOLUTION	1		// 1 ms target resolution
#define TARGET_RESOLUTION	0		// [ms] set the timer resolution to zero for an absolutely accurate timer?

UINT wTimerRes;
DWORD timerID;
UINT interval;		// 단위 : ms
void CALLBACK TimeProc(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);

// MMTimer 시작
int Start_MMTimer();

// MMTimer 종료
int Stop_MMTimer();
*/






double prev_Save_Time;






//////////////////////////////////////////////////////////////////////////
//////////////			Main Dialog				/////////////////////////

//////////////////			Buttons			///////////////////
// 로봇 제어 플래그
bool bButton_RobotControl;
// 부속 변수 // 제어 루프 Control 플래그
//bool bControlOn;




// Main Dialog Callback Func.
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMainWnd;


// OpenGL Window Handle
HWND		hGLWnd;
HDC			hDCMain = NULL;			// GDI Device Context
HGLRC		hRCMain = NULL;			// Rendering Context





//////////////////////////////////////////////////////////////////////////
// OpenGL Test
//////////////////////////////////////////////////////////////////////////

// 센서 초기화 플래그.// 영점 세팅 모드 버튼 // 마찰, 중력 보상기 버튼 플래그, 자동 제어 버튼 플래그
bool bButton_InitFTSensor, bButton_Homing, bButton_FrictionComp, bButton_GravityComp, bButton_DataSave, bButton_FileSave, bButton_MotorOn, bButton_MotorRun;
bool bButton_ImpControl, bButton_GoHome, bButton_RIC;
bool bButton_AdaptiveControl, bButton_RIC_BReset;




//////////////////////////////////////////////////////////////////////////
/*
						시리얼 통신 관련
*/
//////////////////////////////////////////////////////////////////////////
// winapi32 에서 CString 클래스를 사용하기 위해!
//#include <atlstr.h>
#include <afxstr.h>
void PrintStr(CString* pString);



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



// test variables
//unsigned int win_cnt;
//#include <gl\glut.h>

//#include <gl\glu.h>


// 변수 초기화 함수
void Initialize_Parameter();
// 종료 함수
void Terminate_System();







#endif