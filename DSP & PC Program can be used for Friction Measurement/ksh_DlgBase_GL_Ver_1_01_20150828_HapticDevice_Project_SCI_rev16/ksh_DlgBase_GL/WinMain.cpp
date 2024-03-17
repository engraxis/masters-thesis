///////////////////////////////////////
//      프로그램 메뉴얼              //
////////////////////////////////////////

// 참고사항
// Process 의 우선순위를 최고 (Real time)로 설정했지만, High 밖에 안됨.
// MMTimer 기반의 쓰레드는 최고 우선순위를 가짐. (나머지는 Normal 수준)



//////////////////////////////////////////////////////////////////////////
// 프로그램 개선 사항

// 2015.03.13
// 스레드 기반으로 파일 저장! => 메모리에 저장된 data를 파일로 저장시, cpu 가 block 되는 문제 해결!!!

// 2015.03.14
// Control Thread (MMTimer 기반) 의 주기는 고해상도 타이머로 측정함

// 2015.06.22
/*
// 시리얼 통신 기능 구현 중!!!

// 파일 추가: CommThread, stdafx, targetver, SerialPort
// 프로젝트 속성 변경:
	MFC 관련 에러 해결: 프로젝트 속성->General, MFC 사용 -> 공유 DLL에서 MFC 사용



*/

//////////////////  유니코드 /////////////////////////////////
// 이식성을 위해서 C언어 타입이 아닌, 중간 타입을 사용
/*

C 타입				유니코드 타입
char				TCHAR
char*				LPSTR
const char*			LPCTSTR


C 표준함수				유니코드 지원 함수 (앞에 l 을 붙이기)
strlen				lstrlen
strcpy				lstrcpy
strcat				lstrcat
strcmp				lstrcmp
sprintf				wsprintf

// 문자열 상수도 타입이 있으므로, TEXT 매크로를 써야함
TCHAR * str = "string";				// No!!!
TCHAR* str = TEXT("string");		// TEXT 매크로 안에 문자열 상수를 쓴다.


ex) #ifdef UNICODE
typedef wchar_t	TCHAR;
#else
typedef char	TCHAR;
#endif


// 크기가 고정된 타입
				부호 있음					부호없음
32비트			INT32, LONG32				UINT32, ULONG32, DWORD32
64비트			INT64, LONG64				UINT64, ULONG64, DWORD64

// 컴파일되는 환경에 따라 길이가 달라져야 할 경우
INT_PTR, LONG_PTR, UINT_PTR, ULONG_PTR, DWORD_PTR

뒤에 _PTR 이 붙은 타입은 운영체제의 비트수에 따라 크기가 가변적이며 포인터와 같은 길이를 가짐.
포인터와 호환되어야 하는 정수가 필요할 때는 이 타입을 사용해야 함.

*/

//////////////// Serial Communication /////////////////////////
/*

조사 사항: http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=50&MAEULNo=20&no=649883&ref=649883


// 참고문헌: http://magicom9.tistory.com/54
http://msdn.microsoft.com/en-us/library/ff802693.aspx
http://blog.daum.net/pg365/
http://blog.daum.net/_blog/BlogTypeView.do?blogid=0MMaA&articleno=8039404&categoryId=0&regdt=20121225062827


*/







//////////////////////////////////////////////////////////////////////////
// 참고사항...

////////////////// extern 키워드 ///////////////////////////////////
// extern => 외부에 선언 및 정의되었다고 컴파일러에게 알려주는 용도.
// 해당 변수가 어떤 자료형으로, 어디에 (외부) 선언되어 있는지 컴파일러에게 알려주는 메시지.

// 함수의 경우, extern 선언을 생략할 수 있음.


//////////////// static 키워드 ////////////////////////////////
// 전역 변수의 static 선언
// => 이 변수는 외부 파일에서의 접근을 허용하지 않는다.
// => (In other words) 이 변수의 접근범위는 파일 내부로 제한한다.

// 코드의 안정성에 좋음.



////////////////// #include 지시자 /////////////////////////////
// 파일의 내용을 단순히 포함시키는 용도.


////////////////// 인스턴스 (Instance)	//////////////////////////////////
// 실행중인 프로그램 하나를 칭하는 용어.
// hInstance 란 프로그램 자체를 일컫는 정수값이며, 프로그램 내부에서는 자기 자신을 가리키는 1인칭 대명사임.


///////////// SendMessage, PostMessage 차이 ////////////////////
/*
SendMessage: 다이렉트로 바로 전달
   -> 긴급하게 처리하기 때문에 예외 상황에 취약함.
   -> 수신측에 Return 이 올 때까지 계속 기다림.

PostMessage: 메시지 큐에 넣었다가 순서에 맞춰서 전달 (덕분에 느림)
   -> 순차적으로 처리가 되기 때문에 각종 예외 상황에도 큰 문제가 없음.
   -> 수신측에서 Return 이 없어도 계속 작업을 수행함.


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


///////////////////////// Data 저장 관련
extern bool IsDataSaving;
extern unsigned int NumOfData, IndexData, Save_Data_Count;
extern double Save_Time, dtime, *Mem_Buffer;
extern bool IsDataSaveFinish;

extern int wFile_Cnt;



//////////////////////////////////////////////////////////////////////////
//				extern (외부 선언 - 다른 파일에 선언됨)					//
//////////////////////////////////////////////////////////////////////////
// 외부 선언을 알려주는 문장


////////			Graphics.h				//////////////////////

//extern BOOL CreateGLDialog(HWND* hDlg, HDC* hDC, HGLRC* hRC);
//extern GLvoid KillGLDialog(HDC* hDC, HGLRC* hRC);

extern bool InitGLMainWindow(HDC hDC, HGLRC hRC);
extern bool DrawMainWindow(HWND hWnd, HDC hDC, HGLRC hRC);


// OPENGL Window size 변수
extern int WSIZEX;  
extern int WSIZEY;



extern bool bButton_InitFTSensor;

// FT Sensor Init 버튼.  원점 세팅  // DATA Save
extern lwtButton BInitFTSensor, BHoming, BDATASAVEButton, BSAVEFILEButton, BFrictionComp, BGravityComp, BMotorOn, BMotorRun;
extern lwtButton BRIC;
extern lwtButton BAdaptiveControl, RIC_BReset;

extern lwtSlider sSaveTime, sDATASlider;


/////////////////////////////////////////////////////////////

////////////////		Controls.h				//////////////////
extern double Control_Rate_Set;









////////////// Test
// 고행상도 타이머 - 시간 계산용
//Timer cFps;

// control loop rate (고해상도 타이머)
Timer *cFps = new Timer;

// graphic rendering rate (이건 고해상도 타이머가 아님)
FpsTracker* gFps = new FpsTracker(10);




//////////////////////////////////////////////////////////////////////////
///////    File Save 관련 Thread 변수
DWORD ThreadID;
HANDLE hFileSaveThread;





//////////////////////////////////////////////////////////////////////////
					// ComPort Setting Dialog
//////////////////////////////////////////////////////////////////////////
// Callback function for ComPort Setting Dialog
LRESULT CALLBACK CB_ComPort_Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ComPort List: Com1, Com2, Com3, etc
// 2차원 배열, 첫번째는 행에 해당, 2번째는 열의 갯수 (5 는 5개의 열을 의미)
char combo_ComPort_List[][6] = {"COM1", "COM2", "COM3", "COM4", "COM5"};

// BaudRate List: 9600, 19200, 115200
char combo_BaudRate_List[][7] = {"9600", "38400", "19200", "115200", "230400", "460800", "921600"};




// ComPort, BaudRate 에 해당하는 변수
char str_ComPort[6], str_BaudRate[7];


// 접속 주소 문자열
char cComPath[13];

CString strComPath;


// SerialPort class 선언
CSerialPort ComNum;


// CommThread SerialPort class 선언: 시리얼 통신 객체
CCommThread Comm_SCI;

// Receive function
//LONG Comm_Receive(UINT port, LONG lParam);

CString strReceive, strWrite;

// received data
char* cCommRec_Data;


// Send data 문자열 변수
char str_Send_Data[1024];
//CString str_Send_Data;


// Com connect
bool bComPort_On = false;

// 통신 Connect Status
char str_Com_Status[size_buf] = {"Ready"};


// List Display 변수
char str_ListDISP_Data[1024];


/////////////////////// DSP data 수신 관련
// DSP로부터 정보를 수신할 수신 버퍼
BYTE rBuf[TX_BUF];
// 수신한 BYTE 갯수 카운터
BYTE rCounter;

// DSP TX FIFO 크기 (한번에 보내는 송신량)
//#define TX_BUF	16

// unsigned 32 bit data 형: DSP float 형이 32 bit임. float 변환을 위한 임시 변수
DWORD RxBuf_Temp[TX_BUF];


///////////////
///////////// Haptic Device 정보 관련
////////////
// 위치, 속도 정보
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



	// 프로세스 우선순위: 최상위 우선순위 선정: REALTIME_PRIORITY_CLASS
	// Z227 놋북에서는 HIGH_PRIORITY_CLASS (0x80) 밖에 안됨.

	if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
	{
		// Error

		return 0;

	}

	// 프로그램 시작 시간 측정.
	//cFps.start();
	cFps->start();
	

	// TEST
	dQPF = cFps->getFreq();



	// Open Main Dialog
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_MAIN), HWND_DESKTOP, MainDlgProc);

	return 0;
}

// WinAPI 2판: 대화상자 메시지. 대화상자 프로시저 (pp. 575)
// 운영체제 내의 #32770 윈도우 프로시저가 존재함.

// 리턴값은 시스템의 숨겨진 윈도우 프로시저가 받으며 이 값으로부터 디폴트 처리 여부를 결정한다.
// 만약 대화상자 프로시저가 TRUE를 리턴했다면 메시지를 잘 처리한 것이므로 더 이상의 처리를 할 필요가 없으며,
// FALSE를 리턴했다면 자신이 직접 처리하거나 아니면 DefWindowProc으로 처리를 넘길 것이다.
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	//////////////////////////////////////////////////////////////////////////
	// TEST variables

	// 각 컨트롤에 대한 윈도우 핸들 선언
	static HWND hButton_ComPort_On;
	static HWND hEdit_Send_Data;
//	static HWND hList_DISPLAY;
	

	//////////////////////////////////////////////////////////////////////////
	

	switch (iMessage) {
	case WM_INITDIALOG:
		// 다이얼로그 초기화 부분
		hDlgMainWnd = hDlg;


		// TEST
		dwMainThreadPri = GetThreadPriority(GetCurrentThread());


		

		// First, let's get the handle to all the window we created in the Dialog
		// with the resource editor
		//hGLWnd = GetDlgItem(hDlg, IDC_GL_WND);
		hGLWnd = GetDlgItem(hDlg, IDC_GL_WND);

						

		// OpenGL 부분: Picture Control 의 핸들을 넘김.  Picture control 에 GL Rendering 수행함.
		//if(CreateGLDialog(&hGLWnd, &hDCMain, &hRCMain) == FALSE)
		if(CreateGLWindow(&hGLWnd, &hDCMain, &hRCMain) == FALSE)
		{
			MessageBox(NULL,"Error initializing OpenGL Window","OpneGL",MB_OK);
			return FALSE;
		}
	

		//////////////////////////////////////////////////////////////////////////
		// variables initialize (변수 등의 초기화는 Initialize_Parameter() 함수 안에 넣으면 됩니다.)

		Initialize_Parameter();

		// Handle 얻기
		//Comm_SCI.hCommWnd = hGLWnd;
		Comm_SCI.hCommWnd = hDlgMainWnd;

		//////////////////////////////////////////////////////////////////////////


		// Open GL Window 초기화.
		InitGLMainWindow(hDCMain, hRCMain);


		// Windows Timer: Graphic rendering timer
		SetTimer(hDlgMainWnd, 1, GRAPHIC_UPDATE_PERIOD, NULL);



		


		return TRUE;


	case WM_CLOSE:

		// 각종 종료 함수들
		Terminate_System();

		return TRUE;



	case WM_COMM_READ:
		// Comm receive message handler

		TEST_var3++;

		//Comm_Receive(NULL, lParam);

		// 다시 수신 메세지를 보낼 수 있도록 갱신함.
		//Comm_SCI.m_bReserveMsg = false;

		return 0;
		


	case WM_COMMAND:
		// 핸들 이름 정의
		HWND hBControl, hBImpControl, hBGoHome;

	//	double Ts;


		switch (LOWORD(wParam))
		{
			// LOWORD(wParam) 에 메시지를 보낸 컨트롤의 ID가 전달됨
			// HIWORD(wParam)에 통지 코드가 전달됨.

			// 메뉴: File -> Exit 
		case ID_FILE_EXIT:
		case IDCANCEL:
			
			// 각종 종료 함수들
			Terminate_System();
			
			return TRUE;

		// Setting 메뉴 클릭
		case ID_MENU_SETTING:
						

			bButton_RobotControl = false;
			
//			DialogBox(g_hInst, (LPCTSTR)IDD_SETTING, hDlgMainWnd, (DLGPROC)Setting);

			break;

		// COMPORT Setting 메뉴 클릭
		case ID_MENU_SETTING_COMPORT:

			// Open Comport setting dialog box
			DialogBox(g_hInst, (LPCTSTR)IDD_COMPORT_SETTING, hDlgMainWnd, (DLGPROC)CB_ComPort_Setting);

					
			

			break;

			//////////////////////////////////////////////////////////////////////////
			//    COMPORT  관련 COMMAND
			//////////////////////////////////////////////////////////////////////////

			// COM Connect or Disconnect
		case IDC_BUTTON_COMPORT_CONNECT:
	
			if (!Comm_SCI.m_bConnected)		// Try to Connect!
			{
				
				sprintf_s(str_TEST2, size_buf, "On");


				//CString strPortNum;

				// 접속 주소 문자열
				//char cComPath[13];

				sprintf_s(cComPath, 13, "\\\\.\\%s", str_ComPort);


				// char * --> CString 변환
				strComPath.Format("%s", cComPath);

				// TEST Monitoring
				//strcpy(str_TEST, strComPath);
				strcpy(str_TEST1, str_BaudRate);
				


				// 통신 속도 (BaudRate 검사)
//				if (!strcmp(str_BaudRate, "115200"))		// 문자열이 일치하면 0 을 반환하므로!!!
//				{
					// BaudRate: 57600 bps
					//if (ComNum.Open(cComPath, CBR_115200, 8, ONESTOPBIT, NOPARITY))
					if (Comm_SCI.OpenPort(strComPath, str_BaudRate, "8", "NO", "NO", "1"))
					{
						// 접속 성공

						sprintf_s(str_Com_Status, size_buf, "Success!");

						win_test = 1;

						SetDlgItemText(hDlgMainWnd, IDC_BUTTON_COMPORT_CONNECT, "Disconnect");


						//////////////////////////////////////////////////////////////////////////
						////////////////	List Box에 문자열 추가 (통째로 사용하기)
						//////////////////////////////////////////////////////////////////////////
						CString strTemp;
				
						strTemp = strComPath + _T("(") + str_BaudRate + _T(")");

						strTemp += "에 접속 되었음.";
				
						// CString to char 변환
						strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
						// ListBox - DISPLAY 핸들 얻기
						hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

						SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);
						
						// 항목 수 체크
						int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

						if (getLboxitems > NUM_LIST_BOX)
							// return: 남은 항목수
							getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

						SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);
						//////////////////////////////////////////////////////////////////////////



					}
					else
					{
						// 접속 실패
						sprintf_s(str_Com_Status, size_buf, "Failed!");

						win_test = 10;


						SetDlgItemText(hDlgMainWnd, IDC_BUTTON_COMPORT_CONNECT, "Connect");

						//////////////////////////////////////////////////////////////////////////
						////////////////	List Box에 문자열 추가
						//////////////////////////////////////////////////////////////////////////
						CString strTemp;
				
						strTemp = strComPath + _T("(") + str_BaudRate + _T(")");

						strTemp += "에 접속 불가능!";
				
						// CString to char 변환
						strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
						// ListBox - DISPLAY 핸들 얻기
						hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

						SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);
						
						// 항목 수 체크
						int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

						if (getLboxitems > NUM_LIST_BOX)
							// return: 남은 항목수
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
			else		// 접속 종료
			{

				//str_TEST2 = "";
				sprintf_s(str_TEST2, size_buf, "Off");


				SetDlgItemText(hDlgMainWnd, IDC_BUTTON_COMPORT_CONNECT, "Connect");

				Comm_SCI.ClosePort();

				//////////////////////////////////////////////////////////////////////////
				////////////////	List Box에 문자열 추가
				//////////////////////////////////////////////////////////////////////////
				CString strTemp;
				
				strTemp = "연결 종료되었습니다.";
													
				// CString to char 변환
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
				// ListBox - DISPLAY 핸들 얻기
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);
						
				// 항목 수 체크
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: 남은 항목수
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);
				//////////////////////////////////////////////////////////////////////////

			}



			// Open Selected Com Port


			break;

			// End
			//////////////////////////////////////////////////////////////////////////








/////////////////// EDIT 박스 (SCI 통신 용)
			// Write a message for Serial communication
		case IDC_EDIT_COMPORT_WRITE:

			// 핸들 얻기
			hEdit_Send_Data = GetDlgItem(hDlgMainWnd, IDC_EDIT_COMPORT_WRITE);

			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				GetWindowText(hEdit_Send_Data, str_Send_Data, 1024);
				

			}

			break;



			// Send data to MCU
		case IDC_BUTTON_COMPORT_WRITE:

			// 연결되었을 때만!

			// TEST
			bComPort_On = true;


			if (bComPort_On)
			{
				// Send data which is obtained from Edit control (str_Send_Data)
				int n;

				n = strlen(str_Send_Data);


				strWrite.Format("%s", str_Send_Data);

				// 정상 작동함.
			//	Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength()+1);

				// 이렇게 하면 널문자가 빠지지 않을까?
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

								


			//	PrintStr(&CString(">> " + strWrite.GetString()));

				CString strTemp;
				
				strTemp = ">> ";

				strTemp += str_Send_Data;
				
				// CString to char 변환
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));
		
				// ListBox - DISPLAY 핸들 얻기
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// 항목 수 체크
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: 남은 항목수
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


			// 임피던스 버튼 On/Off
		case IDC_BUTTON_IMP_CONTROL:

			++wFile_Cnt;


			return TRUE;



		// 리셋 버튼
		case IDC_BUTTON_RESET:


			// Reset Everything!

		
			// Data Save Reset
			IsDataSaving = FALSE;
			sDATASlider.Enable = FALSE;

			NumOfData = 0;
			IndexData = 0;
			Save_Data_Count = 0;	// Count the number of save data


			// 저장 파일의 순차 넘버링을 위한 변수.
			wFile_Cnt = 0;


			// 자동저장 On/Off 플래그
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
			// 핸들 얻기
			hBControl = GetDlgItem(hDlgMainWnd, IDC_BUTTON_CONTROL);

			//////// 버튼 캡션 변경		///////////////////
			// 캡션 변경
			SetWindowText(hBControl, TEXT("Control On"));



			bButton_ImpControl = false;
			// 핸들 얻기
//			HWND hBImpControl;
			hBImpControl = GetDlgItem(hDlgMainWnd, IDC_BUTTON_IMP_CONTROL);
			
			SetWindowText(hBImpControl, TEXT("Imp. Control On"));



			return TRUE;

		case IDC_BUTTON_READY:
			// bSafety 버튼 정상화


	

			return TRUE;


		case IDC_BUTTON_GOHOME:
			// 제어 오프후에 Back Home 하기 위한 플래그

	//		bButton_GoHome = !bButton_GoHome;

			// 핸들 얻기
			
			hBGoHome = GetDlgItem(hDlgMainWnd, IDC_BUTTON_GOHOME);

			//////////////////////////////////////////////////////////////////////////
			// 제어 On/Off 체크
			if (!bButton_GoHome)
			{
				// Enalbe GoHome

				//////// 버튼 캡션 변경		///////////////////
				// 캡션 변경
				SetWindowText(hBGoHome, TEXT("Disable GoHome"));



				// Enalbe GoHome
				//////////////////////////////////////////////////////////////////////////
				// 제어 변수 세팅.
//				sprintf_s(buffer_Err, size_buf, "[Err. Msg]: OK: GoHome");
		//		bSafety = true;



				// true
				bButton_GoHome = !bButton_GoHome;
				

			}
			else
			{
				// 캡션 변경
				SetWindowText(hBGoHome, TEXT("Enable GoHome"));


				// Reset Control Torque
	//			for (int i = 0; i < 2; i++)
	//				Control_Input[i] = 0.0;


				// false;
				bButton_GoHome = !bButton_GoHome;

			}
			


			return TRUE;


		}
		// default 프로시저로 보내기.
		return FALSE;






	case WM_LBUTTONDOWN:
		MouseX=LOWORD(lParam);
		MouseY=HIWORD(lParam);
		IsLeftMouseOn = true;
		//////////////////////////////////////////////////////////////////////////
		////////////////////   OpenGL GUI 명령 수행 관련 //////////////////////
		//////////////////////////////////////////////////////////////////////////
		
		// 영점 조정 On
		BHoming.read(&bButton_Homing, lParam, true);

		// Motor Enable On/OFF
		BMotorOn.read(&bButton_MotorOn, lParam, true);


		// 클릭 명령 수행 체크
		if (BMotorOn.bClicked)
		{
			// Enable 명령을 DSP로 송신

			// 연결되었을 때만!
			if (Comm_SCI.m_bConnected)
			{
				CString str_TBuf;

				if (bButton_MotorOn)
				{
					// 16개 data 송신
					// ENT 송신, 나머지 13개는 zzzzzzzzz
					str_TBuf = "CMD:ENTzzzzzzzzz";

				}
				else
				{
					// 16개 data 송신
					// ENF 송신, 나머지 13개는 zzzzzzzzz
					str_TBuf = "CMD:ENFzzzzzzzzz";


					// Run/Stop 버튼 종료
					bButton_MotorRun = false;
					BMotorRun.update(bButton_MotorRun);

				}

				// 송신을 위한 변수에 대입
				strWrite.Format("%s", str_TBuf);

				// 이렇게 하면 널문자가 빠지지 않을까? (GetLength()에서 +1 제거)
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

				CString strTemp;

				strTemp = ">> ";

				strTemp += str_TBuf;

				// CString to char 변환
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));

				// ListBox - DISPLAY 핸들 얻기
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				// LISTBOX에 행 추가 메시지 발송
				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// 항목 수 체크
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: 남은 항목수
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				// LISTBOX에 현재 행 삭제 메시지 발송
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);

			}



			// false로 리셋
			BMotorOn.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}



		// Motor Run/Stop
		BMotorRun.read(&bButton_MotorRun, lParam, true);


		// 클릭 명령 수행 체크
		if (BMotorRun.bClicked)
		{
			// Run/Stop 명령을 DSP로 송신

			// 연결되었을 때만!
			if (Comm_SCI.m_bConnected)
			{
				CString str_TBuf;

				if (bButton_MotorRun)
				{
					// 16개 data 송신
					// ENT 송신, 나머지 13개는 zzzzzzzzz
					str_TBuf = "CMD:ENRzzzzzzzzz";

				}
				else
				{
					// 16개 data 송신
					// ENF 송신, 나머지 13개는 zzzzzzzzz
					str_TBuf = "CMD:ENSzzzzzzzzz";

				}

				// 송신을 위한 변수에 대입
				strWrite.Format("%s", str_TBuf);

				// 이렇게 하면 널문자가 빠지지 않을까? (GetLength()에서 +1 제거)
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

				CString strTemp;

				strTemp = ">> ";

				strTemp += str_TBuf;

				// CString to char 변환
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));

				// ListBox - DISPLAY 핸들 얻기
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				// LISTBOX에 행 추가 메시지 발송
				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// 항목 수 체크
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: 남은 항목수
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				// LISTBOX에 현재 행 삭제 메시지 발송
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);

			}



			// false로 리셋
			BMotorRun.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}
		


		
		// Friction Compensation On/Off
		BFrictionComp.read(&bButton_FrictionComp, lParam, true);



		// 클릭 명령 수행 체크
		if (BFrictionComp.bClicked)
		{
			// Friction Comp. On/Off 명령을 DSP로 송신

			// 연결되었을 때만!
			if (Comm_SCI.m_bConnected)
			{
				CString str_TBuf;

				if (bButton_FrictionComp)
				{
					// 16개 data 송신
					// FRT 송신, 나머지 13개는 zzzzzzzzz
					str_TBuf = "CMD:FRTzzzzzzzzz";

				}
				else
				{
					// 16개 data 송신
					// FRF 송신, 나머지 13개는 zzzzzzzzz
					str_TBuf = "CMD:FRFzzzzzzzzz";

				}

				// 송신을 위한 변수에 대입
				strWrite.Format("%s", str_TBuf);

				// 이렇게 하면 널문자가 빠지지 않을까? (GetLength()에서 +1 제거)
				Comm_SCI.WriteComm((BYTE *)strWrite.GetString(), strWrite.GetLength());

				CString strTemp;

				strTemp = ">> ";

				strTemp += str_TBuf;

				// CString to char 변환
				strcpy(str_ListDISP_Data, strTemp.GetBuffer(0));

				// ListBox - DISPLAY 핸들 얻기
				hList_DISPLAY = GetDlgItem(hDlg, IDC_LIST_COMPORT_DISPLAY);

				// LISTBOX에 행 추가 메시지 발송
				SendMessage(hList_DISPLAY, LB_ADDSTRING, 0, (LPARAM)str_ListDISP_Data);


				// 항목 수 체크
				int getLboxitems = SendMessage(hList_DISPLAY, LB_GETCOUNT, NULL, NULL);

				if (getLboxitems > NUM_LIST_BOX)
					// return: 남은 항목수
					getLboxitems = SendMessage(hList_DISPLAY, LB_DELETESTRING, 0, NULL);

				// LISTBOX에 현재 행 삭제 메시지 발송
				SendMessage(hList_DISPLAY, LB_SETCURSEL, getLboxitems-1, NULL);

			}



			// false로 리셋
			BFrictionComp.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}

		




		
		// Gravity Compensation On/Off
		BGravityComp.read(&bButton_GravityComp, lParam, true);

		// Impedance Control On/Off
		BRIC.read(&bButton_RIC, lParam, true);

		
		BAdaptiveControl.read(&bButton_AdaptiveControl, lParam, true);

		RIC_BReset.read(&bButton_RIC_BReset, lParam, true);


		// FT Reset 버튼
		BInitFTSensor.read(&bButton_InitFTSensor, lParam, true);


	
		// Data Save 시간 설정 슬라이더바
		sSaveTime.read(&Save_Time, lParam, true);

		// Data Save On/Off 명령
		BDATASAVEButton.read(&bButton_DataSave, lParam, true);


		// 클릭 명령 수행 체크
		if (BDATASAVEButton.bClicked)
		{
			// 자동저장 On/Off 플래그
			bAutoSave = true;
					

			// false로 리셋
			BDATASAVEButton.bClicked = false;
			//////////////////////////////////////////////////////////////////////////
		}



		

		
		// Make Save File On/Off 명령
		BSAVEFILEButton.read(&bButton_FileSave, lParam, true);


		// Create file to save data in the memory
		// 메모리에 저장된 data를 disk 에 저장하기 위해 thread 호출
		// 검사
		if (IsDataSaveFinish && !bButton_FileSave)
		{
			// 저장된 data를 파일로 저장하기 위한 스레드 생성
			// HANDLE CreateThread
			// 인수1: 보안 속성 지정: 자식 프로세스로 핸들을 상속하지 않는 한 NULL 로 지정
			// 인수2: 스레드의 스택 크기. 0으로 지정하면 주 스레드 (CreateThread를 호출한 스레드)와 같은 크기를 가짐
			//      기본적으로 주 스레드의 스택은 1 M가 예약되며, 그 중 한 페이지(보통 4K)만 확정되어 있음.
			//      만약 재귀 호출의 깊이가 아주 깊다면 스택 크기를 더 늘릴 수도 있음.
			// 인수3: 스레드의 시작 함수 지정 (Entry point). 주 스레드의 WinMain 함수에 해당한다고 보면 됨.
			//		이 함수로부터 스레드의 실행을 시작하며, 시작 함수가 종료되면 스레드로 종료됨.
			//		시작함수는 다음과 같은 원형을 가져야 함.
			//		DWORD WINAPI ThreadFunc(LPVOID lpParameter);
			// 인수4: 스레드 시작함수의 인수 (lpParameter). 인수가 없을 경우 NULL 전달.
			// 인수5: 스레드의 특성 지정. 0이면, 아무 특성 없는 보통 스레드임.
			//		CREATE_SUSPENDED 플래그: 스레드를 만들기만 하고, 실행은 하지 않음.
			//		중지된 스레드를 실행할 때는 ResumeThread 함수를 호출하면 됨.
			// 인수6: 스레드의 ID를 리턴하기 위한 출력용 인수.
			//		스레드 ID가 필요한 경우는 별로 없는데, 이 경우는 NULL 전달. (비고: 9x계열은 이 인수가 NULL이면, 스레드가 생성되지 않음.)
			//		호환성을 위해서는 DWORD형의 변수를 선언한 후 번지를 넘겨야 함.

			// 결과: 에러가 발생했을 경우, NULL 을 리턴함.
			hFileSaveThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);

			// 스레드의 핸들과 스레드 자체는 다르므로, 핸들을 닫는다고 해서 스레드가 종료되는 것은 아님.
			CloseHandle(hFileSaveThread);
		}




		// 이 콜백에서 완료
		return TRUE;

	case WM_LBUTTONUP:
		IsLeftMouseOn = false;


		if (prev_Save_Time != Save_Time)
		{
			// 저장 시간이 변경됨!
			sDATASlider.max = Save_Time;

			// Save Data 동적 메모리 할당
			// Make save data space
			if (Mem_Buffer != NULL)
			{
				// 메모리 해제
				free(Mem_Buffer);

				// 메모리 새로 할당
				Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));
			}
			else
				// 메모리 새로 할당
				Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));


			// 갱신
			prev_Save_Time = Save_Time;

		}



		return TRUE;

	case WM_MOUSEMOVE:

		// Save Time 관련 슬라이더바 값 변경
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
			// 약 30 Hz 화면 렌더링
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
	// MMTimer 설정
	TIMECAPS tc;

	// 타이머의 최소/최대 해상도를 얻어옵니다.
	// 인수1: Pointer to a TIMECAPS structure. It is filled with informaiton about the resolution of the timer device
	// 인수2: Size of TIMECAPS (sizeof(TIMECAPS))
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
		// 에러 발생.
		return -1;
	}

	// TIMECAPS: 구조체, UINT wPeriodMin, UINT wPeriodMax 2개 변수만 있음.
	// wPeriodMin: Minimum supported resolution
	// wPeriodMax: Maximum supported resolution
	wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	

	// 타이머의 최소 해상도를 설정합니다.
	if (timeBeginPeriod(wTimerRes) != TIMERR_NOERROR) {
		// 에러 발생.
		return -1;
	}

	// 1ms 의 타이머 이벤트를 시작합니다.
	// 인수1: Event delay, in milliseconds
	// 인수2: Resolution of the timer event, in milliseconds
	// 인수3: Pointer to the callback function that we want to be called periodically
	// 인수4: User data passed to the callback function
	// 인수5: Timer event type. May be either TIME_ONESHOT, in which case the callback function is called only once,
	//		or TIME_PERIODIC for periodic calling
	timerID = timeSetEvent(interval, wTimerRes, TimeProc, (DWORD)0, TIME_PERIODIC);
	if (timerID == NULL) {
		// 에러 발생.
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////

	return 0;
}



int Stop_MMTimer()
{
	/////////////////////////////////////////////////////
	// 멀티미디어 타이머를 해제합니다.
	if (timeKillEvent(timerID) != TIMERR_NOERROR) {
		// 에러 발생
		return -1;
	}

	// After we are finished with the timer, we should "reset" the timer resolution with a call
	// 인수 (uPeriod): The same as in timeBeginPeriod
	if (timeEndPeriod(wTimerRes) != TIMERR_NOERROR) {
		// 에러 발생
		return -1;
	}

	return 0;
	/////////////////////////////////////////////

}
*/


LRESULT CALLBACK CB_ComPort_Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//int i;


	// 처음 초기화 되었는지 확인하기 위한 flag
	static bool bFirst_COM = false;

	// 각 콤보 컨트롤에 대한 윈도우 핸들 선언
	static HWND hCombo_ComPort, hCombo_BaudRate;

	// 선택된 Combo 위치 저장 변수
	static int iIndex_Com, iIndex_Baud;


	switch (message)
	{
	case WM_INITDIALOG:

		// 초기화


		//////////////////////////////////////////////////////////////////////////
		// Combo box 에 list 삽입
		// IDC_COMBO_COMPORT, IDC_COMBO_BAUDRATE



		// 핸들 얻기
		hCombo_ComPort = GetDlgItem(hDlg, IDC_COMBO_COMPORT);
		hCombo_BaudRate = GetDlgItem(hDlg, IDC_COMBO_BAUDRATE);

		// ComPort list update: 현재 5개
		for (int i = 0; i < 5; i++)
			SendMessage(hCombo_ComPort, CB_ADDSTRING, 0, (LPARAM)combo_ComPort_List[i]);


		// BaudRate list update: 현재 7개
		for (int i = 0; i < 7; i++)
			SendMessage(hCombo_BaudRate, CB_ADDSTRING, 0, (LPARAM)combo_BaudRate_List[i]);


		// 처음 한번만 수행!!!
		//	if (bFirst_COM == false)
		//	{
		// Default Setting (현재 Comport 선택이 있는가 없는가 판단?
		SendMessage(hCombo_ComPort, CB_SETCURSEL, iIndex_Com, 0);
		SendMessage(hCombo_BaudRate, CB_SETCURSEL, iIndex_Baud, 0);

		//SendMessage(hCombo_ComPort)

		//	}


		// str_ComPort & str_BaudRate 에 초기값 설정
		strcpy(str_ComPort, combo_ComPort_List[0]);
		strcpy(str_BaudRate, combo_BaudRate_List[2]);




		// 갱신
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
	// GL 버튼, 메뉴 초기화
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

	// 데이터 저장 시간.
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
	////////////////////	수신 관련 변수 초기화
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

	// 수신 BYTE 갯수 카운터
	rCounter = 0;




	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//////////////////		직렬 통신 관련
	//////////////////////////////////////////////////////////////////////////

	// str_ComPort & str_BaudRate 에 초기값 설정
	strcpy(str_ComPort, combo_ComPort_List[0]);
	strcpy(str_BaudRate, combo_BaudRate_List[2]);


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	//TEST_var1 = 0;

	
	wFile_Cnt = 0;


	// Save Data 동적 메모리 할당
	// Make save data space
	if (Mem_Buffer != NULL)
	{
		// 메모리 해제
		free(Mem_Buffer);

		// 메모리 새로 할당
		Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));
	}
	else
		// 메모리 새로 할당
		Mem_Buffer = (double *)calloc((int)Control_Rate_Set * Save_Time * NUM_OF_DATA, sizeof(double));

		
/*
	// multimedia timer for controller setting
	interval = 1;		// 단위 : ms
	
	if (0 != Start_MMTimer())
	{
		MessageBox(NULL, "Can not Start MMTimer Normally!", NULL, NULL);

		// 시스템 종료
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
	// OpenGL Rendering 종료
	KillTimer(hDlgMainWnd, 1);

	KillGLWindow(&hDCMain, &hRCMain);


	if(hDCMain)
		ReleaseDC(hDlgMainWnd, hDCMain);

	hDCMain = NULL;



	if (NULL != Mem_Buffer)
		free(Mem_Buffer);

	
	// 시간 측정용 타이머 해제
	delete cFps;

	delete gFps;



	// Main Dialog 종료
	EndDialog(hDlgMainWnd,0);

	
}
