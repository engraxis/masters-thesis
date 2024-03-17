#ifndef __ZMAIN_H_
#define __ZMAIN_H_



// Serial communication alternative
#include "CommThread.h"					// windows.h ���� ���� ����Ǿ�� ��.



#include <windows.h>
#include "resource.h"

#include <gl\gl.h>


#include "kshMenuGL.h"

// sprint_s ���
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
UINT interval;		// ���� : ms
void CALLBACK TimeProc(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);

// MMTimer ����
int Start_MMTimer();

// MMTimer ����
int Stop_MMTimer();
*/






double prev_Save_Time;






//////////////////////////////////////////////////////////////////////////
//////////////			Main Dialog				/////////////////////////

//////////////////			Buttons			///////////////////
// �κ� ���� �÷���
bool bButton_RobotControl;
// �μ� ���� // ���� ���� Control �÷���
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

// ���� �ʱ�ȭ �÷���.// ���� ���� ��� ��ư // ����, �߷� ����� ��ư �÷���, �ڵ� ���� ��ư �÷���
bool bButton_InitFTSensor, bButton_Homing, bButton_FrictionComp, bButton_GravityComp, bButton_DataSave, bButton_FileSave, bButton_MotorOn, bButton_MotorRun;
bool bButton_ImpControl, bButton_GoHome, bButton_RIC;
bool bButton_AdaptiveControl, bButton_RIC_BReset;




//////////////////////////////////////////////////////////////////////////
/*
						�ø��� ��� ����
*/
//////////////////////////////////////////////////////////////////////////
// winapi32 ���� CString Ŭ������ ����ϱ� ����!
//#include <atlstr.h>
#include <afxstr.h>
void PrintStr(CString* pString);



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



// test variables
//unsigned int win_cnt;
//#include <gl\glut.h>

//#include <gl\glu.h>


// ���� �ʱ�ȭ �Լ�
void Initialize_Parameter();
// ���� �Լ�
void Terminate_System();







#endif