#ifndef __CONTROLS_H_
#define __CONTROLS_H_

#include <windows.h>

#include "kshFPS.h"


// sprintf_s ����� ����
#include <stdio.h>

#include <math.h>

// avg filter ����� ����.		// Sensor offset ���.
#include <ksh_Func.h>


#include "Data_Save.h"



#define INDEX	0


// �ݿø��Լ� ��ũ��
#define ROUND(x)	(floor((x) + 0.5))


//////////////////////////////////////////////////////////////////////////
//				extern (�ܺ� ���� - �ٸ� ���Ͽ� �����)					//
//////////////////////////////////////////////////////////////////////////
// �ܺ� ������ �˷��ִ� ����

////////			WinMain.h				//////////////////////

extern void CALLBACK TimeProc(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);







//////////////////////////////////////////////////////////////////////////
//						MMTimer Thread Setting Parameters					//
//////////////////////////////////////////////////////////////////////////
// Multimedia timer ���� �Ķ����!
//FpsTracker* cFps = new FpsTracker(10);

// Control rate
double Control_Rate, Control_Rate_Set = 1000.0;


//////////////////////////////////////////////////////////////////////////

// ���� ������ ���� ���� (index �� data �Ѱ��� �����ϹǷ�!)
//double Buf_Data[NUM_OF_DATA-1];






#endif






