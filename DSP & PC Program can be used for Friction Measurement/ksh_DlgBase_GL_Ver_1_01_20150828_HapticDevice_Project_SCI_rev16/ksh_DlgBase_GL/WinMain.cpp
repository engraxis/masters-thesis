///////////////////////////////////////
//      ���α׷� �޴���              //
////////////////////////////////////////

// �������
// Process �� �켱������ �ְ� (Real time)�� ����������, High �ۿ� �ȵ�.
// MMTimer ����� ������� �ְ� �켱������ ����. (�������� Normal ����)



//////////////////////////////////////////////////////////////////////////
// ���α׷� ���� ����

// 2015.03.13
// ������ ������� ���� ����! => �޸𸮿� ����� data�� ���Ϸ� �����, cpu �� block �Ǵ� ���� �ذ�!!!

// 2015.03.14
// Control Thread (MMTimer ���) �� �ֱ�� ���ػ� Ÿ�̸ӷ� ������

// 2015.06.22
/*
// �ø��� ��� ��� ���� ��!!!

// ���� �߰�: CommThread, stdafx, targetver, SerialPort
// ������Ʈ �Ӽ� ����:
	MFC ���� ���� �ذ�: ������Ʈ �Ӽ�->General, MFC ��� -> ���� DLL���� MFC ���



*/

//////////////////  �����ڵ� /////////////////////////////////
// �̽ļ��� ���ؼ� C��� Ÿ���� �ƴ�, �߰� Ÿ���� ���
/*

C Ÿ��				�����ڵ� Ÿ��
char				TCHAR
char*				LPSTR
const char*			LPCTSTR


C ǥ���Լ�				�����ڵ� ���� �Լ� (�տ� l �� ���̱�)
strlen				lstrlen
strcpy				lstrcpy
strcat				lstrcat
strcmp				lstrcmp
sprintf				wsprintf

// ���ڿ� ����� Ÿ���� �����Ƿ�, TEXT ��ũ�θ� �����
TCHAR * str = "string";				// No!!!
TCHAR* str = TEXT("string");		// TEXT ��ũ�� �ȿ� ���ڿ� ����� ����.


ex) #ifdef UNICODE
typedef wchar_t	TCHAR;
#else
typedef char	TCHAR;
#endif


// ũ�Ⱑ ������ Ÿ��
				��ȣ ����					��ȣ����
32��Ʈ			INT32, LONG32				UINT32, ULONG32, DWORD32
64��Ʈ			INT64, LONG64				UINT64, ULONG64, DWORD64

// �����ϵǴ� ȯ�濡 ���� ���̰� �޶����� �� ���
INT_PTR, LONG_PTR, UINT_PTR, ULONG_PTR, DWORD_PTR

�ڿ� _PTR �� ���� Ÿ���� �ü���� ��Ʈ���� ���� ũ�Ⱑ �������̸� �����Ϳ� ���� ���̸� ����.
�����Ϳ� ȣȯ�Ǿ�� �ϴ� ������ �ʿ��� ���� �� Ÿ���� ����ؾ� ��.

*/

//////////////// Serial Communication /////////////////////////
/*

���� ����: http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=50&MAEULNo=20&no=649883&ref=649883


// ������: http://magicom9.tistory.com/54
http://msdn.microsoft.com/en-us/library/ff802693.aspx
http://blog.daum.net/pg365/
http://blog.daum.net/_blog/BlogTypeView.do?blogid=0MMaA&articleno=8039404&categoryId=0&regdt=20121225062827


*/







//////////////////////////////////////////////////////////////////////////
// �������...

////////////////// extern Ű���� ///////////////////////////////////
// extern => �ܺο� ���� �� ���ǵǾ��ٰ� �����Ϸ����� �˷��ִ� �뵵.
// �ش� ������ � �ڷ�������, ��� (�ܺ�) ����Ǿ� �ִ��� �����Ϸ����� �˷��ִ� �޽���.

// �Լ��� ���, extern ������ ������ �� ����.


//////////////// static Ű���� ////////////////////////////////
// ���� ������ static ����
// => �� ������ �ܺ� ���Ͽ����� ������ ������� �ʴ´�.
// => (In other words) �� ������ ���ٹ����� ���� ���η� �����Ѵ�.

// �ڵ��� �������� ����.



////////////////// #include ������ /////////////////////////////
// ������ ������ �ܼ��� ���Խ�Ű�� �뵵.


////////////////// �ν��Ͻ� (Instance)	//////////////////////////////////
// �������� ���α׷� �ϳ��� Ī�ϴ� ���.
// hInstance �� ���α׷� ��ü�� ���´� �������̸�, ���α׷� ���ο����� �ڱ� �ڽ��� ����Ű�� 1��Ī ������.


///////////// SendMessage, PostMessage ���� ////////////////////
/*
SendMessage: ���̷�Ʈ�� �ٷ� ����
   -> ����ϰ� ó���ϱ� ������ ���� ��Ȳ�� �����.
   -> �������� Return �� �� ������ ��� ��ٸ�.

PostMessage: �޽��� ť�� �־��ٰ� ������ ���缭 ���� (���п� ����)
   -> ���������� ó���� �Ǳ� ������ ���� ���� ��Ȳ���� ū ������ ����.
   -> ���������� Return �� ��� ��� �۾��� ������.


*/



#include "Parameters.h"
#include "WinMain.h"
#include "Dlg_Setting.h"


#include "kshOpenGL.h"


#include "Data_Save.h"

#include <math.h>


#include "kshFPS.h"








//////////////		extern		/////////////////////////////////////
extern bool bPathMode, bRPathMode;

extern double ks[2];
extern double lpf_alpha, lpf_alpha2;

extern double End_Pos[2], End_JPos[2], Tf;


///////////////////////// Data ���� ����
extern bool IsDataSaving;
extern unsigned int NumOfData, IndexData, Save_Data_Count;
extern double Save_Time, dtime, *Mem_Buffer;
extern bool IsDataSaveFinish;

extern int wFile_Cnt;



//////////////////////////////////////////////////////////////////////////
//				extern (�ܺ� ���� - �ٸ� ���Ͽ� �����)					//
//////////////////////////////////////////////////////////////////////////
// �ܺ� ������ �˷��ִ� ����


////////			Graphics.h				//////////////////////

//extern BOOL CreateGLDialog(HWND* hDlg, HDC* hDC, HGLRC* hRC);
//extern GLvoid KillGLDialog(HDC* hDC, HGLRC* hRC);

extern bool InitGLMainWindow(HDC hDC, HGLRC hRC);
extern bool DrawMainWindow(HWND hWnd, HDC hDC, HGLRC hRC);


// OPENGL Window size ����
extern int WSIZEX;  
extern int WSIZEY;



extern bool bButton_InitFTSensor;

// FT Sensor Init ��ư.  ���� ����  // DATA Save
extern lwtButton BInitFTSensor, BHoming, BDATASAVEButton, BSAVEFILEButton, BFrictionComp, BGravityComp, BMotorOn, BMotorRun;
extern lwtButton BRIC;
extern lwtButton BAdaptiveControl, RIC_BReset;

extern lwtSlider sSaveTime, sDATASlider;


/////////////////////////////////////////////////////////////

////////////////		Controls.h				//////////////////
extern double Control_Rate_Set;









////////////// Test
// ����� Ÿ�̸� - �ð� ����
//Timer cFps;

// control loop rate (���ػ� Ÿ�̸�)
Timer *cFps = new Timer;

// graphic rendering rate (�̰� ���ػ� Ÿ�̸Ӱ� �ƴ�)
FpsTracker* gFps = new FpsTracker(10);




//////////////////////////////////////////////////////////////////////////
///////    File Save ���� Thread ����
DWORD ThreadID;
HANDLE hFileSaveThread;





//////////////////////////////////////////////////////////////////////////
					// ComPort Setting Dialog
//////////////////////////////////////////////////////////////////////////
// Callback function for ComPort Setting Dialog
LRESULT CALLBACK CB_ComPort_Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ComPort List: Com1, Com2, Com3, etc
// 2���� �迭, ù��°�� �࿡ �ش�, 2��°�� ���� ���� (5 �� 5���� ���� �ǹ�)
char combo_ComPort_List[][6] = {"COM1", "COM2", "COM3", "COM4", "COM5"};

// BaudRate List: 9600, 19200, 115200
char combo_BaudRate_List[][7] = {"9600", "38400", "19200", "115200", "230400", "460800", "921600"};




// ComPort, BaudRate �� �ش��ϴ� ����
char str_ComPort[6], str_BaudRate[7];


// ���� �ּ� ���ڿ�
char cComPath[13];

CString strComPath;


// SerialPort class ����
CSerialPort ComNum;


// CommThread SerialPort class ����: �ø��� ��� ��ü
CCommThread Comm_SCI;

// Receive function
//LONG Comm_Receive(UINT port, LONG lParam);

CString strReceive, strWrite;

// received data
char* cCommRec_Data;


// Send data ���ڿ� ����
char str_Send_Data[1024];
//CString str_Send_Data;


// Com connect
bool bComPort_On = false;

// ��� Connect Status
char str_Com_Status[size_buf] = {"Ready"};


// List Display ����
char str_ListDISP_Data[1024];


/////////////////////// DSP data ���� ����
// DSP�κ��� ������ ������ ���� ����
BYTE rBuf[TX_BUF];
// ������ BYTE ���� ī����
BYTE rCounter;

// DSP TX FIFO ũ�� (�ѹ��� ������ �۽ŷ�)
//#define TX_BUF	16

// unsigned 32 bit data ��: DSP float ���� 32 bit��. float ��ȯ�� ���� �ӽ� ����
DWORD RxBuf_Temp[TX_BUF];


///////////////
///////////// Haptic Device ���� ����
////////////
// ��ġ, �ӵ� ����
float fHD_Pos[2], fHD_Vel[2], fHD_Accel[2], fHD_Test;
float fHD_FT[2];

// DSP Tx Index
float dDSPTxIndex;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





HWND hList_DISPLAY;







/// TEST Variable
double Main_Test1;
int MouseX, MouseY;
bool IsLeftMouseOn;


DWORD dwMainThreadPri;


double dQPF;

// COMPORT Test variables
unsigned int win_cnt, win_test, win_test1;

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadFunc(LPVOID temp)
{
	//sDATASlider.Enable = true;


	Data_Save_File();
	
	return 0;
}







int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst = hInstance;



	// ���μ��� �켱����: �ֻ��� �켱���� ����: REALTIME_PRIORITY_CLASS
	// Z227 ��Ͽ����� HIGH_PRIORITY_CLASS (0x80) �ۿ� �ȵ�.

	if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
	{
		// Error

		return 0;

	}

	// ���α׷� ���� �ð� ����.
	//cFps.start();
	cFps->start();
	

	// TEST
	dQPF = cFps->getFreq();



	// Open Main Dialog
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_MAIN), HWND_DESKTOP, MainDlgProc);

	return 0;
}

// WinAPI 2��: ��ȭ���� �޽���. ��ȭ���� ���ν��� (pp. 575)
// �ü�� ���� #32770 ������ ���ν����� ������.

// ���ϰ��� �ý����� ������ ������ ���ν����� ������ �� �����κ��� ����Ʈ ó�� ���θ� �����Ѵ�.
// ���� ��ȭ���� ���ν����� TRUE�� �����ߴٸ� �޽����� �� ó���� ���̹Ƿ� �� �̻��� ó���� �� �ʿ䰡 ������,
// FALSE�� �����ߴٸ� �ڽ��� ���� ó���ϰų� �ƴϸ� DefWindowProc���� ó���� �ѱ� ���̴�.
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	//////////////////////////////////////////////////////////////////////////
	// TEST variables

	// �� ��Ʈ�ѿ� ���� ������ �ڵ� ����
	static HWND hButton_ComPort_On;
	static HWND hEdit_Send_Data;
//	static HWND hList_DISPLAY;
	

	//////////////////////////////////////////////////////////////////////////
	

	switch (iMessage) {
	case WM_INITDIALOG:
		// ���̾�α� �ʱ�ȭ �κ�
		hDlgMainWnd = hDlg;


		// TEST
		dwMainThreadPri = GetThreadPriority(GetCurrentThread());


		

		// First, let's get the handle to all the window we created in the Dialog
		// with the resource editor
		//hGLWnd = GetDlgItem(hDlg, IDC_GL_WND);
		hGLWnd = GetDlgItem(hDlg, IDC_GL_WND);

						

		// OpenGL �κ�: Picture Control �� �ڵ��� �ѱ�.  Picture control �� GL Rendering ������.
		//if(CreateGLDialog(&hGLWnd, &hDCMain, &hRCMain) == FALSE)
		if(CreateGLWindow(&hGLWnd, &hDCMain, &hRCMain) == FALSE)
		{
			MessageBox(NULL,"Error initializing OpenGL Window","OpneGL",MB_OK);
			return FALSE;
		}
	

		//////////////////////////////////////////////////////////////////////////
		// variables initialize (���� ���� �ʱ�ȭ�� Initialize_Parameter() �Լ� �ȿ� ������ �˴ϴ�.)

		Initialize_Parameter();

		// Handle ���
		//Comm_SCI.hCommWnd = hGLWnd;
		Comm_SCI.hCommWnd = hDlgMainWnd;

		//////////////////////////////////////////////////////////////////////////


		// Open GL Window �ʱ�ȭ.
		InitGLMainWindow(hDCMain, hRCMain);


		// Windows Timer: Graphic rendering timer
		SetTimer(hDlgMainWnd, 1, GRAPHIC_UPDATE_PERIOD, NULL);



		


		return TRUE;


	case WM_CLOSE:

		// ���� ���� �Լ���
		Terminate_System();

		return TRUE;



	case WM_COMM_READ:
		// Comm receive message handler

		TEST_var3++;

		//Comm_Receive(NULL, lParam);

		// �ٽ� ���� �޼����� ���� �� �ֵ��� ������.
		//Comm_SCI.m_bReserveMsg = false;

		return 0;
		


	case WM_COMMAND:
		// �ڵ� �̸� ����
		HWND hBControl, hBImpControl, hBGoHome;

	//	double Ts;


		switch (LOWORD(wParam))
		{
			// LOWORD(wParam) �� �޽����� ���� ��Ʈ���� ID�� ���޵�
			// HIWORD(wParam)�� ���� �ڵ尡 ���޵�.

			// �޴�: File -> Exit 
		case ID_FILE_EXIT:
		case IDCANCEL:
			
			// ���� ���� �Լ���
			Terminate_System();
			
			return TRUE;

		// Setting �޴� Ŭ��
		case ID_MENU_SETTING:
						

			bButton_RobotControl = false;
			
//			DialogBox(g_hInst, (LPCTSTR)IDD_SETTING, hDlgMainWnd, (DLGPROC)Setting);

			break;

		// COMPORT Setting �޴� Ŭ��
		case ID_MENU_SETTING_COMPORT:

			// Open Comport setting dialog box
			DialogBox(g_hInst, (LPCTSTR)IDD_COMPORT_SETTING, hDlgMainWnd, (DLGPROC)CB_ComPort_Setting);

					
			

			break;

			//////////////////////////////////////////////////////////////////////////
			//    COMPORT  ���� COMMAND
			//////////////////////////////////////////////////////////////////////////

			// COM Connect or Disconnect
		case IDC_BUTTON_COMPORT_CONNECT:
	
			if (!Comm_SCI.m_bConnected)		// Try to Connect!
			{
				
				sprintf_s(str_TEST2, size_buf, "On");


				//CString strPortNum;

				// ���� �ּ� ���ڿ�
				//char cComPath[13];

				sprintf_s(cComPath, 13, "\\\\.\\%s", str_ComPort);


				// char * --> CString ��ȯ
				strComPath.Format("%s", cComPath);

				// TEST Monitoring
				//strcpy(str_TEST, strComPath);
				strcpy(str_TEST1, str_BaudRate);
				


				// ��� �ӵ� (BaudRate �˻�)
//				if (!strcmp(str_BaudRate, "115200"))		// ���ڿ��� ��ġ�ϸ� 0 �� ��ȯ�ϹǷ�!!!
//				{
					// BaudRate: 57600 bps
					//if (ComNum.Open(cComPath, CBR_115200, 8, ONESTOPBIT, NOPARITY))
					if (Comm_SCI.OpenPort(strComPath, str_BaudRate, "8", "NO", "NO", "1"))
					{
						// ���� ����

						sprintf_s(str_Com_Status, size_buf, "Success!");

						win_test = 1;

						SetDlgItemText(hDlgMainWnd, IDC_BUTTON_COMPORT_CONNECT, "Disconnect");


						//////////////////////////////////////////////////////////////////////////
						////////////////	List Box�� ���ڿ� �߰� (��°�� ����ϱ�)
						//////////////////////////////////////////////////////////////////////////
						CString strTemp;
				
						strTemp = strComPath + _T("(") + str_BaudRate + _T(")");

						strTemp += "�� ���� �Ǿ���.";
				
						// CString to char ��ȯ
						strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
						// ListBox - DISPLAY �ڵ� ���
						hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

						SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);
						
						// �׸� �� üũ
						int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

						if (getLboxitems > NUM_LIST_BOX)
							// return: ���� �׸��
							getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

						SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);
						//////////////////////////////////////////////////////////////////////////



					}
					else
					{
						// ���� ����
						sprintf_s(str_Com_Status, size_buf, "Failed!");

						win_test = 10;


						SetDlgItemText(hDlgMainWnd, IDC_BUTTON_COMPORT_CONNECT, "Connect");

						//////////////////////////////////////////////////////////////////////////
						////////////////	List Box�� ���ڿ� �߰�
						//////////////////////////////////////////////////////////////////////////
						CString strTemp;
				
						strTemp = strComPath + _T("(") + str_BaudRate + _T(")");

						strTemp += "�� ���� �Ұ���!";
				
						// CString to char ��ȯ
						strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
						// ListBox - DISPLAY �ڵ� ���
						hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

						SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);
						
						// �׸� �� üũ
						int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

						if (getLboxitems > NUM_LIST_BOX)
							// return: ���� �׸��
							getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

						SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);
						//////////////////////////////////////////////////////////////////////////

					}
//				}
//				else
//				{
//					win_test = 100;
//				}
				
			}
			else		// ���� ����
			{

				//str_TEST2 = "";
				sprintf_s(str_TEST2, size_buf, "Off");


				SetDlgItemText(hDlgMainWnd, IDC_BUTTON_COMPORT_CONNECT, "Connect");

				Comm_SCI.ClosePort();

				//////////////////////////////////////////////////////////////////////////
				////////////////	List Box�� ���ڿ� �߰�
				//////////////////////////////////////////////////////////////////////////
				CString strTemp;
				
				strTemp = "���� ����Ǿ����ϴ�.";
													
				// CString to char ��ȯ
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
				// ListBox - DISPLAY �ڵ� ���
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);
						
				// �׸� �� üũ
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: ���� �׸��
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);
				//////////////////////////////////////////////////////////////////////////

			}



			// Open Selected Com Port


			break;

			// End
			//////////////////////////////////////////////////////////////////////////








/////////////////// EDIT �ڽ� (SCI ��� ��)
			// Write a message for Serial communication
		case IDC_EDIT_COMPORT_WRITE:

			// �ڵ� ���
			hEdit_Send_Data = GetDlgItem(hDlgMainWnd, IDC_EDIT_COMPORT_WRITE);

			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				GetWindowText(hEdit_Send_Data, str_Send_Data, 1024);
				

			}

			break;



			// Send data to MCU
		case IDC_BUTTON_COMPORT_WRITE:

			// ����Ǿ��� ����!

			// TEST
			bComPort_On = true;


			if (bComPort_On)
			{
				// Send data which is obtained from Edit control (str_Send_Data)
				int n;

				n = strlen(str_Send_Data);


				strWrite.Format("%s", str_Send_Data);

				// ���� �۵���.
			//	Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength()+1);

				// �̷��� �ϸ� �ι��ڰ� ������ ������?
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

								


			//	PrintStr(&CString(">> " + strWrite.GetString()));

				CString strTemp;
				
				strTemp = ">> ";

				strTemp += str_Send_Data;
				
				// CString to char ��ȯ
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
				// ListBox - DISPLAY �ڵ� ���
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// �׸� �� üũ
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: ���� �׸��
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);
				
				

			}





			break;
/*
		case IDC_LIST_COMPORT_DISPLAY:
			switch (HIWORD(wParam)) {
			//	case LBN_SELCHANGE
			}


			break;
*/


			
						
			

			// Buttons
		case IDC_BUTTON_CONTROL:

//			
			--wFile_Cnt;

			if (wFile_Cnt < 0)
				wFile_Cnt = 0;


						

			return TRUE;


			// ���Ǵ��� ��ư On/Off
		case IDC_BUTTON_IMP_CONTROL:

			++wFile_Cnt;


			return TRUE;



		// ���� ��ư
		case IDC_BUTTON_RESET:


			// Reset Everything!

		
			// Data Save Reset
			IsDataSaving = FALSE;
			sDATASlider.Enable = FALSE;

			NumOfData = 0;
			IndexData = 0;
			Save_Data_Count = 0;	// Count the number of save data


			// ���� ������ ���� �ѹ����� ���� ����.
			wFile_Cnt = 0;


			// �ڵ����� On/Off �÷���
			bAutoSave = false;


			return TRUE;

			// E-Stop
		case IDC_BUTTON_ESTOP:
			// Stop everything! E-Stop
						
			
			
			bButton_MotorOn = false;
			BMotorOn.update(false);

			bButton_MotorRun = false;
			BMotorRun.update(false);

										

			bButton_FrictionComp = false;
			BFrictionComp.update(false);

			bButton_GravityComp = false;
			BGravityComp.update(false);

			bButton_RIC = false;
			BRIC.update(false);

			bButton_RIC_BReset = false;
			RIC_BReset.update(false);


			bButton_AdaptiveControl = false;
			BAdaptiveControl.update(false);

					


			bButton_RobotControl = false;
			// �ڵ� ���
			hBControl = GetDlgItem(hDlgMainWnd, IDC_BUTTON_CONTROL);

			//////// ��ư ĸ�� ����		///////////////////
			// ĸ�� ����
			SetWindowText(hBControl, TEXT("Control On"));



			bButton_ImpControl = false;
			// �ڵ� ���
//			HWND hBImpControl;
			hBImpControl = GetDlgItem(hDlgMainWnd, IDC_BUTTON_IMP_CONTROL);
			
			SetWindowText(hBImpControl, TEXT("Imp. Control On"));



			return TRUE;

		case IDC_BUTTON_READY:
			// bSafety ��ư ����ȭ


	

			return TRUE;


		case IDC_BUTTON_GOHOME:
			// ���� �����Ŀ� Back Home �ϱ� ���� �÷���

	//		bButton_GoHome = !bButton_GoHome;

			// �ڵ� ���
			
			hBGoHome = GetDlgItem(hDlgMainWnd, IDC_BUTTON_GOHOME);

			//////////////////////////////////////////////////////////////////////////
			// ���� On/Off üũ
			if (!bButton_GoHome)
			{
				// Enalbe GoHome

				//////// ��ư ĸ�� ����		///////////////////
				// ĸ�� ����
				SetWindowText(hBGoHome, TEXT("Disable GoHome"));



				// Enalbe GoHome
				//////////////////////////////////////////////////////////////////////////
				// ���� ���� ����.
//				sprintf_s(buffer_Err, size_buf, "[Err. Msg]: OK: GoHome");
		//		bSafety = true;



				// true
				bButton_GoHome = !bButton_GoHome;
				

			}
			else
			{
				// ĸ�� ����
				SetWindowText(hBGoHome, TEXT("Enable GoHome"));


				// Reset Control Torque
	//			for (int i = 0; i < 2; i++)
	//				Control_Input[i] = 0.0;


				// false;
				bButton_GoHome = !bButton_GoHome;

			}
			


			return TRUE;


		}
		// default ���ν����� ������.
		return FALSE;






	case WM_LBUTTONDOWN:
		MouseX=LOWORD(lParam);
		MouseY=HIWORD(lParam);
		IsLeftMouseOn = true;
		//////////////////////////////////////////////////////////////////////////
		////////////////////   OpenGL GUI ��� ���� ���� //////////////////////
		//////////////////////////////////////////////////////////////////////////
		
		// ���� ���� On
		BHoming.read(&bButton_Homing, lParam, true);

		// Motor Enable On/OFF
		BMotorOn.read(&bButton_MotorOn, lParam, true);


		// Ŭ�� ��� ���� üũ
		if (BMotorOn.bClicked)
		{
			// Enable ����� DSP�� �۽�

			// ����Ǿ��� ����!
			if (Comm_SCI.m_bConnected)
			{
				CString str_TBuf;

				if (bButton_MotorOn)
				{
					// 16�� data �۽�
					// ENT �۽�, ������ 13���� zzzzzzzzz
					str_TBuf = "CMD:ENTzzzzzzzzz";

				}
				else
				{
					// 16�� data �۽�
					// ENF �۽�, ������ 13���� zzzzzzzzz
					str_TBuf = "CMD:ENFzzzzzzzzz";


					// Run/Stop ��ư ����
					bButton_MotorRun = false;
					BMotorRun.update(bButton_MotorRun);

				}

				// �۽��� ���� ������ ����
				strWrite.Format("%s", str_TBuf);

				// �̷��� �ϸ� �ι��ڰ� ������ ������? (GetLength()���� +1 ����)
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

				CString strTemp;

				strTemp = ">> ";

				strTemp += str_TBuf;

				// CString to char ��ȯ
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));

				// ListBox - DISPLAY �ڵ� ���
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				// LISTBOX�� �� �߰� �޽��� �߼�
				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// �׸� �� üũ
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: ���� �׸��
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				// LISTBOX�� ���� �� ���� �޽��� �߼�
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);

			}



			// false�� ����
			BMotorOn.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}



		// Motor Run/Stop
		BMotorRun.read(&bButton_MotorRun, lParam, true);


		// Ŭ�� ��� ���� üũ
		if (BMotorRun.bClicked)
		{
			// Run/Stop ����� DSP�� �۽�

			// ����Ǿ��� ����!
			if (Comm_SCI.m_bConnected)
			{
				CString str_TBuf;

				if (bButton_MotorRun)
				{
					// 16�� data �۽�
					// ENT �۽�, ������ 13���� zzzzzzzzz
					str_TBuf = "CMD:ENRzzzzzzzzz";

				}
				else
				{
					// 16�� data �۽�
					// ENF �۽�, ������ 13���� zzzzzzzzz
					str_TBuf = "CMD:ENSzzzzzzzzz";

				}

				// �۽��� ���� ������ ����
				strWrite.Format("%s", str_TBuf);

				// �̷��� �ϸ� �ι��ڰ� ������ ������? (GetLength()���� +1 ����)
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

				CString strTemp;

				strTemp = ">> ";

				strTemp += str_TBuf;

				// CString to char ��ȯ
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));

				// ListBox - DISPLAY �ڵ� ���
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				// LISTBOX�� �� �߰� �޽��� �߼�
				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// �׸� �� üũ
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: ���� �׸��
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				// LISTBOX�� ���� �� ���� �޽��� �߼�
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);

			}



			// false�� ����
			BMotorRun.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}
		


		
		// Friction Compensation On/Off
		BFrictionComp.read(&bButton_FrictionComp, lParam, true);



		// Ŭ�� ��� ���� üũ
		if (BFrictionComp.bClicked)
		{
			// Friction Comp. On/Off ����� DSP�� �۽�

			// ����Ǿ��� ����!
			if (Comm_SCI.m_bConnected)
			{
				CString str_TBuf;

				if (bButton_FrictionComp)
				{
					// 16�� data �۽�
					// FRT �۽�, ������ 13���� zzzzzzzzz
					str_TBuf = "CMD:FRTzzzzzzzzz";

				}
				else
				{
					// 16�� data �۽�
					// FRF �۽�, ������ 13���� zzzzzzzzz
					str_TBuf = "CMD:FRFzzzzzzzzz";

				}

				// �۽��� ���� ������ ����
				strWrite.Format("%s", str_TBuf);

				// �̷��� �ϸ� �ι��ڰ� ������ ������? (GetLength()���� +1 ����)
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

				CString strTemp;

				strTemp = ">> ";

				strTemp += str_TBuf;

				// CString to char ��ȯ
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));

				// ListBox - DISPLAY �ڵ� ���
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				// LISTBOX�� �� �߰� �޽��� �߼�
				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// �׸� �� üũ
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: ���� �׸��
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				// LISTBOX�� ���� �� ���� �޽��� �߼�
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);

			}



			// false�� ����
			BFrictionComp.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}

		




		
		// Gravity Compensation On/Off
		BGravityComp.read(&bButton_GravityComp, lParam, true);

		// Impedance Control On/Off
		BRIC.read(&bButton_RIC, lParam, true);

		
		BAdaptiveControl.read(&bButton_AdaptiveControl, lParam, true);

		RIC_BReset.read(&bButton_RIC_BReset, lParam, true);


		// FT Reset ��ư
		BInitFTSensor.read(&bButton_InitFTSensor, lParam, true);


	
		// Data Save �ð� ���� �����̴���
		sSaveTime.read(&Save_Time, lParam, true);

		// Data Save On/Off ���
		BDATASAVEButton.read(&bButton_DataSave, lParam, true);


		// Ŭ�� ��� ���� üũ
		if (BDATASAVEButton.bClicked)
		{
			// �ڵ����� On/Off �÷���
			bAutoSave = true;
					

			// false�� ����
			BDATASAVEButton.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}



		

		
		// Make Save File On/Off ���
		BSAVEFILEButton.read(&bButton_FileSave, lParam, true);


		// Create file to save data in the memory
		// �޸𸮿� ����� data�� disk �� �����ϱ� ���� thread ȣ��
		// �˻�
		if (IsDataSaveFinish && !bButton_FileSave)
		{
			// ����� data�� ���Ϸ� �����ϱ� ���� ������ ����
			// HANDLE CreateThread
			// �μ�1: ���� �Ӽ� ����: �ڽ� ���μ����� �ڵ��� ������� �ʴ� �� NULL �� ����
			// �μ�2: �������� ���� ũ��. 0���� �����ϸ� �� ������ (CreateThread�� ȣ���� ������)�� ���� ũ�⸦ ����
			//      �⺻������ �� �������� ������ 1 M�� ����Ǹ�, �� �� �� ������(���� 4K)�� Ȯ���Ǿ� ����.
			//      ���� ��� ȣ���� ���̰� ���� ��ٸ� ���� ũ�⸦ �� �ø� ���� ����.
			// �μ�3: �������� ���� �Լ� ���� (Entry point). �� �������� WinMain �Լ��� �ش��Ѵٰ� ���� ��.
			//		�� �Լ��κ��� �������� ������ �����ϸ�, ���� �Լ��� ����Ǹ� ������� �����.
			//		�����Լ��� ������ ���� ������ ������ ��.
			//		DWORD WINAPI ThreadFunc(LPVOID lpParameter);
			// �μ�4: ������ �����Լ��� �μ� (lpParameter). �μ��� ���� ��� NULL ����.
			// �μ�5: �������� Ư�� ����. 0�̸�, �ƹ� Ư�� ���� ���� ��������.
			//		CREATE_SUSPENDED �÷���: �����带 ����⸸ �ϰ�, ������ ���� ����.
			//		������ �����带 ������ ���� ResumeThread �Լ��� ȣ���ϸ� ��.
			// �μ�6: �������� ID�� �����ϱ� ���� ��¿� �μ�.
			//		������ ID�� �ʿ��� ���� ���� ���µ�, �� ���� NULL ����. (���: 9x�迭�� �� �μ��� NULL�̸�, �����尡 �������� ����.)
			//		ȣȯ���� ���ؼ��� DWORD���� ������ ������ �� ������ �Ѱܾ� ��.

			// ���: ������ �߻����� ���, NULL �� ������.
			hFileSaveThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);

			// �������� �ڵ�� ������ ��ü�� �ٸ��Ƿ�, �ڵ��� �ݴ´ٰ� �ؼ� �����尡 ����Ǵ� ���� �ƴ�.
			CloseHandle(hFileSaveThread);
		}




		// �� �ݹ鿡�� �Ϸ�
		return TRUE;

	case WM_LBUTTONUP:
		IsLeftMouseOn = false;


		if (prev_Save_Time != Save_Time)
		{
			// ���� �ð��� �����!
			sDATASlider.max = Save_Time;

			// Save Data ���� �޸� �Ҵ�
			// Make save data space
			if (Mem_Buffer != NULL)
			{
				// �޸� ����
				free(Mem_Buffer);

				// �޸� ���� �Ҵ�
				Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));
			}
			else
				// �޸� ���� �Ҵ�
				Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));


			// ����
			prev_Save_Time = Save_Time;

		}



		return TRUE;

	case WM_MOUSEMOVE:

		// Save Time ���� �����̴��� �� ����
		sSaveTime.read(&Save_Time, lParam, IsLeftMouseOn);

		

//		if (IsLeftMouseOn)
//		{
			MouseX=LOWORD(lParam);
			MouseY=HIWORD(lParam);

//		}
		



		return TRUE;




	case WM_TIMER:		
		if (wParam==1) 
		{
			// �� 30 Hz ȭ�� ������
			DrawMainWindow(hGLWnd, hDCMain, hRCMain);

		}

		return TRUE;


	}
	return FALSE;
}


/*

int Start_MMTimer()
{

	///////////////////////////////////////////////////////////
	// MMTimer ����
	TIMECAPS tc;

	// Ÿ�̸��� �ּ�/�ִ� �ػ󵵸� ���ɴϴ�.
	// �μ�1: Pointer to a TIMECAPS structure. It is filled with informaiton about the resolution of the timer device
	// �μ�2: Size of TIMECAPS (sizeof(TIMECAPS))
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
		// ���� �߻�.
		return -1;
	}

	// TIMECAPS: ����ü, UINT wPeriodMin, UINT wPeriodMax 2�� ������ ����.
	// wPeriodMin: Minimum supported resolution
	// wPeriodMax: Maximum supported resolution
	wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	

	// Ÿ�̸��� �ּ� �ػ󵵸� �����մϴ�.
	if (timeBeginPeriod(wTimerRes) != TIMERR_NOERROR) {
		// ���� �߻�.
		return -1;
	}

	// 1ms �� Ÿ�̸� �̺�Ʈ�� �����մϴ�.
	// �μ�1: Event delay, in milliseconds
	// �μ�2: Resolution of the timer event, in milliseconds
	// �μ�3: Pointer to the callback function that we want to be called periodically
	// �μ�4: User data passed to the callback function
	// �μ�5: Timer event type. May be either TIME_ONESHOT, in which case the callback function is called only once,
	//		or TIME_PERIODIC for periodic calling
	timerID = timeSetEvent(interval, wTimerRes, TimeProc, (DWORD)0, TIME_PERIODIC);
	if (timerID == NULL) {
		// ���� �߻�.
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////

	return 0;
}



int Stop_MMTimer()
{
	/////////////////////////////////////////////////////
	// ��Ƽ�̵�� Ÿ�̸Ӹ� �����մϴ�.
	if (timeKillEvent(timerID) != TIMERR_NOERROR) {
		// ���� �߻�
		return -1;
	}

	// After we are finished with the timer, we should "reset" the timer resolution with a call
	// �μ� (uPeriod): The same as in timeBeginPeriod
	if (timeEndPeriod(wTimerRes) != TIMERR_NOERROR) {
		// ���� �߻�
		return -1;
	}

	return 0;
	/////////////////////////////////////////////

}
*/


LRESULT CALLBACK CB_ComPort_Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//int i;


	// ó�� �ʱ�ȭ �Ǿ����� Ȯ���ϱ� ���� flag
	static bool bFirst_COM = false;

	// �� �޺� ��Ʈ�ѿ� ���� ������ �ڵ� ����
	static HWND hCombo_ComPort, hCombo_BaudRate;

	// ���õ� Combo ��ġ ���� ����
	static int iIndex_Com, iIndex_Baud;


	switch (message)
	{
	case WM_INITDIALOG:

		// �ʱ�ȭ


		//////////////////////////////////////////////////////////////////////////
		// Combo box �� list ����
		// IDC_COMBO_COMPORT, IDC_COMBO_BAUDRATE



		// �ڵ� ���
		hCombo_ComPort = GetDlgItem(hDlg, IDC_COMBO_COMPORT);
		hCombo_BaudRate = GetDlgItem(hDlg, IDC_COMBO_BAUDRATE);

		// ComPort list update: ���� 5��
		for (int i = 0; i < 5; i++)
			SendMessage(hCombo_ComPort, CB_ADDSTRING, 0, (LPARAM)combo_ComPort_List[i]);


		// BaudRate list update: ���� 7��
		for (int i = 0; i < 7; i++)
			SendMessage(hCombo_BaudRate, CB_ADDSTRING, 0, (LPARAM)combo_BaudRate_List[i]);


		// ó�� �ѹ��� ����!!!
		//	if (bFirst_COM == false)
		//	{
		// Default Setting (���� Comport ������ �ִ°� ���°� �Ǵ�?
		SendMessage(hCombo_ComPort, CB_SETCURSEL, iIndex_Com, 0);
		SendMessage(hCombo_BaudRate, CB_SETCURSEL, iIndex_Baud, 0);

		//SendMessage(hCombo_ComPort)

		//	}


		// str_ComPort & str_BaudRate �� �ʱⰪ ����
		strcpy(str_ComPort, combo_ComPort_List[0]);
		strcpy(str_BaudRate, combo_BaudRate_List[2]);




		// ����
		bFirst_COM = true;


		return TRUE;

	case WM_COMMAND:
		//if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)

		switch (LOWORD(wParam)) {

		case IDOK:


			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;

			//		case IDCANCEL:

			//			EndDialog(hDlg, LOWORD(wParam));
			//			return TRUE;



			// Combo Box Control - ComPort, BaudRate
		case IDC_COMBO_COMPORT:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				iIndex_Com = SendMessage(hCombo_ComPort, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo_ComPort, CB_GETLBTEXT, iIndex_Com, (LPARAM)str_ComPort);

				break;
			}

			// Combo Box Control - ComPort, BaudRate
		case IDC_COMBO_BAUDRATE:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				iIndex_Baud = SendMessage(hCombo_BaudRate, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo_BaudRate, CB_GETLBTEXT, iIndex_Baud, (LPARAM)str_BaudRate);

				break;
			}


		}

		return 0;
	}
	return FALSE;
}


void Initialize_Parameter()
{

	//////////////////////////////////////////////////////////////////////////
	// variables initialize
		
	//////////////////////////////////////////////////////////////////////////
	// GL ��ư, �޴� �ʱ�ȭ
	bButton_Homing = false;
	bButton_InitFTSensor = false;
	bButton_FrictionComp = false;
	bButton_GravityComp = false;
	bButton_ImpControl = false;
	bButton_RIC	= false;
	bButton_RobotControl = false;		// Window Button control
	bButton_MotorOn = false;
	bButton_MotorRun = false;
	bButton_DataSave = false;
	bButton_FileSave = false;

	// ������ ���� �ð�.
	//Save_Time = 15 + 1/Control_Rate_Set;

	Save_Time = 12 + 1/Control_Rate_Set;

	//Save_Time = 10 + 1/Control_Rate_Set;

	//Save_Time = 330 + 1/Control_Rate_Set;


	prev_Save_Time = Save_Time;

	dtime = 0.0;

	
	BHoming.x = 750;
	BHoming.y = 340;
	BHoming.w = 120;
	BHoming.h = 25;
	sprintf(BHoming.title, "Homing On");
	BHoming.update(bButton_Homing);	


	BInitFTSensor.x = 750;
	BInitFTSensor.y = 370;
	BInitFTSensor.w = 100;
	BInitFTSensor.h = 25;
	sprintf(BInitFTSensor.title, "FT Sensor Init!");
	BInitFTSensor.update(bButton_InitFTSensor);


	BMotorOn.x = 750;
	BMotorOn.y = 400;
	BMotorOn.w = 120;
	BMotorOn.h = 25;
	sprintf(BMotorOn.title, "Motor On/Off");
	BMotorOn.update(bButton_MotorOn);


	BMotorRun.x = 750;
	BMotorRun.y = 430;
	BMotorRun.w = 120;
	BMotorRun.h = 25;
	sprintf(BMotorRun.title, "Motor Run/Stop");
	BMotorRun.update(bButton_MotorRun);



	BFrictionComp.x = 620;
	BFrictionComp.y = 400;
	BFrictionComp.w = 100;
	BFrictionComp.h = 25;
	sprintf(BFrictionComp.title, "Fric. On/Off");
	BFrictionComp.update(bButton_FrictionComp);

	BGravityComp.x = 620;
	BGravityComp.y = 430;
	BGravityComp.w = 100;
	BGravityComp.h = 25;
	sprintf(BGravityComp.title, "Grav. On/Off");
	BGravityComp.update(bButton_GravityComp);

	
	BRIC.x = 620;
	BRIC.y = 460;
	BRIC.w = 100;
	BRIC.h = 25;
	sprintf(BRIC.title, "RIC. On/Off");
	BRIC.update(bButton_RIC);


	BAdaptiveControl.x = 620;
	BAdaptiveControl.y = 490;
	BAdaptiveControl.w = 100;
	BAdaptiveControl.h = 25;
	sprintf(BAdaptiveControl.title, "Adaptive. On/Off");
	BAdaptiveControl.update(bButton_AdaptiveControl);


	RIC_BReset.x = 620;
	RIC_BReset.y = 520;
	RIC_BReset.w = 100;
	RIC_BReset.h = 25;
	sprintf(RIC_BReset.title, "RIC BReset. On/Off");
	RIC_BReset.update(bButton_RIC_BReset);


		
	
	
	// Save Button
	BDATASAVEButton.x = 750;
	BDATASAVEButton.y = 530;
	BDATASAVEButton.w = 75;
	BDATASAVEButton.h = 25;
	sprintf(BDATASAVEButton.title, "SaveData");
	BDATASAVEButton.update(bButton_DataSave);

	
	// Save File Button
	BSAVEFILEButton.x = 750;
	BSAVEFILEButton.y = 640;
	BSAVEFILEButton.w = 75;
	BSAVEFILEButton.h = 25;
	sprintf(BSAVEFILEButton.title, "SaveFile");
	BSAVEFILEButton.update(bButton_FileSave);
	


	// Save Time
	sSaveTime.x = 750;
	sSaveTime.y = 580;
	sSaveTime.w = 120;
	sSaveTime.h = 15;
	sSaveTime.min = 0.0;
	sSaveTime.max = 100.0;		// Save Time
	sprintf(sSaveTime.title, "Save Time");
	sSaveTime.update(Save_Time);




	sDATASlider.x = 750;
	sDATASlider.y = 620;
	sDATASlider.w = 120;
	sDATASlider.h = 15;
	sDATASlider.min = 0.0;
	sDATASlider.max = Save_Time;		// Save Time
	sprintf(sDATASlider.title, "Saving Display");
	sDATASlider.update(dtime);


	//////////////////////////////////////////////////////////////////////////
	////////////////////	���� ���� ���� �ʱ�ȭ
	for (int i = 0; i < TX_BUF; i++)
	{
		rBuf[i] = 0;

		RxBuf_Temp[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		fHD_Pos[i] = 0.0;
		fHD_Vel[i] = 0.0;
	}

	// ���� BYTE ���� ī����
	rCounter = 0;




	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//////////////////		���� ��� ����
	//////////////////////////////////////////////////////////////////////////

	// str_ComPort & str_BaudRate �� �ʱⰪ ����
	strcpy(str_ComPort, combo_ComPort_List[0]);
	strcpy(str_BaudRate, combo_BaudRate_List[2]);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	//TEST_var1 = 0;

	
	wFile_Cnt = 0;


	// Save Data ���� �޸� �Ҵ�
	// Make save data space
	if (Mem_Buffer != NULL)
	{
		// �޸� ����
		free(Mem_Buffer);

		// �޸� ���� �Ҵ�
		Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));
	}
	else
		// �޸� ���� �Ҵ�
		Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));

		
/*
	// multimedia timer for controller setting
	interval = 1;		// ���� : ms
	
	if (0 != Start_MMTimer())
	{
		MessageBox(NULL, "Can not Start MMTimer Normally!", NULL, NULL);

		// �ý��� ����
		Terminate_System();

	}
*/	

}


void Terminate_System()
{	
/*	
	if (0 != Stop_MMTimer())
		MessageBox(NULL, "Can not terminate MMTimer Normally!", NULL, NULL);
*/
	// OpenGL Rendering ����
	KillTimer(hDlgMainWnd, 1);

	KillGLWindow(&hDCMain, &hRCMain);


	if(hDCMain)
		ReleaseDC(hDlgMainWnd, hDCMain);

	hDCMain = NULL;



	if (NULL != Mem_Buffer)
		free(Mem_Buffer);

	
	// �ð� ������ Ÿ�̸� ����
	delete cFps;

	delete gFps;



	// Main Dialog ����
	EndDialog(hDlgMainWnd,0);

	
}
