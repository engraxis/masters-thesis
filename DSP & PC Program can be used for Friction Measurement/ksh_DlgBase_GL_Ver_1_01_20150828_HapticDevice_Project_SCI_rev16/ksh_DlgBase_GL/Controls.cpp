

#include "Parameters.h"
//#include "Info_2DOF.h"

//#include "Kinematics_2DOF.h"

#include "kshMenuGL.h"

#include "Controls.h"


extern Timer *cFps;


// ���� �ð�
double dControlTime, dControlTime1;

// TEST ����
// control thread �켱���� ��
DWORD dwContThreadPri;
double temp_cnt1, temp_cnt2, temp_NowCnt1, dControl_CompTime;

//////////////////////////////////////////////////////////////////////////


/*
//////////////////////////////////////////////////////////////////////////
// Control loop
// THREAD_PRIORITY_TIME_CRITICAL �켱������ ������.
// �μ�1: Timer ID, returned by timeSetEvent
// �μ�2: Reserved
// �μ�3 (lpTimeProc): Pointer to the callback function that we want to be called periodically
// �μ�4 (dwUser): User data passed to the callback function
// �μ�5, 6 (dw1, dw2): Reserved
void CALLBACK TimeProc(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	//////////////////////////////////////////////////////////////////////////
	// Sampling rate ��� (���� ������ sampling rate ������)

	// ��� ī���� ���
	cFps->inc();
	// ���ļ� ���
	// 1�ʰ� ī���� �� (QPF) / ��ȭ�� ī���Ͱ�
	// (cnt / sec) / (cnt) = (1 / sec) - ���ļ�
	Control_Rate = cFps->getFreq() / cFps->queryInc();


	//TEST
	temp_NowCnt1 = cFps->getCnt();

	


//	cFps->timestamp();
//	Control_Rate = cFps->fpsInstant();
	//////////////////////////////////////////////////////////////////////////
	// coding part //

	// ���� �ð� ���

	dControlTime = dControlTime + 1 / Control_Rate;

	dControlTime1 = dControlTime1 + 1 / Control_Rate;

	// ������ �켱���� üũ
	// �� 5�ʸ���
	if (dControlTime1 > 5.0)
	{
		dwContThreadPri = GetThreadPriority(GetCurrentThread());

		dControlTime1 = 0.0;		
	}



	

	// Data Save ���� �Լ���

	// ������ �Է� ������ ������ ����
	//Buf_Data[0] = dtime;
	

	//////////////////////////////////////////////////////////////////////////
	// SCI Data ���� test
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