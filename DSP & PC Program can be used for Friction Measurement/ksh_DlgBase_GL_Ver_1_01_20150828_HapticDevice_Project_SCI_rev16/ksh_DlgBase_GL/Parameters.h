#ifndef __PARAMETERS_H_
#define __PARAMETERS_H_


typedef unsigned long       DWORD;
typedef unsigned char       BYTE;


#define PI	3.14159265

#define size_buf 500

#define NUM_LIST_BOX 15

//#define	BUFF_SIZE			10000		// 수신 circular queue 버퍼 사이즈
#define	BUFF_SIZE			15000		// 수신 circular queue 버퍼 사이즈


extern double Main_Test1;
extern int MouseX, MouseY;
extern bool IsLeftMouseOn;


extern float fHD_Pos[2], fHD_Vel[2], fHD_Accel[2], fHD_Test;
extern float fHD_FT[2];

// DSP Tx Index
extern float dDSPTxIndex;


// TEST
#define TX_BUF	16
//#define TX_BUF	32			// DSP 에서 연속해서 32개의 data를 보냄



// 자동저장 On/Off 플래그
extern bool bAutoSave;









extern BYTE rBuf[TX_BUF];
//extern unsigned char rBuf[16];
// 수신한 BYTE 갯수 카운터
//extern unsigned char rCounter;
extern BYTE rCounter;
extern DWORD RxBuf_Temp[TX_BUF];


// 모니터링용 변수
extern char str_TEST1[size_buf], str_TEST2[size_buf], str_TEST3[size_buf];
extern float fTEST1, fTEST2, fTEST3, fTEST4, fTEST5, fTEST6, fTEST7, fTEST8, fTEST9, fTEST10;

extern float fTEST81, fTEST82, fTEST83, fTEST84, fTEST85, fTEST86, fTEST87, fTEST88, fTEST89, fTEST90;

extern float fTEST91, fTEST92, fTEST93, fTEST94, fTEST95, fTEST96, fTEST97, fTEST98, fTEST99, fTEST100;

extern char cTEST[8];

extern int TEST_var1, TEST_var2, TEST_var3, TEST_var4, TEST_var5, TEST_var6;

#endif