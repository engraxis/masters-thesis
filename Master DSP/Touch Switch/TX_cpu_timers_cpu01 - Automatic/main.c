// 0: Translation
// 1: Twisting
//###########################################################################
// FILE:   cpu_Timers_cpu01.c
// TITLE:  Cpu Timers Example for F2837xS.
//
/* 프로그램 설명 (CCS v6.1) (DSP: F28377S)
 * 1. CPUTimer01 주기적 인터럽트: 5 kHz sampling rate => 제어 주기....2015. 05. 21. (완료)
 * 2. SPI: DAC 테스트 (TLV5638): 아날로그 출력 => 모터 제어용.........2015. 06. 18.  (완료)
 * 3. EQEP: 엔코더 신호 해석 테스트:  엔코더 신호를 각도로 해석하기...........2015. 06. 18 (1개만 테스트함.)
 * 4. SCI 통신 (PC - DSP) 테스트
 *
 *
 *
 * 비고: bool 형 (unsigned char)을 쓰면, Debug모드에서, 값이 아스키 코드로 표시되는 것 같음. Uint16을 대신 사용. (크기 같음)
 *
 *
 *
 */
//
//###########################################################################
// $TI Release: F2837xS Support Library v160 $
// $Release Date: Mon Jun 15 14:14:46 CDT 2015 $
// $Copyright: Copyright (C) 2014-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F28x_Project.h"     // Device Headerfile and Examples Include File

#include <math.h>


// LED ON/OFF 제어용 상수 정의 -------------------------------------------------------
#define GREEN_LED_ON		GpioDataRegs.GPASET.bit.GPIO31=1	// 녹색 LED ON
#define GREEN_LED_OFF		GpioDataRegs.GPACLEAR.bit.GPIO31=1	// 녹색 LED OFF
//#define RED_LED_ON			GpioDataRegs.GPBSET.bit.GPIO34=1	// 적색 LED ON
//#define RED_LED_OFF			GpioDataRegs.GPBCLEAR.bit.GPIO34=1	// 적색 LED OFF
//#define RED_LED_TOGGLE		GpioDataRegs.GPBTOGGLE.bit.GPIO34=1	// 적색 LED TOGGLE
// -----------------------------------------------------------------------------------

#define BUTTON GpioDataRegs.GPADAT.bit.GPIO9;
#define START_MOTOR_CONTROL GpioDataRegs.GPBDAT.bit.GPIO43;
#define STOP_MOTOR_CONTROL GpioDataRegs.GPBDAT.bit.GPIO42;

#define LOOPRATE	GpioDataRegs.GPCTOGGLE.bit.GPIO69 = 1;

// Motor Enable ON/OFF 제어용 상수 정의 -------------------------------------------------------
#define MOTOR_ON		GpioDataRegs.GPBSET.bit.GPIO33=1	// Motor ON
#define MOTOR_OFF		GpioDataRegs.GPBCLEAR.bit.GPIO33=1	// Motor OFF
#define TWIST_MOTOR_ON	GpioDataRegs.GPBSET.bit.GPIO34=1
#define TWIST_MOTOR_OFF	GpioDataRegs.GPBCLEAR.bit.GPIO34=1
// -----------------------------------------------------------------------------------

// 반올림 매크로 함수 (소수점 첫째자리)--------------------------------------------------------------
// float 형이 정수형으로 바뀔 때, 오차를 줄이기 위해서.
#define ROUND(X)	((X)+(0.5))

//------------------------------------------------------------------------

// sign or signum 매크로 함수
#define SIGN(x)		(  ((x) < 0) ? -1 : ( ((x) > 0) ? 1 : 0.0  )   )
//------------------------------------------------------


// Note: 힘 단위: [N], 토크 단위: [mNm], 병진 단위: [mm], Twisting 단위: [rad]
// 기준:
// Pose
/////Push 방향이 +,  오른쪽 Twisting이 + (병진 위치의 부호 변경함)

// Force/Torque
//// 전진 방향이 + 힘, 오른쪽 Twisting이 + 토크 (Twisting 토크 방향 변경함)


// Pull limit: 39.08 => 30.0 [mm]
// Push limit: -24.91 => -20.0 [mm]

// PI: 3.14159265359
#define PI		3.14159265359

// 병진부: 회전운동 => 병진운동 [mm]
// 와이어선 원통 지름: 13.1 [mm] = 2*r
// 와이어선 원통 반지름: 6.55 [mm]
// 6.55 [mm]
#define TRANS_R			6.55

// 원통부 피치: 1 [mm]

//////////////////////////////////////////////////////////////////////////////
//////////			제어 주기			//////////////////////////////////////////
// 5 kHz => 0.2 ms = 200 us
// 5 kHz => 200
//#define CONTROL_HZ	5000
//#define CONTROL_RATE 200	// CPU 주기 결정하는 값

// 4 kHz
#define CONTROL_RATE 250	// CPU 주기 결정하는 값
#define CONTROL_HZ	4000
#define Ts			0.00025		// 제어 주기 [s]

// Prototype statements for functions found within this file.
__interrupt void cpu_timer0_isr(void);

float32 Timer_Prd;		// 제어 주기 [usec] 단위

///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//////////////////   SCI (PC - DSP 통신) 관련		///////////////////////////////

// 수신: 인터럽트 방식
interrupt void sciaRxFifoIsr(void);	// FIFO RX interrupt
interrupt void scibRxFifoIsr(void);	// FIFO-B RX interrupt
void scia_fifo_init(void);			// SCI-A 포트 초기화
void scib_fifo_init(void);

// Global variables used in this system
// FIFO
Uint16 TxBuffA[16];
Uint16 RxBuffA[16];
Uint16 TxBuffB[16];
Uint16 RxBuffB[16];
//Uint16 rdata_pointA; // Used for checking the received data

Uint16 TxFifoLevel;
Uint16 RxFifoLevel;
//Uint16 SCI_RXintCount;
Uint16 SCI_RxFlag;
Uint16 SCIB_RxFlag;
Uint16 i;


Uint32 MasterForce[2];
float32 fMasterForce[2];
float32 fMasterTorqueDirection;
float32 fMasterTorqueMagnitude;
Uint16 SCI_cnt, SCI_cnt1;


// 2번째 송신을 위한 flag 설정
Uint16 bNextTx;
Uint16 bStartMotorControl;
Uint16 bStopMotorControl;

float32 fPushCalib;
float32 fPullCalib;


// Save Data 송신 Index; (십만개에 리셋: 1 kHz 의 경우, 100초)
float fSCI_TxIndex;


//////////////////////////////////////////////////
/////////////		variables for data communication between DSP and PC
//////////////////////////////////////////////////////////////////////////////
//	특히 float data 에 대해서.
#define NUM_OF_BUF	16
// float -> Uint32 저장형 변수 (송신용)
Uint32 tBuf[NUM_OF_BUF];
Uint16 Tx_cnt;

// 8개 송신할 data 임시 저장소
float fTempTxBuf[8];

// TEST variables
Uint32 test_Buf[NUM_OF_BUF];
Uint32 test_BufB[NUM_OF_BUF];
//a float ftest_Buf, ftest_Buf1, ftest_Buf2, ftest_Buf3;
Uint32 test_Rev_Buf[NUM_OF_BUF];
Uint32 test_Rev_BufB[NUM_OF_BUF];



/////////////////////////////////////////////////////////////////////////
/////////				DAC (TLV5638)
////////////////////////////////////////////////////////////////////////
// DAC (TLV5638): SPI 방식으로 조작
#include "TLV5638_API_F2837xS.h"

Uint16	BackTicker;
Uint16	TestDataA;
Uint16	TestDataB;
TLV5638 DAC = TLV5638_DEFAULTS;
Uint16 xa;



///////////////////////////////////////////////////////////////////////
///////		엔코더 관련 (EQEP)
/////////////////////////////////////////////////////////////////////
/* Define for used Encoder Resolution */
// 엔코더 변경: 병진: 8000, Twisting: 1024
#define	ENCODER_REV1		8000			/* Pulse/Revolution */
#define	ENCODER_REV2		1024			/* Pulse/Revolution */

// Encoder 관련
void InitEQep1(Uint32 LineEncoder1);
void InitEQep2(Uint32 LineEncoder2);

// Encoder 관련
//Uint32	PositionMax;
Uint32	PositionMax[2];			// EQEP-1 모듈 내장 위치 카운터(Position Counter)의 최대값
Uint32	PositionCounter[2];		// EQEP-1 모듈 내장 위치 카운터(Position Counter)의 현재값
Uint32	RotateDirection[2];		// EQEP-1 모듈의 Quadrature Decoder Unit이 해석해낸 회전방향 (0 : 역방향, 1 : 정방향)
float32	RotateAngleUnit[2];		// EQEP-1 모듈로 해석해낸 현재 회전각을 비율로 환산한 값 (pu : 0.0 ~ 1.0)
float32	RotateAngleDegree[2];	// EQEP-1 모듈로 해석해낸 현재 회전각도(Degree)
float32	RotateAngleRad[2];	// EQEP-1 모듈로 해석해낸 현재 회전각도(Rad)

int16 NumOfTurns[2];

// HD Pos!
float32 fCurrentPos[2];
// HD Vel, Accel
float32 fCurrentVel[2], fCurrentAccel[2];
float32 fprePos[2], fpreVel[2];

float32 fCurrentPos_ToSend[2];
float32 fPos_JustBeforeButton[2];
float32 fMoveDuringButton[2];

// 병진부 회전각 저장 임시 변수
float32 fTransRad;


////////////////////////////////////
///////////		터치 스위치		////////
Uint16 bInput_TouchSW;		// 터치 스위치 입력


Uint16 bHandShake;
Uint16 bSwitch;
//////////////////////////////////////////////////////////
///////			모터 관련 정보들		////////////////////////
///////////////////////////////////////////////////////////
// 병진 & Twisting

// DSP value (X: 0 ~ 4095) to bipolar 10V DAC output (Vo: motor control input)
// Vo = aX + b

// 병진부 (a: 0.004898, b: -9.9971)
// Twsiting (a: 0.004874, b: -9.95)

// DSP to DAC parameters
float32 fa[2], fb[2];

// Voltage to current relationship
// 병진부: 10.86/10 = 1.086
// Twisting: 4.2/10 = 0.42
float32 fG_Ia[2];

// Torque constant
// 병진부: 29.2 [mNm/A]
// Twisting: 22.9 [mNm/A]
float32 fKt[2];


// 매크로 함수
#define X_TO_TORQUE0(DSP_X0)	((fKt[0])*(fG_Ia[0])*(((fa[0])*(DSP_X0))+(fb[0])))
#define X_TO_TORQUE1(DSP_X1)	((fKt[1])*(fG_Ia[1])*(((fa[1])*(DSP_X1))+(fb[1])))

#define TORQUE_TO_X0(TORQUE0)	(((TORQUE0)-((fKt[0])*(fG_Ia[0])*(fb[0])))/((fKt[0])*(fG_Ia[0])*(fa[0])))
//#define TORQUE_TO_X1(TORQUE1)	(((TORQUE1)-((fKt[1])*(fG_Ia[1])*(fb[1])))/((fKt[1])*(fG_Ia[1])*(fa[1])))
// 기준 방향을 맞추기 위해서 Twisting 토크 부호 바꾸기.
#define TORQUE_TO_X1(TORQUE1)	(((-1.0*(TORQUE1))-((fKt[1])*(fG_Ia[1])*(fb[1])))/((fKt[1])*(fG_Ia[1])*(fa[1])))

// 병진방향 Force => Torque 변환
#define TRANS_FOR_TO_TOR(FORCE0)		((FORCE0)*(TRANS_R))

// 가상벽 위치, 강성값
//float32 fVE_Pos, fVE_K;

// 병진방향 Force
float32 fTrans_Force;
float32 MasterTorqueTest;
Uint16 button1, button2;

// PI: 3.14159265359
//#define PI		3.1416
//#define PI		3.14159265359

// 병진부: 회전운동 => 병진운동 [mm]
// 와이어선 원통 지름: 13.1 [mm] = 2*r
// 와이어선 원통 반지름: 6.55 [mm]
// 병진 길이 [mm]: (2*PI*r / 360) * x [deg]

// 6.55 [mm]
//#define TRANS_R			6.55


// PI*2*r * DEG0 / 360
//#define TRANS_POS(DEG0)			((PI*13.1*(DEG0))/360.0)

// 실측치: r: 8.9406 (왼쪽과 오른쪽이 다름.)

// A circular helix: arc length L = sqrt(r^2 + (pitch/2*PI)^2) * 회전각
float32 fArcGain;// = sqrt((TRANS_R*TRANS_R) + 1/(4*PI*PI));

#define TRANS_POS(RAD0)			(fArcGain*(RAD0))


// Motor Torque
float32 fMotorTorque[2];

// Motor Enable flag
//bool bMotorEnable;
Uint16 bMotorEnable;

// Motor Run/Stop flag
//bool bMotorRun;
Uint16 bMotorRun;


// Friction Compensator
Uint16 bFrictionComp;

// Friction Comp. Force
float32 fFrictionCompForce;

// alpha0, alpha1, vs, alpha2
float32 fa0, fa1, fvs, fa2;



////////////////////////////////////////////////////////////////////////////////
////	Chirp signal Test for exciting translation direction	////////////////
/////////////////////////////////////////////////////////////////////////
// Linear Chirp signal
// A: Amplitude [mN]
// f1: Final Freq. [Hz]
// f0: Inital Freq. [Hz]
// k: is the rate of frequency increase over duration T.
// t1: time duration (final time)
// phi: phase offset
//float fA, fF0, fF1, fK, fT1, fPhi;


// Time variable
float fControl_Time;


// PD position controller
//a float fRef_Pos, fPos_Err, fP_Gain, fD_Gain, fI_Gain;
//a float fRef_Vel, fVel_Err, fpreYi, fYi;


// limit variables
float rLimit, lLimit;

//PID Loop to follow slave
float32 fMA_Error[2];
float32 fMA_AccError[2];
float32 fMA_PreError[2];
float32 fMA_ChangeError[2];
float32 fMA_PID_Kp[2];
float32 fMA_PID_Ki[2];
float32 fMA_PID_Kd[2];
float32 fMA_PID_OP[2];
float32 fSL_CurrentPos[2];
float32 fSL_UnderX;
/////////////////////////



Uint16 cMasterPosControl; //Can be T:True or F:False

Uint16 bMasterTwistPosCont; //Master Twist new commanded status, True or False
Uint16 ZeroTorque;

Uint16 bSoftwareFootButton;

// TEST
//a Uint32 uTest1, uTest2, uTest3;
//a float32 fTest1, fTest2;

Uint32 SlavePos[2];
float32 fSlavePos[2];

Uint16 NotRx_NewDataCount;

void main(void)

{

	//============================================================================================
	// Step 1. Disable Global Interrupt
	//			전역 인터럽트 비-활성화 및 인터럽트 플래그 초기화
	//--------------------------------------------------------------------------------------------
	// Step 1. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;

	// Disable CPU interrupts and clear all CPU interrupt flags
	IER = 0x0000;
	IFR = 0x0000;



// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xS_SysCtrl.c file.

	// LOSPCP = SYSCLKOUT/4 (default) = 200 M / 4 = 50.0 MHz

	InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the F2837xS_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// 칩의 모든 입출력 포트 상태 초기화(InitGpio)
	InitGpio();


	///////////////////////////////////////////////////////
	/////////////		LED Indicator //////////////////////
	////////////////////////////////////////////////////////
	// Note: GPIO34 (Red)는 제어 주기 확인도 겸함.

	// GPIO31번을 범용 입출력용으로 설정하고 CPU1에 데이터 레지스터 그룹 접근 권한을 부여
	// GPIO31번을 Push-Pull 회로에 의한 출력포트로 설정 (녹색 LED가 연결되어 있음)
	GPIO_SetupPinMux(31, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(31, GPIO_OUTPUT, GPIO_PUSHPULL);

	// GPIO34번을 범용 입출력용으로 설정하고 CPU1에 데이터 레지스터 그룹 접근 권한을 부여
	// GPIO34번을 Push-Pull 회로에 의한 출력포트로 설정 (적색 LED가 연결되어 있음)
//a	GPIO_SetupPinMux(34, GPIO_MUX_CPU1, 0);
//a	GPIO_SetupPinOptions(34, GPIO_OUTPUT, GPIO_PUSHPULL);


	//////////////////////////////////////////////////////////
	//////////// SCI-A (DSP <=> PC 통신) 설정
	/////////////////////////////////////////////////////////

	//  GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
	//  GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS
	// These functions are found in the F2837xS_Gpio.c file.
	GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);

	//////////////////////////////////////////////////////////
	//////////// SCI-B (DSP <=> DSP 통신) 설정
	/////////////////////////////////////////////////////////

	//  GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
	//  GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS
	// These functions are found in the F2837xS_Gpio.c file.
	GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 2);
	GPIO_SetupPinOptions(19, GPIO_INPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 2);
	GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_ASYNC);

	/////////////////////////////////////////////////////
	/////////		EQEP A & B 포트 설정		/////////////
	////////////////////////////////////////////////////


	GPIO_SetupPinMux(9, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(9, GPIO_INPUT, GPIO_QUAL6);

	GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_QUAL6);

	GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(42, GPIO_INPUT, GPIO_QUAL6);



	EALLOW;

	// for EQEP A

	// GPIO50번부터 53번까지 핀들에 대해 내부 Pull-Up 해제
	GpioCtrlRegs.GPBPUD.bit.GPIO50 = 1;		// Enable pull-up on GPIO50 (EQEP1A) //
	GpioCtrlRegs.GPBPUD.bit.GPIO51 = 1;		// Enable pull-up on GPIO51 (EQEP1B) //
	//GpioCtrlRegs.GPBPUD.bit.GPIO52 = 1;
	//GpioCtrlRegs.GPBPUD.bit.GPIO53 = 1;

/*
	// GPIO50번부터 53번까지의 핀들에 샘플링 윈도우 6의 노이즈 필터(Input Qualification) 활성화
	GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 2;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 2;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO52 = 2;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 2;

	// GPIO48번부터 55번까지에 적용되는 노이즈 필터 샘플링 주기 설정
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 255;	// 샘플링 주기 = SYSCLK/510
*/
	// No Input Qualification (SYSCLKOUT 에 동기화)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 0;		// Sync to SYSCLKOUT GPIO50 (EQEP1A)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 0;		// Sync to SYSCLKOUT GPIO51 (EQEP1B)

	// Specifies the qualification sampling period for GPIO48 to GPIO55 //
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 0x00;

	// GPIO50번부터 53번까지의 핀들을 EQEP-1 모듈 기능 핀으로 설정
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 1;	// EQEP1A
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 1;	// EQEP1B
	//GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 1;	// EQEP1S
	//GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 1;	// EQEP1I


	// for EQEP B
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;		// Enable pull-up on GPIO50 (EQEP1A) //
	GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;		// Enable pull-up on GPIO51 (EQEP1B) //

	GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0x00;	// Specifies the qualification sampling period for GPIO 24 to GPIO 31 //
	GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;		// Sync to SYSCLKOUT GPIO50 (EQEP1A)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;		// Sync to SYSCLKOUT GPIO51 (EQEP1B)

	// Select the pin function: (GPxMUXn registers 구성)
	// sprufb0d - 76 page 참고
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;	// Configure GPIO50 as EQEP1A //
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;	// Configure GPIO51 as EQEP1B //



	////////////////////////////////////////////
	////////	GPIO: Touch Switch 입력 (I)
	////////////////////////////////////////////
	// GPIO 32

	///////////////// Input Qualification
	// GPIO 32 ~ 63 // 현재까지는 32번만 해당
	// 3 - samples
	// 주기: 2 * {(2 * GPxCTRL[QUALPRDn]) * T_SYSCLKOUT}
	// 5 - samples
	// 최소 주기: 5 * {(2 * GPxCTRL[QUALPRDn]) * T_SYSCLKOUT}

	// (QUALPRD0: GPIO 32 ~ 39)  Qual period 설정
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 1;		// 2 * T_SYSCLKOUT (= PLLSYSCLK/2)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 2;		// Qualification using 6 samples

	EDIS;

	////////////////////////////////////////////
	////////	GPIO: Motor Enable 출력 (O)
	////////////////////////////////////////////
	// GPIO 33

	/////////////	Motor Enable /////////////////////
	// GPIO 33 (출력)
	//GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;		// 출력으로 설정
	//GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;	// Low 출력
	//MOTOR_OFF;

	// GPIO33번을 범용 입출력용으로 설정하고 CPU1에 데이터 레지스터 그룹 접근 권한을 부여
	// GPIO33번을 Push-Pull 회로에 의한 출력포트로 설정 (적색 LED가 연결되어 있음)
	GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(33, GPIO_OUTPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(34, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(34, GPIO_OUTPUT, GPIO_PUSHPULL);


	GPIO_SetupPinMux(69, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(69, GPIO_OUTPUT, GPIO_PUSHPULL);


	MOTOR_OFF;
	TWIST_MOTOR_OFF;


	// 단계 4. 주변회로 인터럽트 확장회로(PIE) 초기화 --------------------------------
	// PIE 회로 컨트롤 레지스터 초기화 / 모든 인터럽트 비-활성화(Disable)
	// 모든 표지(Flag) 레지스터 초기화(Clear)
	// 함수가 정의된 파일 이름 : F2837xS_PieCtrl.c

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xS_PieCtrl.c file.
	InitPieCtrl();


	// 단계 5. PIE 회로 인터럽트 벡터 초기화 -----------------------------------------
	// PIE 벡터 테이블 초기화 및 기본 인터럽트 서비스 루틴 연결
	// 주변회로 인터럽트 확장 회로의 모든 벡터들에 기본 서비스 루틴 함수 연결
	// 기본 인터럽트 서비스 루틴 함수들이 정의된 파일 이름 : F2837xD_DefaultISR.c
	// 함수가 정의된 파일 이름 : F2837xS_PieVect.c

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xS_DefaultIsr.c.
// This function is found in F2837xS_PieVect.c.
	InitPieVectTable();


	// Vector Remapping //

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;

	// This is needed to write to EALLOW protected registers
	PieVectTable.TIMER0_INT = &cpu_timer0_isr;

	// SCI-A 수신 인터럽트 함수 리매핑
	PieVectTable.SCIA_RX_INT = &sciaRxFifoIsr;
	PieVectTable.SCIB_RX_INT = &scibRxFifoIsr;

	EDIS;
	// This is needed to disable write to EALLOW protected registers


	//////////////////////////////////////////
	// 단계 8. 변수 초기화
	///////////////////////////////////////////////

	////////////////////////////////////////
		// SCI 통신 관련

		SCI_RxFlag = false;
		SCIB_RxFlag = false;

		SCI_cnt = 0;
		SCI_cnt1 = 0;

		// Save Data 송신 Index; (십만개에 리셋: 1 kHz 의 경우, 100초)
		fSCI_TxIndex = 0.0;


		// FIFO
		for(i=0;i<16;i++)
		{
			TxBuffA[i] = 0;
		}
		for(i=0;i<16;i++)
		{
			RxBuffA[i] = 0;
		}
		for(i=0;i<16;i++)
		{
			TxBuffB[i] = 0;
		}
		for(i=0;i<16;i++)
		{
			RxBuffB[i] = 0;
		}

		TxFifoLevel = 0;
		RxFifoLevel = 16;


		//////////////////////////////////////////////
		//////////		SCI 데이터 통신 관련
		for (i = 0; i < NUM_OF_BUF; i++)
		{
			tBuf[i] = 0;

			test_Buf[i] = 0;


			// TEST
			test_Rev_Buf[i] = 0;

		}
		for(i=0;i<NUM_OF_BUF;i++)
		{
			test_BufB[i] = 0;
		}
		for(i=0;i<NUM_OF_BUF;i++)
		{
			test_Rev_BufB[i] = 0;
		}


		for (i = 0; i < 8; i++)
			// 8개 송신할 data 임시 저장소
			fTempTxBuf[i] = 0.0;


		// 2번째 송신을 위한 flag 설정
		bNextTx = 0;

		for(i = 0; i<3; i++)
		{
			MasterForce[i] = 0;
			fMasterForce[i] = 0;
		}

		fMasterTorqueMagnitude = 0;
		fMasterTorqueDirection = 'x';
//a		ftest_Buf = 0.0;
//a		ftest_Buf1 = 0.1;

		//ftest_Buf2 = 0.12;
		//ftest_Buf3 = 0.123;

//a		ftest_Buf2 = 0.123;
//a		ftest_Buf3 = 777.777;


		cMasterPosControl = 'F'; //Can be T:True or F:False
		bMasterTwistPosCont = false;
		ZeroTorque = 3;
		bSoftwareFootButton = false;


		//PID Loop
		for(i = 0; i<2; i++)
		{
			fMA_Error[i] = 0.0;
			fMA_AccError[i] = 0.0;
			fMA_ChangeError[i] = 0.0;
			fMA_PreError[i] = 0.0;
			fSL_CurrentPos[i] = 0.0;
			fMA_PID_OP[i] = 0.0;
		}
		fSL_UnderX = 0.0;

		fMA_PID_Kp[0] = 0.2;
		fMA_PID_Ki[0] = 0.02;
		fMA_PID_Kd[0] = 0.001;

		fMA_PID_Kp[1] = 6.0;
		fMA_PID_Ki[1] = 0.1;
		fMA_PID_Kd[1] = 0.1;




		Tx_cnt = 0;

fPushCalib = -0.01;
fPullCalib = 0.19;

SlavePos[0] = 0;
SlavePos[1] = 0;

fSlavePos[0] = 0;
fSlavePos[1] = 0;
NotRx_NewDataCount = 0;



bStartMotorControl = false;
bStopMotorControl = false;
	////////////////////////////////////////
	////		EQEP 관련
	////////////////////////////////////////////
	// EQEP
	PositionMax[0] = ENCODER_REV1 * 4;
	PositionMax[1] = ENCODER_REV2 * 4;

	for (i = 0; i < 2; i++)
	{
		PositionCounter[i] = 0;
		RotateDirection[i] = 0;
		RotateAngleUnit[i] = 0;
		RotateAngleDegree[i] = 0;
		RotateAngleRad[i] = 0.0;	// EQEP-1 모듈로 해석해낸 현재 회전각도(Rad)

		NumOfTurns[i] = 0;
		fCurrentPos[i] = 0.0;

		fCurrentPos_ToSend[i] = 0.0;
		fPos_JustBeforeButton[i] = 0;
		fMoveDuringButton[i] = 0;

		fCurrentVel[i] = 0.0;
		fCurrentAccel[i] = 0.0;

		fprePos[i] = 0.0;
		fpreVel[i] = 0.0;

	}

	fTransRad = 0.0;

	////////////////////////////////
	//// 터치 스위치
	////////////////////
	bInput_TouchSW = false;


	//////////////////////////////////////////////////////////
	///////			모터 관련 정보들		////////////////////////
	///////////////////////////////////////////////////////////
	// 병진 & Twisting

	// DSP value (X: 0 ~ 4095) to bipolar 10V DAC output (Vo: motor control input)
	// Vo = aX + b

	// DSP to DAC parameters
	// 병진부 (a: 0.004898, b: -9.9971)
	fa[0] = 0.004914;
	fb[0] = -10.01;
	fa[1] = 0.004914;
	fb[1] = -10.01;

	// Voltage to current relationship
	// 병진부: 10.86/10 = 1.086
	fG_Ia[0] = 1.086;

	// Twisting: 4.2/10 = 0.42
	//fG_Ia[1] = 0.42;
	fG_Ia[1] = 0.14;

	// Torque constant
	// 병진부: 29.2 [mNm/A]
	fKt[0] = 29.2;	// [mNm/A]

	// Twisting: 22.9 [mNm/A]
	fKt[1] = 22.9;		// [mNm/A]


	// Motor Enable: Disabled
	//bMotorEnable = false;
	bMotorEnable = 0;

	// Motor Run/Stop flag
	bMotorRun = false;

	bFrictionComp = false;

	fFrictionCompForce = 0.0;


	// circular helix 아크 길이 게인
	fArcGain = sqrt((TRANS_R*TRANS_R) + 1/(4*PI*PI));


	//// Chirp signal variables initialize
	//fA = 1.0;
	//fA = 7.0;

	// 위치 제어용
	//fA = 4.0;
//a	fA = 3.0;



//a	fF0 = 0.0;
	//fF1 = 20.0;
	//fF1 = 5.0;

//a	fF1 = 8.0;


	//fF1 = 10.0;


	//fT1 = 1800.0;	// [sec], 30분

	//fT1 = 60.0;	// [sec], 30분

//a	fT1 = 300.0;	// [sec], 5분

//a	fPhi = PI/2;
//a	fK = (fF1 - fF0) / fT1;


	// Time variable
	fControl_Time = 0.0;


	// PID Friction measure: Velocity controller
//a	fRef_Pos = 0.0;
//a	fRef_Vel = 0.0;

//a	fP_Gain = 0.33;
//a	fI_Gain = 0.003;
//a	fD_Gain = 0.000003;


//a	fPos_Err = 0.0;
//a	fVel_Err = 0.0;

	// I gain parameter
//a	fYi = 0.0;
//a	fpreYi = 0.0;



	// limit variables
	rLimit = 30.0;
	lLimit = -30.0;

	bHandShake = false;
	bSwitch = false;

	////////////////////////////////////////
	/////////		DAC (TLV5638) 관련
	///////////////////////////////////////

	// 현재 50 MHz default clock 사용 (SPI 통신 초기화 및 DAC 칩 설정)
	DAC.init(&DAC);

	BackTicker = 0;

	// zero torque로 초기화
	// ideal condition: 2048 = 0 V를 의미함.
	// ideal: a = 10/2048, b = -10
	// X = (Vo - b) / a = 10 / (10/2048) = 2048

	TestDataA = ROUND(TORQUE_TO_X0(0.0));		// actual: 2037
	TestDataB = ROUND(TORQUE_TO_X1(0.0));		// actual: 2037


	// SPI 통신으로 motor torque 전송 (0.0 토크 전송)
	DAC.update(&DAC);



	// TEST
//	uTest1 = ROUND(1.4);
//	uTest2 = ROUND(2.6);
//	uTest3 = ROUND(3.9);

	//fTest1 = PI;

	fMotorTorque[0] = 0.0;
	fMotorTorque[1] = 0.0;

button1 =0;
button2 = 0;
MasterTorqueTest = 0;
	/////////////////////////////////
	///		가상벽 Test
	// 가상벽 위치
//a	fVE_Pos = 3.0;

//a	fVE_K = 1.0;

	// 병진방향 Force
	fTrans_Force = 0.0;

	///////////////////////////////////////////


	// Step 4. Initialize the Device Peripheral. This function can be
	//         found in F2806x_CpuTimers.c
	InitCpuTimers();   // For this example, only initialize the Cpu Timers


	// 4 kHz => 0.25 ms = 250 us
	Timer_Prd = CONTROL_RATE;		// 타이머 주기 설정 (usec 단위)


	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// 200MHz CPU Freq, 0.25 ms (4 kHz) Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, 200, Timer_Prd);


	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in F2837xS_cputimervars.h), the
	// below settings must also be updated.
	// Start Timer!
	StartCpuTimer0();
	//CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0


	//////////////////////////////////////////////////////////
	//////////		SCI - A       //////////////////////////
	////////////////////////////////////////////////////////////
	scia_fifo_init();  // Init SCI-A
	scib_fifo_init(); // Init SCI-B


	/////////////////////////////////////////////////////////////
	////////		EQEP		/////////////////////////////////

	/* Initialize Enhanced QEP Module 1 */
	InitEQep1(PositionMax[0]);

	/* Initialize Enhanced QEP Module 2 */
	InitEQep2(PositionMax[1]);



	//////////////////////////////////////////////////////////
	// Step 5. User specific code, enable interrupts:


	// CPU Timer0 인터럽트 활성화
	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	// which is connected to CPU-Timer 1, and CPU int 14, which is connected
	// to CPU-Timer 2:
	IER |= M_INT1;


	/////////////////////////////////////
	///////		SCI-A
	///////////////////////////////////////
	// SCIA_RX interrupt
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;		// PIE 인터럽트(SCIRXINTA) : Enable (INT9.1)
	IER |= M_INT9;
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
	IER |= M_INT9;




	//============================================================================================
	// Enable global Interrupts and higher priority real-time debug events:
	//--------------------------------------------------------------------------------------------

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM


	//============================================================================================
	// IDLE loop. Just sit and loop forever :
	//--------------------------------------------------------------------------------------------

// Step 6. IDLE loop. Just sit and loop forever (optional):
	while (1) {
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////						 제어 쓰레드			//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


__interrupt void cpu_timer0_isr(void) {

	bStartMotorControl = START_MOTOR_CONTROL;
	bStopMotorControl = STOP_MOTOR_CONTROL;
	if(bStartMotorControl)
	{
		bFrictionComp = 1;
		bMotorEnable = 1;
		bMotorRun = 1;
	}
	if(bStopMotorControl || NotRx_NewDataCount >= 400)
	{
		bFrictionComp = 0;
		bMotorEnable = 0;
		bMotorRun = 0;
	}

	LOOPRATE;
	bSwitch = BUTTON;

	if (SCIB_RxFlag == true)
	{
		for(i=0;i<RxFifoLevel;i++)
		{
			test_Rev_BufB[i] = RxBuffB[i] & 0x000000FF;
		}
		SCIB_RxFlag = false;
	}

	if(test_Rev_BufB[0] == 'H')
		if(test_Rev_BufB[1] == 'S')
			if(test_Rev_BufB[2] == 'T')
			{
				bHandShake = true;
				GREEN_LED_ON;
				while(SciaRegs.SCIFFTX.bit.TXFFST != 0);
				{
					SciaRegs.SCITXBUF.all = 'A';
					SciaRegs.SCITXBUF.all = 'L';
					SciaRegs.SCITXBUF.all = 'L';
					SciaRegs.SCITXBUF.all = 'R';
					SciaRegs.SCITXBUF.all = 'E';
					SciaRegs.SCITXBUF.all = 'A';
					SciaRegs.SCITXBUF.all = 'D';
					SciaRegs.SCITXBUF.all = 'Y';
				}
				for(i=8;i<RxFifoLevel;i++)
				{
					SciaRegs.SCITXBUF.all = '*';
				}
				button2 = 1;
			}

	fControl_Time += 0.00025;

	if(button2){


	// Motor Enable GPIO 33
	if (bMotorEnable)
	{
		MOTOR_ON;
		TWIST_MOTOR_ON;
	}
	else
	{
		MOTOR_OFF;
		TWIST_MOTOR_OFF;
	}


	//////////////////////////////////////////////////////////////////
	/////		Receive data or command from PC.
	//////////////////////////////////////////////////////////////////

	if (SCI_RxFlag == true)
	{
		for(i=0;i<RxFifoLevel;i++)
		{
			test_Rev_Buf[i] = RxBuffA[i];
		}
		SCI_RxFlag = false;
	}




	if(test_Rev_BufB[0] == 'a' && test_Rev_BufB[4] == 'b') //Extracting Linear Motion Position
	{
		if( (test_Rev_BufB[1] & 0x00000080) == 0x00000080)
		{
			SlavePos[0] = 0xFF000000;
			SlavePos[0] |= (test_Rev_BufB[1] << 16);
			SlavePos[0] |= (test_Rev_BufB[2] << 8);
			SlavePos[0] |= (test_Rev_BufB[3]);
		}
		else
		{
			SlavePos[0] = 0;
			SlavePos[0] = (test_Rev_BufB[1] << 16);
			SlavePos[0] |= (test_Rev_BufB[2] << 8);
			SlavePos[0] |= (test_Rev_BufB[3]);
		}
		if( (test_Rev_BufB[5] & 0x00000080) == 0x00000080)
		{
			SlavePos[1] = 0xFF000000;
			SlavePos[1] |= (test_Rev_BufB[5] << 16);
			SlavePos[1] |= (test_Rev_BufB[6] << 8);
			SlavePos[1] |= (test_Rev_BufB[7]);
		}
		else
		{
			SlavePos[1] = 0;
			SlavePos[1] = (test_Rev_BufB[5] << 16);
			SlavePos[1] |= (test_Rev_BufB[6] << 8);
			SlavePos[1] |= (test_Rev_BufB[7]);
		}
	}

	fSlavePos[0] = ((float)(long)SlavePos[0]) * 0.001 * 10;
	fSlavePos[1] = ((float)(long)SlavePos[1]) * 0.001 * 10;



	//////////////////////////////////////////////////////////////
	/////////	Read Sensors
	//////////////////////////////////////////////////////////////

/*
	// Obtain Touch SW input (TEST) - GPIO 32번
	bInput_TouchSW = GpioDataRegs.GPBDAT.bit.GPIO32;

	if (bInput_TouchSW)
		GREEN_LED_ON;
	else
		GREEN_LED_OFF;
*/



	///////////////////////////////////////////////////////
	/////		EQEP 1, 2
	//////////////////////////////////////////////////////////
	///////////////////

	// EQEP 1
	// Check Position & Direction //
	// EQEP-1 모듈 내장 위치 카운터(Position Counter)의 현재값을 'PositionCounter' 변수에 저장
	PositionCounter[0] = EQep1Regs.QPOSCNT;
	// EQEP-1 모듈의 Quadrature Decoder Unit이 알아낸 회전방향을 'RotateDirection' 변수에 저장
	RotateDirection[0] = EQep1Regs.QEPSTS.bit.QDF;
	// 현재의 회전각도를 비율로 환산 (0.0 ~ 1.0)
	RotateAngleUnit[0] = (float32)PositionCounter[0] / PositionMax[0];
	// 현재의 회전각도 산출(Degree)
	RotateAngleDegree[0] = RotateAngleUnit[0] * 360;
	// 현재의 회전각도 산출 (Rad)
	//RotateAngleRad[0] = RotateAngleUnit[0] * 2*PI;	// EQEP-1 모듈로 해석해낸 현재 회전각도(Rad)

	// Overflow check!   In this case, add + 360 degree
	// Underflow check!  In this case, add - 360 degree
	if (EQep1Regs.QFLG.bit.PCO)		// Positive direction revolution
	{
		// Clear Overflow interrupt flag
		EQep1Regs.QCLR.bit.PCO = 1;

		NumOfTurns[0]++;
	}
	else if (EQep1Regs.QFLG.bit.PCU)	// Negative direction revolution
	{
		// Clear Underflow interrupt flag
		EQep1Regs.QCLR.bit.PCU = 1;

		NumOfTurns[0]--;
	}

	fCurrentPos[0] = -(RotateAngleUnit[0] + (float)NumOfTurns[0])*2*PI;

	fTransRad = fCurrentPos[0];

	fCurrentPos[0] = TRANS_POS(fCurrentPos[0]);

	// EQEP
	// Check Position & Direction
	PositionCounter[1] = EQep2Regs.QPOSCNT;
	RotateDirection[1] = EQep2Regs.QEPSTS.bit.QDF;
	RotateAngleUnit[1] = (float32)PositionCounter[1] / PositionMax[1];
	RotateAngleDegree[1] = RotateAngleUnit[1] * 360;
	//RotateAngleRad[1] = RotateAngleUnit[1] * 2*PI;

	// Overflow check!   In this case, add + 360 degree
	// Underflow check!  In this case, add - 360 degree
	if (EQep2Regs.QFLG.bit.PCO)		// Positive direction revolution
	{
		// Clear Overflow interrupt flag
		EQep2Regs.QCLR.bit.PCO = 1;

		NumOfTurns[1]++;
	}
	else if (EQep2Regs.QFLG.bit.PCU)	// Negative direction revolution
	{
		// Clear Underflow interrupt flag
		EQep2Regs.QCLR.bit.PCU = 1;

		NumOfTurns[1]--;
	}

	// 최종 현재 위치 계산 (Deg)
	//fCurrentPos[1] = RotateAngleDegree[1] + 360.0*NumOfTurns[1];
	// rad
	fCurrentPos[1] = (RotateAngleUnit[1] + (float)NumOfTurns[1])*2*PI;

	//if(bSwitch)
	if(bSoftwareFootButton)
	{
		fCurrentPos_ToSend[0] = fPos_JustBeforeButton[0];
		fMoveDuringButton[0] = fCurrentPos[0] - fPos_JustBeforeButton[0];


		fCurrentPos_ToSend[1] = fPos_JustBeforeButton[1];
		fMoveDuringButton[1] = fCurrentPos[1] - fPos_JustBeforeButton[1];

	}
	else
	{
		fCurrentPos_ToSend[0] = fCurrentPos[0] - fMoveDuringButton[0];
		fPos_JustBeforeButton[0] = fCurrentPos_ToSend[0];

		fCurrentPos_ToSend[1] = fCurrentPos[1] - fMoveDuringButton[1];
		fPos_JustBeforeButton[1] = fCurrentPos_ToSend[1];
	}


	// 속도 및 가속도 계산 (Backward 방식) (변위 * fs)
	fCurrentVel[0] = (fCurrentPos[0] - fprePos[0])*CONTROL_HZ;		// mm/s
	fCurrentVel[1] = (fCurrentPos[1] - fprePos[1])*CONTROL_HZ;		// rad/s

	fCurrentAccel[0] = (fCurrentVel[0] - fpreVel[0])*CONTROL_HZ;		// mm/(s^2)
	fCurrentAccel[1] = (fCurrentVel[1] - fpreVel[1])*CONTROL_HZ;		// rad/(s^2)


	///////////////////////////////////////////////////////////////
	/////////			DAC (TLV5638): 계산된 제어기 출력 (토크)을 모터 토크 제어기에 제공
	///////////////////////////////////////////////////////////////


	if (bFrictionComp)
	{
		if (fCurrentVel[0] > 0.0)
		{
			fa0 = 0.9465;
			fa1 = 0.1237;
			fvs = 6;//Originally: 14.6047;
			fa2 = 0.0022;
			fFrictionCompForce = (fa0 + fa1*exp(-pow((fCurrentVel[0]/fvs), 2))) * (SIGN(fCurrentVel[0])) + fa2*fCurrentVel[0] + fPushCalib;
			if(fFrictionCompForce < 0)
				fFrictionCompForce = 0;

		}
//		else
		if (fCurrentVel[0] < 0.0)
		{
			fa0 = 0.8810;
			fa1 = 0.1242;
			fvs = 9;//Originally: 49.999;
			fa2 = 0.0015;
			fFrictionCompForce = (fa0 + fa1*exp(-pow((fCurrentVel[0]/fvs), 2))) * (SIGN(fCurrentVel[0])) + fa2*fCurrentVel[0] + fPullCalib;
			if(fFrictionCompForce > 0)
				fFrictionCompForce = 0;
		}
		if (fCurrentVel[0] == 0.0)
			fFrictionCompForce = 0;
	//	fFrictionCompForce = (fa0 + fa1*exp(-pow((fCurrentVel[0]/fvs), 2))) * (SIGN(fCurrentVel[0])) + fa2*fCurrentVel[0];

	}
	else
		fFrictionCompForce = 0.0;



	///////////////// Safety //////////////////////////////
	// Push: 38.7 mm  => 약 30 mm
	// Pull: -20 mm
	//if (fCurrentPos[0] > 4.5 || fCurrentPos[0] < -4.5)


	if (fCurrentPos[0] > rLimit || fCurrentPos[0] < lLimit) //This is for safety purposes, commenting for the time being
	{
	//	fMotorTorque[0] = 0.0;
	//	fMotorTorque[1] = 0.0;
	//	bMotorRun = 0;
	//	fTrans_Force = 0.0;
//a		fYi = 0.0;
//a		fpreYi = 0.0;
	//	fFrictionCompForce = 0.0;
	//	fMasterForce[0] = 0.0;
	}

	if (bMotorRun)
	{
		TWIST_MOTOR_OFF;
		if(ZeroTorque == 1)
			TWIST_MOTOR_ON;


		fTrans_Force = /*fFrictionCompForce + */fMasterForce[0];

		fMotorTorque[0] = TRANS_FOR_TO_TOR(fTrans_Force);
		fMotorTorque[1] = fMasterForce[1];

//		fTrans_Force = fTrans_Force - fFrictionCompForce;

		TestDataA = (Uint16)(ROUND(TORQUE_TO_X0(fMotorTorque[0])));
		TestDataB = (Uint16)(ROUND(TORQUE_TO_X1(fMotorTorque[1])));
		DAC.DataRegisterA = TestDataA;
		DAC.DataRegisterB = TestDataB;
		DAC.update(&DAC);



		bMasterTwistPosCont = false;

/////////////

	}
	else
	{

		// Time variable (리셋)
		fControl_Time = 0.0;

		// 초기화
//a		fVel_Err = 0.0;
		fTrans_Force = 0.0;
//a		fYi = 0.0;
//a		fpreYi = 0.0;

		fMotorTorque[0] = 0.0;
		fMotorTorque[1] = 0.0;

		TestDataA = ROUND(TORQUE_TO_X0(fMotorTorque[0]));		// actual: 2037
		TestDataB = ROUND(TORQUE_TO_X1(fMotorTorque[1]));		// actual: 2037


		DAC.DataRegisterA = TestDataA;
		DAC.DataRegisterB = TestDataB;
		DAC.update(&DAC);
	}

	// SCI-A 통신 주기용 카운터
	++SCI_cnt;
	++SCI_cnt1;

	// Save Data 송신 Index; (십만개에 리셋: 1 kHz 의 경우, 100초)
	fSCI_TxIndex += 0.001;


	if (!(SCI_cnt % 4))
	{
		// Reset 카운터
		SCI_cnt = 0;

		// 2번째 송신을 위한 flag 설정: On
		bNextTx = 1;


			///////////////////////////////////////////////
			////	SCI 통신: 송신 부분 ////////////////////////
			/////////////////////////////////////////////////////////
			//// 한번 송신할 수 있는 data 갯수: 4 개

			////	전송할 Data

			/////// float 형
			/// a. (97)  Pos 0
			/// b. (98)  Pos 1
			/// c. (99)  Vel 0
			/// d. (100) Vel 1


			//////////////////////////////////////////////
			/////	SCI 통신:  data 형변환:  => 8 bit Uint8 로 변환

//a			++ftest_Buf2;

			// 제한값: 8388.608
//a			if (ftest_Buf2 > 8001)
//a				ftest_Buf2 = 0.123;

			//++ftest_Buf3;

			// 소수점 3자리까지만 고려
			// long: 32 bit
			// 24 bit 까지만 고려. 보다 컴팩트화를 위해 (32비트 전체 쓰는 것도 고려해볼 예정)

			// MSB (부호 비트)는 빼고 고려
			// 2^23 = 8388608
			// 소수점에 해당하는 3자리 빼면,
			// 8388.608 이  통신 최대 제한선이 된다고 판단됨. (체크 요!) (음수의 경우)

			// (양수의 경우) 2^24 = 16777216 이 최대 제한값이 됨.
			// 1000 을 나누면, 16777.216 이 실제 DSP 내부에서 고려하는 양수 최대값

			// 결국 가장 작은 수를 고려하면, 8388.608  이  음수, 양수 최대 제한값임. (DSP 내부에서 최종으로 쓸 값.  24bit만 전송하므로!!!)


			///////////////////////////////////////////////////////
			// a. fCurrentPos[0]
			// b. fCurrentPos[1]


			// 8개의 data를 송신 임시 저장소에 저장한다. (다음번 전송을 위해서.
			// (4~7)의 나중 4개만 저장해 놓자.
			fTempTxBuf[0] = fSCI_TxIndex;
			fTempTxBuf[1] = fCurrentPos[0]*0.1;
			fTempTxBuf[2] = fCurrentVel[0]*0.01;
			fTempTxBuf[3] = fCurrentAccel[0]*0.00001;

			//fTempTxBuf[4] = fMotorTorque[0];
			fTempTxBuf[4] = fTrans_Force*0.1;			// 병진 Force
			fTempTxBuf[5] = fCurrentPos[1]*0.1;
			fTempTxBuf[6] = fCurrentVel[1]*0.01;
//a			fTempTxBuf[7] = fVel_Err*0.1;		// overflow 방지를 위해서 스케일 다운 (0.1)


		test_Buf[0] = (long)(1000 * fTempTxBuf[0]);

		test_Buf[1] = (long)(1000 * fTempTxBuf[1]);
			//test_Buf[0] = (long)(1000 * ftest_Buf);

		test_Buf[2] = (long)(1000 * fTempTxBuf[2]);

		test_Buf[3] = (long)(1000 * fTempTxBuf[3]);

	//		ftest_Buf = 425.1234;
		//test_Buf[3] = (long)(1000 * ftest_Buf2);


		fTempTxBuf[0] = fCurrentPos[0]*0.1;
		fTempTxBuf[1] = fCurrentPos[1]*0.1;
		fTempTxBuf[2] = fSlavePos[0]*0.1;
		fTempTxBuf[3] = fSlavePos[1]*0.1;

		test_Buf[0] = (long)(1000 * fTempTxBuf[0]);
		test_Buf[1] = (long)(1000 * fTempTxBuf[1]);
		test_Buf[2] = (long)(1000 * fTempTxBuf[2]);
		test_Buf[3] = (long)(1000 * fTempTxBuf[3]);



		//Preparing data to send to PC via SCI-A
		//MasterPos0 - Translation
		TxBuffA[0] = 97;
		TxBuffA[1] = (test_Buf[0] >> 16) & 0x00FF;		// 23 ~ 16 bit 값 저장
		TxBuffA[2] = (test_Buf[0] >> 8) & 0x00FF;		// 15 ~ 8 bit 값 저장
		TxBuffA[3] = test_Buf[0] & 0x00FF;				// 7 ~ 0 bit (하위 8 bit) 값 저장

		//MasterPos1 - Twisting
		TxBuffA[4] = 98;
		TxBuffA[5] = (test_Buf[1] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffA[6] = (test_Buf[1] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffA[7] = test_Buf[1] & 0x00FF;				// 하위 16 bit 값 저장

		//SlavePos0 - Translation
		TxBuffA[8] = 99;
		TxBuffA[9] = (test_Buf[2] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffA[10] = (test_Buf[2] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffA[11] = test_Buf[2] & 0x00FF;				// 하위 16 bit 값 저장

		//SlavePos1 - Twisting
		TxBuffA[12] = 100;
		TxBuffA[13] = (test_Buf[3] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffA[14] = (test_Buf[3] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffA[15] = test_Buf[3] & 0x00FF;				// 하위 16 bit 값 저장




		//Preparing data to send to Slave via SCI-B
		fTempTxBuf[1] = fCurrentPos_ToSend[0]*0.1;
		fTempTxBuf[5] = fCurrentPos_ToSend[1]*0.1;
		// Sending only the position information of Linear and Rotary Motion
		test_BufB[0] = (long)(1000 * fTempTxBuf[1]);
		test_BufB[1] = (long)(1000 * fTempTxBuf[5]);
		// Translation Motion Position
		TxBuffB[0] = 'a';
		TxBuffB[1] = (test_BufB[0] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffB[2] = (test_BufB[0] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffB[3] = test_BufB[0] & 0x00FF;
		// Rotation Motion Position
		TxBuffB[4] = 'b';
		TxBuffB[5] = (test_BufB[1] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffB[6] = (test_BufB[1] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
		TxBuffB[7] = test_BufB[1] & 0x00FF;				// 하위 16 bit 값 저장



		//---------------------------------------------------------------------------
		//		A. SCI 데이터 송신
		//	---------------------------------------------------------------------------//
		// Trnasmit FIFO가 empty 가 될때까지, 대기!
		while(SciaRegs.SCIFFTX.bit.TXFFST != 0);

		// Transmit FIFO 가 empty 이면, 송신을 수행한다.
		for(i=0;i<RxFifoLevel;i++)
		{
			//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
			SciaRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;
		}

	if(bHandShake)
	{
		//---------------------------------------------------------------------------
		//		B. SCI 데이터 송신
		//	---------------------------------------------------------------------------//
		// Trnasmit FIFO가 empty 가 될때까지, 대기!
		while(ScibRegs.SCIFFTX.bit.TXFFST != 0);

		// Transmit FIFO 가 empty 이면, 송신을 수행한다.
		for(i=0;i<RxFifoLevel;i++)
		{
			//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
			ScibRegs.SCITXBUF.all = TxBuffB[i] & 0x00FF;
		}
///////////////////////////////////////////////////////////////////////////////////////////
	}
		// Save Data 송신 Index; (8000개에 리셋: 1 kHz 의 경우, 8초)
		if (fSCI_TxIndex >= 8000.0)
			fSCI_TxIndex = 0.0;



	// SCI 통신 if 문 End
	///////////////////////////////////////////////////////
	}


////////////////////////////////////////////////////////////////////////////
	/////////////////			2번째 Data (16개)를 다음 주기에 연속해서 보내기		//////////////
	/////////////////////////////////////////////////////////////////////////////////////

	// 1 kHz로 보내기. 즉, 1초에 1000번 보내기 (현재 4kHz 제어 주기 => 1초에 4천번)
	// 921600 bps로는 4kHz 유지됨. (Good)
	// 1 kHz로 절반을 떨구자.
	else if ((SCI_cnt == 2) && bNextTx)
	{

			// Reset 카운터
			//SCI_cnt1 = 0;

			// 2번째 송신을 위한 flag 설정: On
			bNextTx = 0;



			test_Buf[0] = (long)(1000 * fTempTxBuf[4]);

			test_Buf[1] = (long)(1000 * fTempTxBuf[5]);
			//test_Buf[1] = (long)(1000 * 0.0);
						//test_Buf[0] = (long)(1000 * ftest_Buf);

			test_Buf[2] = (long)(1000 * fTempTxBuf[6]);
			//test_Buf[2] = (long)(1000 * 0.0);

			test_Buf[3] = (long)(1000 * fTempTxBuf[7]);
			//																										31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
			//																										|----------------------|-----------------------|-------------------|----------------|

			TxBuffA[0] = 101;
	//		TxBuffA[1] = (test_Buf[0] >> 16) & 0x00FF;		// 23 ~ 16 bit 값 저장
	//		TxBuffA[2] = (test_Buf[0] >> 8) & 0x00FF;		// 15 ~ 8 bit 값 저장
	//		TxBuffA[3] = test_Buf[0] & 0x00FF;				// 7 ~ 0 bit (하위 8 bit) 값 저장
			TxBuffA[1] = bSwitch;
			TxBuffA[2] = 0;
			TxBuffA[3] = 0;

			TxBuffA[4] = 102;
			TxBuffA[5] = (test_Buf[1] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
			TxBuffA[6] = (test_Buf[1] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
			TxBuffA[7] = test_Buf[1] & 0x00FF;				// 하위 16 bit 값 저장

			TxBuffA[8] = 103;
			TxBuffA[9] = (test_Buf[2] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
			TxBuffA[10] = (test_Buf[2] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
			TxBuffA[11] = test_Buf[2] & 0x00FF;				// 하위 16 bit 값 저장

			TxBuffA[12] = 104;
			TxBuffA[13] = (test_Buf[3] >> 16) & 0x00FF;		// 상위 16 bit 값 저장
			TxBuffA[14] = (test_Buf[3] >> 8) & 0x00FF;		// 상위 16 bit 값 저장
			TxBuffA[15] = test_Buf[3] & 0x00FF;				// 하위 16 bit 값 저장


			//---------------------------------------------------------------------------
			//		A. SCI 데이터 송신
			//	---------------------------------------------------------------------------//
			// Trnasmit FIFO가 empty 가 될때까지, 대기!
			while(SciaRegs.SCIFFTX.bit.TXFFST != 0);

			// Transmit FIFO 가 empty 이면, 송신을 수행한다.
			for(i=0;i<RxFifoLevel;i++)
			{
				//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
				SciaRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;
			}
/*
			//---------------------------------------------------------------------------
			//		B. SCI 데이터 송신
			//	---------------------------------------------------------------------------//
			// Trnasmit FIFO가 empty 가 될때까지, 대기!
			while(ScibRegs.SCIFFTX.bit.TXFFST != 0);

			// Transmit FIFO 가 empty 이면, 송신을 수행한다.
			for(i=0;i<RxFifoLevel;i++)
			{
				//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
				ScibRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;
			}
*/

		// SCI 통신 if 문 End
///////////////////////////////////////////////////////
		}

///////////////////////////////////


	////////////	정보 갱신


	// 속도 및 가속도 계산 (Backward 방식) (변위 * fs)
	fpreVel[0] = fCurrentVel[0];
	fpreVel[1] = fCurrentVel[1];

	fprePos[0] = fCurrentPos[0];
	fprePos[1] = fCurrentPos[1];
	}
	///////////////////////////////////////////////////////////////////////////
	// Acknowledge this interrupt to receive more interrupts from group 1
	NotRx_NewDataCount++;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


//===========================================================================
// No more.
//===========================================================================





/*-----------------------------------------------------------------------------
	Step 10
	10.1 Local Interrupt Service Routines & Functions
-----------------------------------------------------------------------------*/
void InitEQep1(Uint32 PositionMax1)
{
	// QEP Decoder Control (QDECCTL) Register
	// 00: Quadrature count mode
	EQep1Regs.QDECCTL.bit.QSRC = 0;	// EQEP-1 모듈의 Quadrature Decoder Unit을 Quadrature count mode로 설정

	// eQEP Control (QEPCTL) Register
	// 1x: Position counter is unaffected by emulation suspend
	// EQEP-1 모듈 내장 카운터들에 대한 에뮬레이션 모드 설정 : (2h) 에뮬레이션 중지(Suspend) 명령이 내장 카운터들의 동작에 영향을 주지 않음
	EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;

	// Position Counter Reset on Maximum Position (QEPCTL[PCRM]=01)
	// 정방향: QPOSMAX => 0 (with overflow flag = 1)
	// 역방향: (with underflow flag = 1) QPOSMAX <= 0
	// EQEP-1 모듈 내장 위치 카운터(Position Counter)가 최대값(QPOSMAX)에 이르면 리셋(Reset) 되도록 함
	EQep1Regs.QEPCTL.bit.PCRM = 1;			/* Position Counter Reset on Maximum Position */
	// EQEP-1 모듈 내장 위치 카운터의 최대값 설정(엔코더 rev. x 4 QCLK @ 1 revolution)
	EQep1Regs.QPOSMAX = PositionMax1 - 1;	/* 24 x 4 QCLK @ 1 revolution */

	// QPEN: 1: eQEP position counter is enabled
	// EQEP-1 모듈 내장 위치 카운터 활성화(Enable)
	EQep1Regs.QEPCTL.bit.QPEN = 1; 			/* QEP enable */
}


/*-----------------------------------------------------------------------------
	Step 10
	10.1 Local Interrupt Service Routines & Functions
-----------------------------------------------------------------------------*/
void InitEQep2(Uint32 PositionMax2)
{
	// QEP Decoder Control (QDECCTL) Register
	// 00: Quadrature count mode
	EQep2Regs.QDECCTL.bit.QSRC = 0;			/* Quadrature count mode */

	// eQEP Control (QEPCTL) Register
	// 1x: Position counter is unaffected by emulation suspend
	EQep2Regs.QEPCTL.bit.FREE_SOFT = 2;

	// Position Counter Reset on Maximum Position (QEPCTL[PCRM]=01)
	// 정방향: QPOSMAX => 0 (with overflow flag = 1)
	// 역방향: (with underflow flag = 1) QPOSMAX <= 0
	EQep2Regs.QEPCTL.bit.PCRM = 1;			/* Position Counter Reset on Maximum Position */
	EQep2Regs.QPOSMAX = PositionMax2 - 1;	/* 24 x 4 QCLK @ 1 revolution */

	// QPEN: 1: eQEP position counter is enabled
	EQep2Regs.QEPCTL.bit.QPEN = 1; 			/* QEP enable */
}

void scib_fifo_init()
{
   ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                  // No parity,8 char bits,
                                  // async mode, idle-line protocol
   ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                  // Disable RX ERR, SLEEP, TXWAKE
   //SciaRegs.SCICTL2.bit.TXINTENA =1;
   //SciaRegs.SCICTL2.bit.RXBKINTENA =1;

//////////////////// 100 MHz 로 클럭 변경함 /////////////////////////////////
   // LOSCLK (LOSPCP) = 200/2 = 100 MHz 로 고려해서 Baud Rate 계산.
     ///////// 460800 bps 설정 (26 (1Ah) BRR): 0.47 % Error at 100 MHz
     /// BRR: 0x001A
     ScibRegs.SCIHBAUD.all = 0x0000;
     ScibRegs.SCILBAUD.all = 0x001A;
/////////////////////////////////////////////////////////////////////////////
   //SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
   ScibRegs.SCIFFTX.bit.SCIFFENA = 1;		// SCI FIFO 사용 설정 Enable
   ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;	// SCI 송신 FIFO RE-enable
   ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;	// SCI 수신 FIFO 인터럽트 플래그 클리어
   ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;	// SCI 수신 FIFO RE-enable
   ScibRegs.SCIFFRX.bit.RXFFIENA = 1;		// SCI 수신 FIFO 인터럽트 Enable
   ScibRegs.SCIFFRX.bit.RXFFIL = 16;	// (0x10h) SCI 수신 FIFO 인터럽트 레벨 설정 (16개 data가 입력되면 수신 인터럽트 수행)
   ScibRegs.SCIFFCT.all=0x00;
   // After a system reset, re-enable the SCI by writing a 1 to this bit.
   ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
   //SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
   //SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

}

void scia_fifo_init()
{
   SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                  // No parity,8 char bits,
                                  // async mode, idle-line protocol
   SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                  // Disable RX ERR, SLEEP, TXWAKE
   //SciaRegs.SCICTL2.bit.TXINTENA =1;
   //SciaRegs.SCICTL2.bit.RXBKINTENA =1;


/*
 //   * working well, but some problems: DSP 수신에 에러값이 존재함. PC 수신값은 차후에 검토.

   // LOSCLK (LOSPCP) = 200/4 = 50 MHz 로 고려해서 Baud Rate 계산.
   ///////// 921600 bps 설정 (6 (6h) BRR): 3.1188 % Error at 50 MHz
   /// BRR: 0x0006
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x0006;
*/


//////////////////// 100 MHz 로 클럭 변경함 /////////////////////////////////

   // LOSCLK (LOSPCP) = 200/2 = 100 MHz 로 고려해서 Baud Rate 계산.
     ///////// 460800 bps 설정 (26 (1Ah) BRR): 0.47 % Error at 100 MHz
     /// BRR: 0x001A
     SciaRegs.SCIHBAUD.all = 0x0000;
     SciaRegs.SCILBAUD.all = 0x001A;


/////////////////////////////////////////////////////////////////////////////


   //////////////////// 200 MHz 로 클럭 변경함 /////////////////////////////////

      // LOSCLK (LOSPCP) = 200 MHz 로 고려해서 Baud Rate 계산.
        ///////// 921600 bps 설정 (26 (1Ah) BRR): 0.47 % Error at 200 MHz
        /// BRR: 0x001A
     //   SciaRegs.SCIHBAUD.all = 0x0000;
    //    SciaRegs.SCILBAUD.all = 0x001A;


   /////////////////////////////////////////////////////////////////////////////




   /*
    * 송수신 모두 잘 되는 것 같음.
    * baud rate의 error 가 작아야 하는걸까???

   // LOSCLK (LOSPCP) = 200/4 = 50 MHz 로 고려해서 Baud Rate 계산.
   ///////// 230400 bps 설정 (26 (1Ah) BRR): 0.4694 % Error at 50 MHz
   /// BRR: 0x001A
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x001A;
*/

   /*
    * 가끔 DSP 수신 에러가 발생함.

   // LOSCLK (LOSPCP) = 200/4 = 50 MHz 로 고려해서 Baud Rate 계산.
   ///////// 460800 bps 설정 (13 (Dh) BRR): 3.1188 % Error at 50 MHz
   /// BRR: 0x000D
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x000D;
   */


   /*
   // LOSCLK (LOSPCP) = 200/4 = 50 MHz 로 고려해서 Baud Rate 계산.
   ///////// 115200 bps 설정 (53 (35h) BRR): -0.469 % Error at 50 MHz
   /// BRR: 0x35
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x0035;
   */




   /*
   100 MHz
   BRR = 107.507 => 108
   actual Baud rate: 114678.9, 0.45 % (에러)

   200 MHz
   BRR = 216.014 => 216
   actual Baud rate: 115207.3733, -0.0064% (에러)
   */

   //SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
   // SCI의 송신 FIFO 설정
   SciaRegs.SCIFFTX.bit.SCIFFENA = 1;		// SCI FIFO 사용 설정 Enable
   SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;	// SCI 송신 FIFO RE-enable

   // SCI의 수신 FIFO 설정 with 인터럽트
   SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;	// SCI 수신 FIFO 인터럽트 플래그 클리어
   SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;	// SCI 수신 FIFO RE-enable
   SciaRegs.SCIFFRX.bit.RXFFIENA = 1;		// SCI 수신 FIFO 인터럽트 Enable
   SciaRegs.SCIFFRX.bit.RXFFIL = 16;	// (0x10h) SCI 수신 FIFO 인터럽트 레벨 설정 (16개 data가 입력되면 수신 인터럽트 수행)

   SciaRegs.SCIFFCT.all=0x00;

   // After a system reset, re-enable the SCI by writing a 1 to this bit.
   SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

   //SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
   //SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

}






//============================================================================================
// RS-232C 데이터 송수신 함수
//--------------------------------------------------------------------------------------------

//============================================================================================
//	Local Interrupt Service Routine : SCI 수신 FIFO 인터럽트 서비스 루틴
//--------------------------------------------------------------------------------------------
//	SCIFFRX 레지스터의 RSFFST의 값이 RXFFIL에 설정된 값보다 크거나 같으면 실행.
//  SCI RXFIFO에 수신된 데이터를 메로리에 저장함.
//--------------------------------------------------------------------------------------------

interrupt void sciaRxFifoIsr(void)
{

	for(i=0;i<RxFifoLevel;i++)
	{
		RxBuffA[i] = SciaRegs.SCIRXBUF.bit.SAR;
		test_Rev_Buf[i] = RxBuffA[i];
	}
	if (RxBuffA[0] == 'C' && RxBuffA[1] == 'M' && RxBuffA[2] == 'D' && RxBuffA[3] == ':' && RxBuffA[15] == 'z')
	{
		// PC로부터 수신받은 명령어
		if (RxBuffA[4] == 'E' && RxBuffA[5] == 'N' && RxBuffA[6] == 'T')
		//if (RxBuffA[4] == 69 && RxBuffA[5] == 78 && RxBuffA[6] == 84)
		{
			// Motor Enable!
			bMotorEnable = 1;

		}
		//else if (RxBuffA[4] == 69 && RxBuffA[5] == 74 && RxBuffA[6] == 70)
		else if (RxBuffA[4] == 'E' && RxBuffA[5] == 'N' && RxBuffA[6] == 'F')
		{
			// Motor Disable!
			bMotorEnable = 0;

			// Motor Stop 도 같이 주자.
			bMotorRun = false;

		}
		// PC로부터 수신받은 명령어 (Run/Stop)
		else if (RxBuffA[4] == 'E' && RxBuffA[5] == 'N' && RxBuffA[6] == 'R')
		{
			// Motor Run!
			bMotorRun = true;
		}
		else if (RxBuffA[4] == 'E' && RxBuffA[5] == 'N' && RxBuffA[6] == 'S')
		{
			// Motor Stop!
			bMotorRun = false;
		}
		//////////////// Friction Compensator On/Off Flag
		// PC로부터 수신받은 명령어 (Run/Stop)
		else if (RxBuffA[4] == 'F' && RxBuffA[5] == 'R' && RxBuffA[6] == 'T')
		{
			// Motor Run!
			bFrictionComp = true;
			bSwitch = true;
		}
		else if (RxBuffA[4] == 'F' && RxBuffA[5] == 'R' && RxBuffA[6] == 'F')
		{
			// Motor Stop!
			bSwitch = false;
			bFrictionComp = false;
		}





		// 수신 플래그 On
		SCI_RxFlag = true;
	}
	if(RxBuffA[0] == 'F' && RxBuffA[1] == '&' && RxBuffA[2] == 'T')
	{
		NotRx_NewDataCount = 0;
		ZeroTorque = RxBuffA[3];
		if((test_Rev_Buf[4] & 0x00000080) == 0x00000080)
		{
			MasterForce[0] = 0xFF000000;
			MasterForce[0] |= (test_Rev_Buf[4] << 16);
			MasterForce[0] |= (test_Rev_Buf[5] << 8);
			MasterForce[0] |= (test_Rev_Buf[6]);
		}
		else
		{
			MasterForce[0] = 0;
			MasterForce[0] = (test_Rev_Buf[4] << 16);
			MasterForce[0] |= (test_Rev_Buf[5] << 8);
			MasterForce[0] |= (test_Rev_Buf[6]);
		}

		fMasterForce[0] = ((float)(long)MasterForce[0]) * 0.001; //Translation Force

		if((test_Rev_Buf[7] & 0x00000080) == 0x00000080)
		{
			MasterForce[1] = 0xFF000000;
			MasterForce[1] |= (test_Rev_Buf[7] << 16);
			MasterForce[1] |= (test_Rev_Buf[8] << 8);
			MasterForce[1] |= (test_Rev_Buf[9]);
		}
		else
		{
			MasterForce[1] = 0;
			MasterForce[1] = (test_Rev_Buf[7] << 16);
			MasterForce[1] |= (test_Rev_Buf[8] << 8);
			MasterForce[1] |= (test_Rev_Buf[9]);
		}

		fMasterForce[1] = ((float)(long)MasterForce[1]) * 0.001; //Twisting Force

		if(test_Rev_Buf[10] == 0)
			bSoftwareFootButton = false;
		else if(test_Rev_Buf[10] == 1)
			bSoftwareFootButton = true;

		bMasterTwistPosCont = true;

		SCI_RxFlag = true;
	}

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    //////////////////////////////////////////////////////////////////////
    //PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;			// Acknowledge interrupt to PIE
}

interrupt void scibRxFifoIsr(void)
{
	for(i=0;i<RxFifoLevel;i++)
	{
		RxBuffB[i] = ScibRegs.SCIRXBUF.bit.SAR;
	}

	SCIB_RxFlag = true;

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    //////////////////////////////////////////////////////////////////////
    //PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;			// Acknowledge interrupt to PIE
}



