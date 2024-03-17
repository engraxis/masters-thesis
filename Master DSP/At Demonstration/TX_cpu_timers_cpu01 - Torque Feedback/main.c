// 0: Translation
// 1: Twisting
// Manual by Haptic Feedback
//###########################################################################
// FILE:   cpu_Timers_cpu01.c
// TITLE:  Cpu Timers Example for F2837xS.
//
/* ���α׷� ���� (CCS v6.1) (DSP: F28377S)
 * 1. CPUTimer01 �ֱ��� ���ͷ�Ʈ: 5 kHz sampling rate => ���� �ֱ�....2015. 05. 21. (�Ϸ�)
 * 2. SPI: DAC �׽�Ʈ (TLV5638): �Ƴ��α� ��� => ���� �����.........2015. 06. 18.  (�Ϸ�)
 * 3. EQEP: ���ڴ� ��ȣ �ؼ� �׽�Ʈ:  ���ڴ� ��ȣ�� ������ �ؼ��ϱ�...........2015. 06. 18 (1���� �׽�Ʈ��.)
 * 4. SCI ��� (PC - DSP) �׽�Ʈ
 *
 *
 *
 * ���: bool �� (unsigned char)�� ����, Debug��忡��, ���� �ƽ�Ű �ڵ�� ǥ�õǴ� �� ����. Uint16�� ��� ���. (ũ�� ����)
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


// LED ON/OFF ����� ��� ���� -------------------------------------------------------
#define GREEN_LED_ON		GpioDataRegs.GPASET.bit.GPIO31=1	// ��� LED ON
#define GREEN_LED_OFF		GpioDataRegs.GPACLEAR.bit.GPIO31=1	// ��� LED OFF
//#define RED_LED_ON			GpioDataRegs.GPBSET.bit.GPIO34=1	// ���� LED ON
//#define RED_LED_OFF			GpioDataRegs.GPBCLEAR.bit.GPIO34=1	// ���� LED OFF
//#define RED_LED_TOGGLE		GpioDataRegs.GPBTOGGLE.bit.GPIO34=1	// ���� LED TOGGLE
// -----------------------------------------------------------------------------------

#define BUTTON GpioDataRegs.GPADAT.bit.GPIO9;
#define START_MOTOR_CONTROL GpioDataRegs.GPBDAT.bit.GPIO43;
#define STOP_MOTOR_CONTROL GpioDataRegs.GPBDAT.bit.GPIO42;

#define LOOPRATE	GpioDataRegs.GPCTOGGLE.bit.GPIO69 = 1;

// Motor Enable ON/OFF ����� ��� ���� -------------------------------------------------------
#define MOTOR_ON		GpioDataRegs.GPBSET.bit.GPIO33=1	// Motor ON
#define MOTOR_OFF		GpioDataRegs.GPBCLEAR.bit.GPIO33=1	// Motor OFF
#define TWIST_MOTOR_ON	GpioDataRegs.GPBSET.bit.GPIO34=1
#define TWIST_MOTOR_OFF	GpioDataRegs.GPBCLEAR.bit.GPIO34=1
// -----------------------------------------------------------------------------------

// �ݿø� ��ũ�� �Լ� (�Ҽ��� ù°�ڸ�)--------------------------------------------------------------
// float ���� ���������� �ٲ� ��, ������ ���̱� ���ؼ�.
#define ROUND(X)	((X)+(0.5))

//------------------------------------------------------------------------

// sign or signum ��ũ�� �Լ�
#define SIGN(x)		(  ((x) < 0) ? -1 : ( ((x) > 0) ? 1 : 0.0  )   )
//------------------------------------------------------


// Note: �� ����: [N], ��ũ ����: [mNm], ���� ����: [mm], Twisting ����: [rad]
// ����:
// Pose
/////Push ������ +,  ������ Twisting�� + (���� ��ġ�� ��ȣ ������)

// Force/Torque
//// ���� ������ + ��, ������ Twisting�� + ��ũ (Twisting ��ũ ���� ������)


// Pull limit: 39.08 => 30.0 [mm]
// Push limit: -24.91 => -20.0 [mm]

// PI: 3.14159265359
#define PI		3.14159265359

// ������: ȸ��� => ����� [mm]
// ���̾ ���� ����: 13.1 [mm] = 2*r
// ���̾ ���� ������: 6.55 [mm]
// 6.55 [mm]
#define TRANS_R			6.55

// ����� ��ġ: 1 [mm]

//////////////////////////////////////////////////////////////////////////////
//////////			���� �ֱ�			//////////////////////////////////////////
// 5 kHz => 0.2 ms = 200 us
// 5 kHz => 200
//#define CONTROL_HZ	5000
//#define CONTROL_RATE 200	// CPU �ֱ� �����ϴ� ��

// 4 kHz
#define CONTROL_RATE 250	// CPU �ֱ� �����ϴ� ��
#define CONTROL_HZ	4000
#define Ts			0.00025		// ���� �ֱ� [s]

// Prototype statements for functions found within this file.
__interrupt void cpu_timer0_isr(void);

float32 Timer_Prd;		// ���� �ֱ� [usec] ����

///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//////////////////   SCI (PC - DSP ���) ����		///////////////////////////////

// ����: ���ͷ�Ʈ ���
interrupt void sciaRxFifoIsr(void);	// FIFO RX interrupt
interrupt void scibRxFifoIsr(void);	// FIFO-B RX interrupt
void scia_fifo_init(void);			// SCI-A ��Ʈ �ʱ�ȭ
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


// 2��° �۽��� ���� flag ����
Uint16 bNextTx;
Uint16 bStartMotorControl;
Uint16 bStopMotorControl;

float32 fPushCalib;
float32 fPullCalib;


// Save Data �۽� Index; (�ʸ����� ����: 1 kHz �� ���, 100��)
float fSCI_TxIndex;


//////////////////////////////////////////////////
/////////////		variables for data communication between DSP and PC
//////////////////////////////////////////////////////////////////////////////
//	Ư�� float data �� ���ؼ�.
#define NUM_OF_BUF	16
// float -> Uint32 ������ ���� (�۽ſ�)
Uint32 tBuf[NUM_OF_BUF];
Uint16 Tx_cnt;

// 8�� �۽��� data �ӽ� �����
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
// DAC (TLV5638): SPI ������� ����
#include "TLV5638_API_F2837xS.h"

Uint16	BackTicker;
Uint16	TestDataA;
Uint16	TestDataB;
TLV5638 DAC = TLV5638_DEFAULTS;
Uint16 xa;



///////////////////////////////////////////////////////////////////////
///////		���ڴ� ���� (EQEP)
/////////////////////////////////////////////////////////////////////
/* Define for used Encoder Resolution */
// ���ڴ� ����: ����: 8000, Twisting: 1024
#define	ENCODER_REV1		8000			/* Pulse/Revolution */
#define	ENCODER_REV2		1024			/* Pulse/Revolution */

// Encoder ����
void InitEQep1(Uint32 LineEncoder1);
void InitEQep2(Uint32 LineEncoder2);

// Encoder ����
//Uint32	PositionMax;
Uint32	PositionMax[2];			// EQEP-1 ��� ���� ��ġ ī����(Position Counter)�� �ִ밪
Uint32	PositionCounter[2];		// EQEP-1 ��� ���� ��ġ ī����(Position Counter)�� ���簪
Uint32	RotateDirection[2];		// EQEP-1 ����� Quadrature Decoder Unit�� �ؼ��س� ȸ������ (0 : ������, 1 : ������)
float32	RotateAngleUnit[2];		// EQEP-1 ���� �ؼ��س� ���� ȸ������ ������ ȯ���� �� (pu : 0.0 ~ 1.0)
float32	RotateAngleDegree[2];	// EQEP-1 ���� �ؼ��س� ���� ȸ������(Degree)
float32	RotateAngleRad[2];	// EQEP-1 ���� �ؼ��س� ���� ȸ������(Rad)

int16 NumOfTurns[2];

// HD Pos!
float32 fCurrentPos[2];
// HD Vel, Accel
float32 fCurrentVel[2], fCurrentAccel[2];
float32 fprePos[2], fpreVel[2];

float32 fCurrentPos_ToSend[2];
float32 fPos_JustBeforeButton[2];
float32 fMoveDuringButton[2];

// ������ ȸ���� ���� �ӽ� ����
float32 fTransRad;


////////////////////////////////////
///////////		��ġ ����ġ		////////
Uint16 bInput_TouchSW;		// ��ġ ����ġ �Է�


Uint16 bHandShake;
Uint16 bSwitch;
//////////////////////////////////////////////////////////
///////			���� ���� ������		////////////////////////
///////////////////////////////////////////////////////////
// ���� & Twisting

// DSP value (X: 0 ~ 4095) to bipolar 10V DAC output (Vo: motor control input)
// Vo = aX + b

// ������ (a: 0.004898, b: -9.9971)
// Twsiting (a: 0.004874, b: -9.95)

// DSP to DAC parameters
float32 fa[2], fb[2];

// Voltage to current relationship
// ������: 10.86/10 = 1.086
// Twisting: 4.2/10 = 0.42
float32 fG_Ia[2];

// Torque constant
// ������: 29.2 [mNm/A]
// Twisting: 22.9 [mNm/A]
float32 fKt[2];


// ��ũ�� �Լ�
#define X_TO_TORQUE0(DSP_X0)	((fKt[0])*(fG_Ia[0])*(((fa[0])*(DSP_X0))+(fb[0])))
#define X_TO_TORQUE1(DSP_X1)	((fKt[1])*(fG_Ia[1])*(((fa[1])*(DSP_X1))+(fb[1])))

#define TORQUE_TO_X0(TORQUE0)	(((TORQUE0)-((fKt[0])*(fG_Ia[0])*(fb[0])))/((fKt[0])*(fG_Ia[0])*(fa[0])))
//#define TORQUE_TO_X1(TORQUE1)	(((TORQUE1)-((fKt[1])*(fG_Ia[1])*(fb[1])))/((fKt[1])*(fG_Ia[1])*(fa[1])))
// ���� ������ ���߱� ���ؼ� Twisting ��ũ ��ȣ �ٲٱ�.
#define TORQUE_TO_X1(TORQUE1)	(((-1.0*(TORQUE1))-((fKt[1])*(fG_Ia[1])*(fb[1])))/((fKt[1])*(fG_Ia[1])*(fa[1])))

// �������� Force => Torque ��ȯ
#define TRANS_FOR_TO_TOR(FORCE0)		((FORCE0)*(TRANS_R))

// ���� ��ġ, ������
//float32 fVE_Pos, fVE_K;

// �������� Force
float32 fTrans_Force;
float32 MasterTorqueTest;
Uint16 button1, button2;

// PI: 3.14159265359
//#define PI		3.1416
//#define PI		3.14159265359

// ������: ȸ��� => ����� [mm]
// ���̾ ���� ����: 13.1 [mm] = 2*r
// ���̾ ���� ������: 6.55 [mm]
// ���� ���� [mm]: (2*PI*r / 360) * x [deg]

// 6.55 [mm]
//#define TRANS_R			6.55


// PI*2*r * DEG0 / 360
//#define TRANS_POS(DEG0)			((PI*13.1*(DEG0))/360.0)

// ����ġ: r: 8.9406 (���ʰ� �������� �ٸ�.)

// A circular helix: arc length L = sqrt(r^2 + (pitch/2*PI)^2) * ȸ����
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

// TEST
//a Uint32 uTest1, uTest2, uTest3;
//a float32 fTest1, fTest2;

Uint32 SlavePos[2];
float32 fSlavePos[2];

void main(void)

{

	//============================================================================================
	// Step 1. Disable Global Interrupt
	//			���� ���ͷ�Ʈ ��-Ȱ��ȭ �� ���ͷ�Ʈ �÷��� �ʱ�ȭ
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
// Ĩ�� ��� ����� ��Ʈ ���� �ʱ�ȭ(InitGpio)
	InitGpio();


	///////////////////////////////////////////////////////
	/////////////		LED Indicator //////////////////////
	////////////////////////////////////////////////////////
	// Note: GPIO34 (Red)�� ���� �ֱ� Ȯ�ε� ����.

	// GPIO31���� ���� ����¿����� �����ϰ� CPU1�� ������ �������� �׷� ���� ������ �ο�
	// GPIO31���� Push-Pull ȸ�ο� ���� �����Ʈ�� ���� (��� LED�� ����Ǿ� ����)
	GPIO_SetupPinMux(31, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(31, GPIO_OUTPUT, GPIO_PUSHPULL);

	// GPIO34���� ���� ����¿����� �����ϰ� CPU1�� ������ �������� �׷� ���� ������ �ο�
	// GPIO34���� Push-Pull ȸ�ο� ���� �����Ʈ�� ���� (���� LED�� ����Ǿ� ����)
//a	GPIO_SetupPinMux(34, GPIO_MUX_CPU1, 0);
//a	GPIO_SetupPinOptions(34, GPIO_OUTPUT, GPIO_PUSHPULL);


	//////////////////////////////////////////////////////////
	//////////// SCI-A (DSP <=> PC ���) ����
	/////////////////////////////////////////////////////////

	//  GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
	//  GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS
	// These functions are found in the F2837xS_Gpio.c file.
	GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);

	//////////////////////////////////////////////////////////
	//////////// SCI-B (DSP <=> DSP ���) ����
	/////////////////////////////////////////////////////////

	//  GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
	//  GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS
	// These functions are found in the F2837xS_Gpio.c file.
	GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 2);
	GPIO_SetupPinOptions(19, GPIO_INPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 2);
	GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_ASYNC);

	/////////////////////////////////////////////////////
	/////////		EQEP A & B ��Ʈ ����		/////////////
	////////////////////////////////////////////////////


	GPIO_SetupPinMux(9, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(9, GPIO_INPUT, GPIO_QUAL6);

	GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_QUAL6);

	GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(42, GPIO_INPUT, GPIO_QUAL6);



	EALLOW;

	// for EQEP A

	// GPIO50������ 53������ �ɵ鿡 ���� ���� Pull-Up ����
	GpioCtrlRegs.GPBPUD.bit.GPIO50 = 1;		// Enable pull-up on GPIO50 (EQEP1A) //
	GpioCtrlRegs.GPBPUD.bit.GPIO51 = 1;		// Enable pull-up on GPIO51 (EQEP1B) //
	//GpioCtrlRegs.GPBPUD.bit.GPIO52 = 1;
	//GpioCtrlRegs.GPBPUD.bit.GPIO53 = 1;

/*
	// GPIO50������ 53�������� �ɵ鿡 ���ø� ������ 6�� ������ ����(Input Qualification) Ȱ��ȭ
	GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 2;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 2;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO52 = 2;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 2;

	// GPIO48������ 55�������� ����Ǵ� ������ ���� ���ø� �ֱ� ����
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 255;	// ���ø� �ֱ� = SYSCLK/510
*/
	// No Input Qualification (SYSCLKOUT �� ����ȭ)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 0;		// Sync to SYSCLKOUT GPIO50 (EQEP1A)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 0;		// Sync to SYSCLKOUT GPIO51 (EQEP1B)

	// Specifies the qualification sampling period for GPIO48 to GPIO55 //
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 0x00;

	// GPIO50������ 53�������� �ɵ��� EQEP-1 ��� ��� ������ ����
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

	// Select the pin function: (GPxMUXn registers ����)
	// sprufb0d - 76 page ����
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;	// Configure GPIO50 as EQEP1A //
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;	// Configure GPIO51 as EQEP1B //



	////////////////////////////////////////////
	////////	GPIO: Touch Switch �Է� (I)
	////////////////////////////////////////////
	// GPIO 32

	///////////////// Input Qualification
	// GPIO 32 ~ 63 // ��������� 32���� �ش�
	// 3 - samples
	// �ֱ�: 2 * {(2 * GPxCTRL[QUALPRDn]) * T_SYSCLKOUT}
	// 5 - samples
	// �ּ� �ֱ�: 5 * {(2 * GPxCTRL[QUALPRDn]) * T_SYSCLKOUT}

	// (QUALPRD0: GPIO 32 ~ 39)  Qual period ����
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 1;		// 2 * T_SYSCLKOUT (= PLLSYSCLK/2)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 2;		// Qualification using 6 samples

	EDIS;

	////////////////////////////////////////////
	////////	GPIO: Motor Enable ��� (O)
	////////////////////////////////////////////
	// GPIO 33

	/////////////	Motor Enable /////////////////////
	// GPIO 33 (���)
	//GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;		// ������� ����
	//GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;	// Low ���
	//MOTOR_OFF;

	// GPIO33���� ���� ����¿����� �����ϰ� CPU1�� ������ �������� �׷� ���� ������ �ο�
	// GPIO33���� Push-Pull ȸ�ο� ���� �����Ʈ�� ���� (���� LED�� ����Ǿ� ����)
	GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(33, GPIO_OUTPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(34, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(34, GPIO_OUTPUT, GPIO_PUSHPULL);


	GPIO_SetupPinMux(69, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(69, GPIO_OUTPUT, GPIO_PUSHPULL);


	MOTOR_OFF;
	TWIST_MOTOR_OFF;


	// �ܰ� 4. �ֺ�ȸ�� ���ͷ�Ʈ Ȯ��ȸ��(PIE) �ʱ�ȭ --------------------------------
	// PIE ȸ�� ��Ʈ�� �������� �ʱ�ȭ / ��� ���ͷ�Ʈ ��-Ȱ��ȭ(Disable)
	// ��� ǥ��(Flag) �������� �ʱ�ȭ(Clear)
	// �Լ��� ���ǵ� ���� �̸� : F2837xS_PieCtrl.c

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xS_PieCtrl.c file.
	InitPieCtrl();


	// �ܰ� 5. PIE ȸ�� ���ͷ�Ʈ ���� �ʱ�ȭ -----------------------------------------
	// PIE ���� ���̺� �ʱ�ȭ �� �⺻ ���ͷ�Ʈ ���� ��ƾ ����
	// �ֺ�ȸ�� ���ͷ�Ʈ Ȯ�� ȸ���� ��� ���͵鿡 �⺻ ���� ��ƾ �Լ� ����
	// �⺻ ���ͷ�Ʈ ���� ��ƾ �Լ����� ���ǵ� ���� �̸� : F2837xD_DefaultISR.c
	// �Լ��� ���ǵ� ���� �̸� : F2837xS_PieVect.c

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

	// SCI-A ���� ���ͷ�Ʈ �Լ� ������
	PieVectTable.SCIA_RX_INT = &sciaRxFifoIsr;
	PieVectTable.SCIB_RX_INT = &scibRxFifoIsr;

	EDIS;
	// This is needed to disable write to EALLOW protected registers


	//////////////////////////////////////////
	// �ܰ� 8. ���� �ʱ�ȭ
	///////////////////////////////////////////////

	////////////////////////////////////////
		// SCI ��� ����

		SCI_RxFlag = false;
		SCIB_RxFlag = false;

		SCI_cnt = 0;
		SCI_cnt1 = 0;

		// Save Data �۽� Index; (�ʸ����� ����: 1 kHz �� ���, 100��)
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
		//////////		SCI ������ ��� ����
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
			// 8�� �۽��� data �ӽ� �����
			fTempTxBuf[i] = 0.0;


		// 2��° �۽��� ���� flag ����
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



bStartMotorControl = false;
bStopMotorControl = false;
	////////////////////////////////////////
	////		EQEP ����
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
		RotateAngleRad[i] = 0.0;	// EQEP-1 ���� �ؼ��س� ���� ȸ������(Rad)

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
	//// ��ġ ����ġ
	////////////////////
	bInput_TouchSW = false;


	//////////////////////////////////////////////////////////
	///////			���� ���� ������		////////////////////////
	///////////////////////////////////////////////////////////
	// ���� & Twisting

	// DSP value (X: 0 ~ 4095) to bipolar 10V DAC output (Vo: motor control input)
	// Vo = aX + b

	// DSP to DAC parameters
	// ������ (a: 0.004898, b: -9.9971)
	fa[0] = 0.004914;
	fb[0] = -10.01;
	fa[1] = 0.004914;
	fb[1] = -10.01;

	// Voltage to current relationship
	// ������: 10.86/10 = 1.086
	fG_Ia[0] = 1.086;

	// Twisting: 4.2/10 = 0.42
	//fG_Ia[1] = 0.42;
	fG_Ia[1] = 0.14;

	// Torque constant
	// ������: 29.2 [mNm/A]
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


	// circular helix ��ũ ���� ����
	fArcGain = sqrt((TRANS_R*TRANS_R) + 1/(4*PI*PI));


	//// Chirp signal variables initialize
	//fA = 1.0;
	//fA = 7.0;

	// ��ġ �����
	//fA = 4.0;
//a	fA = 3.0;



//a	fF0 = 0.0;
	//fF1 = 20.0;
	//fF1 = 5.0;

//a	fF1 = 8.0;


	//fF1 = 10.0;


	//fT1 = 1800.0;	// [sec], 30��

	//fT1 = 60.0;	// [sec], 30��

//a	fT1 = 300.0;	// [sec], 5��

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
	/////////		DAC (TLV5638) ����
	///////////////////////////////////////

	// ���� 50 MHz default clock ��� (SPI ��� �ʱ�ȭ �� DAC Ĩ ����)
	DAC.init(&DAC);

	BackTicker = 0;

	// zero torque�� �ʱ�ȭ
	// ideal condition: 2048 = 0 V�� �ǹ���.
	// ideal: a = 10/2048, b = -10
	// X = (Vo - b) / a = 10 / (10/2048) = 2048

	TestDataA = ROUND(TORQUE_TO_X0(0.0));		// actual: 2037
	TestDataB = ROUND(TORQUE_TO_X1(0.0));		// actual: 2037


	// SPI ������� motor torque ���� (0.0 ��ũ ����)
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
	///		���� Test
	// ���� ��ġ
//a	fVE_Pos = 3.0;

//a	fVE_K = 1.0;

	// �������� Force
	fTrans_Force = 0.0;

	///////////////////////////////////////////


	// Step 4. Initialize the Device Peripheral. This function can be
	//         found in F2806x_CpuTimers.c
	InitCpuTimers();   // For this example, only initialize the Cpu Timers


	// 4 kHz => 0.25 ms = 250 us
	Timer_Prd = CONTROL_RATE;		// Ÿ�̸� �ֱ� ���� (usec ����)


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


	// CPU Timer0 ���ͷ�Ʈ Ȱ��ȭ
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
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;		// PIE ���ͷ�Ʈ(SCIRXINTA) : Enable (INT9.1)
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
///////////						 ���� ������			//////////////////////////////////////////
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
	if(bStopMotorControl)
	{
		bFrictionComp = 0;
		bMotorEnable = 0;
		bMotorRun = 0;
	}

	LOOPRATE;
	bSwitch = !BUTTON;

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
	// Obtain Touch SW input (TEST) - GPIO 32��
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
	// EQEP-1 ��� ���� ��ġ ī����(Position Counter)�� ���簪�� 'PositionCounter' ������ ����
	PositionCounter[0] = EQep1Regs.QPOSCNT;
	// EQEP-1 ����� Quadrature Decoder Unit�� �˾Ƴ� ȸ�������� 'RotateDirection' ������ ����
	RotateDirection[0] = EQep1Regs.QEPSTS.bit.QDF;
	// ������ ȸ�������� ������ ȯ�� (0.0 ~ 1.0)
	RotateAngleUnit[0] = (float32)PositionCounter[0] / PositionMax[0];
	// ������ ȸ������ ����(Degree)
	RotateAngleDegree[0] = RotateAngleUnit[0] * 360;
	// ������ ȸ������ ���� (Rad)
	//RotateAngleRad[0] = RotateAngleUnit[0] * 2*PI;	// EQEP-1 ���� �ؼ��س� ���� ȸ������(Rad)

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

	// ���� ���� ��ġ ��� (Deg)
	//fCurrentPos[1] = RotateAngleDegree[1] + 360.0*NumOfTurns[1];
	// rad
	fCurrentPos[1] = (RotateAngleUnit[1] + (float)NumOfTurns[1])*2*PI;

	if(!bSwitch)
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


	// �ӵ� �� ���ӵ� ��� (Backward ���) (���� * fs)
	fCurrentVel[0] = (fCurrentPos[0] - fprePos[0])*CONTROL_HZ;		// mm/s
	fCurrentVel[1] = (fCurrentPos[1] - fprePos[1])*CONTROL_HZ;		// rad/s

	fCurrentAccel[0] = (fCurrentVel[0] - fpreVel[0])*CONTROL_HZ;		// mm/(s^2)
	fCurrentAccel[1] = (fCurrentVel[1] - fpreVel[1])*CONTROL_HZ;		// rad/(s^2)


	///////////////////////////////////////////////////////////////
	/////////			DAC (TLV5638): ���� ����� ��� (��ũ)�� ���� ��ũ ����⿡ ����
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
	// Push: 38.7 mm  => �� 30 mm
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

		// Time variable (����)
		fControl_Time = 0.0;

		// �ʱ�ȭ
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

	// SCI-A ��� �ֱ�� ī����
	++SCI_cnt;
	++SCI_cnt1;

	// Save Data �۽� Index; (�ʸ����� ����: 1 kHz �� ���, 100��)
	fSCI_TxIndex += 0.001;


	if (!(SCI_cnt % 4))
	{
		// Reset ī����
		SCI_cnt = 0;

		// 2��° �۽��� ���� flag ����: On
		bNextTx = 1;


			///////////////////////////////////////////////
			////	SCI ���: �۽� �κ� ////////////////////////
			/////////////////////////////////////////////////////////
			//// �ѹ� �۽��� �� �ִ� data ����: 4 ��

			////	������ Data

			/////// float ��
			/// a. (97)  Pos 0
			/// b. (98)  Pos 1
			/// c. (99)  Vel 0
			/// d. (100) Vel 1


			//////////////////////////////////////////////
			/////	SCI ���:  data ����ȯ:  => 8 bit Uint8 �� ��ȯ

//a			++ftest_Buf2;

			// ���Ѱ�: 8388.608
//a			if (ftest_Buf2 > 8001)
//a				ftest_Buf2 = 0.123;

			//++ftest_Buf3;

			// �Ҽ��� 3�ڸ������� ���
			// long: 32 bit
			// 24 bit ������ ���. ���� ����Ʈȭ�� ���� (32��Ʈ ��ü ���� �͵� ����غ� ����)

			// MSB (��ȣ ��Ʈ)�� ���� ���
			// 2^23 = 8388608
			// �Ҽ����� �ش��ϴ� 3�ڸ� ����,
			// 8388.608 ��  ��� �ִ� ���Ѽ��� �ȴٰ� �Ǵܵ�. (üũ ��!) (������ ���)

			// (����� ���) 2^24 = 16777216 �� �ִ� ���Ѱ��� ��.
			// 1000 �� ������, 16777.216 �� ���� DSP ���ο��� ����ϴ� ��� �ִ밪

			// �ᱹ ���� ���� ���� ����ϸ�, 8388.608  ��  ����, ��� �ִ� ���Ѱ���. (DSP ���ο��� �������� �� ��.  24bit�� �����ϹǷ�!!!)


			///////////////////////////////////////////////////////
			// a. fCurrentPos[0]
			// b. fCurrentPos[1]


			// 8���� data�� �۽� �ӽ� ����ҿ� �����Ѵ�. (������ ������ ���ؼ�.
			// (4~7)�� ���� 4���� ������ ����.
			fTempTxBuf[0] = fSCI_TxIndex;
			fTempTxBuf[1] = fCurrentPos[0]*0.1;
			fTempTxBuf[2] = fCurrentVel[0]*0.01;
			fTempTxBuf[3] = fCurrentAccel[0]*0.00001;

			//fTempTxBuf[4] = fMotorTorque[0];
			fTempTxBuf[4] = fTrans_Force*0.1;			// ���� Force
			fTempTxBuf[5] = fCurrentPos[1]*0.1;
			fTempTxBuf[6] = fCurrentVel[1]*0.01;
//a			fTempTxBuf[7] = fVel_Err*0.1;		// overflow ������ ���ؼ� ������ �ٿ� (0.1)


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
		TxBuffA[1] = (test_Buf[0] >> 16) & 0x00FF;		// 23 ~ 16 bit �� ����
		TxBuffA[2] = (test_Buf[0] >> 8) & 0x00FF;		// 15 ~ 8 bit �� ����
		TxBuffA[3] = test_Buf[0] & 0x00FF;				// 7 ~ 0 bit (���� 8 bit) �� ����

		//MasterPos1 - Twisting
		TxBuffA[4] = 98;
		TxBuffA[5] = (test_Buf[1] >> 16) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffA[6] = (test_Buf[1] >> 8) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffA[7] = test_Buf[1] & 0x00FF;				// ���� 16 bit �� ����

		//SlavePos0 - Translation
		TxBuffA[8] = 99;
		TxBuffA[9] = (test_Buf[2] >> 16) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffA[10] = (test_Buf[2] >> 8) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffA[11] = test_Buf[2] & 0x00FF;				// ���� 16 bit �� ����

		//SlavePos1 - Twisting
		TxBuffA[12] = 100;
		TxBuffA[13] = (test_Buf[3] >> 16) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffA[14] = (test_Buf[3] >> 8) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffA[15] = test_Buf[3] & 0x00FF;				// ���� 16 bit �� ����




		//Preparing data to send to Slave via SCI-B
		fTempTxBuf[1] = fCurrentPos_ToSend[0]*0.1;
		fTempTxBuf[5] = fCurrentPos_ToSend[1]*0.1;
		// Sending only the position information of Linear and Rotary Motion
		test_BufB[0] = (long)(1000 * fTempTxBuf[1]);
		test_BufB[1] = (long)(1000 * fTempTxBuf[5]);
		// Translation Motion Position
		TxBuffB[0] = 'a';
		TxBuffB[1] = (test_BufB[0] >> 16) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffB[2] = (test_BufB[0] >> 8) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffB[3] = test_BufB[0] & 0x00FF;
		// Rotation Motion Position
		TxBuffB[4] = 'b';
		TxBuffB[5] = (test_BufB[1] >> 16) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffB[6] = (test_BufB[1] >> 8) & 0x00FF;		// ���� 16 bit �� ����
		TxBuffB[7] = test_BufB[1] & 0x00FF;				// ���� 16 bit �� ����



		//---------------------------------------------------------------------------
		//		A. SCI ������ �۽�
		//	---------------------------------------------------------------------------//
		// Trnasmit FIFO�� empty �� �ɶ�����, ���!
		while(SciaRegs.SCIFFTX.bit.TXFFST != 0);

		// Transmit FIFO �� empty �̸�, �۽��� �����Ѵ�.
		for(i=0;i<RxFifoLevel;i++)
		{
			//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
			SciaRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;
		}

	if(bHandShake)
	{
		//---------------------------------------------------------------------------
		//		B. SCI ������ �۽�
		//	---------------------------------------------------------------------------//
		// Trnasmit FIFO�� empty �� �ɶ�����, ���!
		while(ScibRegs.SCIFFTX.bit.TXFFST != 0);

		// Transmit FIFO �� empty �̸�, �۽��� �����Ѵ�.
		for(i=0;i<RxFifoLevel;i++)
		{
			//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
			ScibRegs.SCITXBUF.all = TxBuffB[i] & 0x00FF;
		}
///////////////////////////////////////////////////////////////////////////////////////////
	}
		// Save Data �۽� Index; (8000���� ����: 1 kHz �� ���, 8��)
		if (fSCI_TxIndex >= 8000.0)
			fSCI_TxIndex = 0.0;



	// SCI ��� if �� End
	///////////////////////////////////////////////////////
	}


////////////////////////////////////////////////////////////////////////////
	/////////////////			2��° Data (16��)�� ���� �ֱ⿡ �����ؼ� ������		//////////////
	/////////////////////////////////////////////////////////////////////////////////////

	// 1 kHz�� ������. ��, 1�ʿ� 1000�� ������ (���� 4kHz ���� �ֱ� => 1�ʿ� 4õ��)
	// 921600 bps�δ� 4kHz ������. (Good)
	// 1 kHz�� ������ ������.
	else if ((SCI_cnt == 2) && bNextTx)
	{

			// Reset ī����
			//SCI_cnt1 = 0;

			// 2��° �۽��� ���� flag ����: On
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
	//		TxBuffA[1] = (test_Buf[0] >> 16) & 0x00FF;		// 23 ~ 16 bit �� ����
	//		TxBuffA[2] = (test_Buf[0] >> 8) & 0x00FF;		// 15 ~ 8 bit �� ����
	//		TxBuffA[3] = test_Buf[0] & 0x00FF;				// 7 ~ 0 bit (���� 8 bit) �� ����
			TxBuffA[1] = bSwitch;
			TxBuffA[2] = 0;
			TxBuffA[3] = 0;

			TxBuffA[4] = 102;
			TxBuffA[5] = (test_Buf[1] >> 16) & 0x00FF;		// ���� 16 bit �� ����
			TxBuffA[6] = (test_Buf[1] >> 8) & 0x00FF;		// ���� 16 bit �� ����
			TxBuffA[7] = test_Buf[1] & 0x00FF;				// ���� 16 bit �� ����

			TxBuffA[8] = 103;
			TxBuffA[9] = (test_Buf[2] >> 16) & 0x00FF;		// ���� 16 bit �� ����
			TxBuffA[10] = (test_Buf[2] >> 8) & 0x00FF;		// ���� 16 bit �� ����
			TxBuffA[11] = test_Buf[2] & 0x00FF;				// ���� 16 bit �� ����

			TxBuffA[12] = 104;
			TxBuffA[13] = (test_Buf[3] >> 16) & 0x00FF;		// ���� 16 bit �� ����
			TxBuffA[14] = (test_Buf[3] >> 8) & 0x00FF;		// ���� 16 bit �� ����
			TxBuffA[15] = test_Buf[3] & 0x00FF;				// ���� 16 bit �� ����


			//---------------------------------------------------------------------------
			//		A. SCI ������ �۽�
			//	---------------------------------------------------------------------------//
			// Trnasmit FIFO�� empty �� �ɶ�����, ���!
			while(SciaRegs.SCIFFTX.bit.TXFFST != 0);

			// Transmit FIFO �� empty �̸�, �۽��� �����Ѵ�.
			for(i=0;i<RxFifoLevel;i++)
			{
				//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
				SciaRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;
			}
/*
			//---------------------------------------------------------------------------
			//		B. SCI ������ �۽�
			//	---------------------------------------------------------------------------//
			// Trnasmit FIFO�� empty �� �ɶ�����, ���!
			while(ScibRegs.SCIFFTX.bit.TXFFST != 0);

			// Transmit FIFO �� empty �̸�, �۽��� �����Ѵ�.
			for(i=0;i<RxFifoLevel;i++)
			{
				//SciaRegs.SCITXBUF = TxBuffA[i] & 0x00FF;
				ScibRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;
			}
*/

		// SCI ��� if �� End
///////////////////////////////////////////////////////
		}

///////////////////////////////////


	////////////	���� ����


	// �ӵ� �� ���ӵ� ��� (Backward ���) (���� * fs)
	fpreVel[0] = fCurrentVel[0];
	fpreVel[1] = fCurrentVel[1];

	fprePos[0] = fCurrentPos[0];
	fprePos[1] = fCurrentPos[1];
	}
	///////////////////////////////////////////////////////////////////////////
	// Acknowledge this interrupt to receive more interrupts from group 1
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
	EQep1Regs.QDECCTL.bit.QSRC = 0;	// EQEP-1 ����� Quadrature Decoder Unit�� Quadrature count mode�� ����

	// eQEP Control (QEPCTL) Register
	// 1x: Position counter is unaffected by emulation suspend
	// EQEP-1 ��� ���� ī���͵鿡 ���� ���ķ��̼� ��� ���� : (2h) ���ķ��̼� ����(Suspend) ����� ���� ī���͵��� ���ۿ� ������ ���� ����
	EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;

	// Position Counter Reset on Maximum Position (QEPCTL[PCRM]=01)
	// ������: QPOSMAX => 0 (with overflow flag = 1)
	// ������: (with underflow flag = 1) QPOSMAX <= 0
	// EQEP-1 ��� ���� ��ġ ī����(Position Counter)�� �ִ밪(QPOSMAX)�� �̸��� ����(Reset) �ǵ��� ��
	EQep1Regs.QEPCTL.bit.PCRM = 1;			/* Position Counter Reset on Maximum Position */
	// EQEP-1 ��� ���� ��ġ ī������ �ִ밪 ����(���ڴ� rev. x 4 QCLK @ 1 revolution)
	EQep1Regs.QPOSMAX = PositionMax1 - 1;	/* 24 x 4 QCLK @ 1 revolution */

	// QPEN: 1: eQEP position counter is enabled
	// EQEP-1 ��� ���� ��ġ ī���� Ȱ��ȭ(Enable)
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
	// ������: QPOSMAX => 0 (with overflow flag = 1)
	// ������: (with underflow flag = 1) QPOSMAX <= 0
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

//////////////////// 100 MHz �� Ŭ�� ������ /////////////////////////////////
   // LOSCLK (LOSPCP) = 200/2 = 100 MHz �� ����ؼ� Baud Rate ���.
     ///////// 460800 bps ���� (26 (1Ah) BRR): 0.47 % Error at 100 MHz
     /// BRR: 0x001A
     ScibRegs.SCIHBAUD.all = 0x0000;
     ScibRegs.SCILBAUD.all = 0x001A;
/////////////////////////////////////////////////////////////////////////////
   //SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
   ScibRegs.SCIFFTX.bit.SCIFFENA = 1;		// SCI FIFO ��� ���� Enable
   ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;	// SCI �۽� FIFO RE-enable
   ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;	// SCI ���� FIFO ���ͷ�Ʈ �÷��� Ŭ����
   ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;	// SCI ���� FIFO RE-enable
   ScibRegs.SCIFFRX.bit.RXFFIENA = 1;		// SCI ���� FIFO ���ͷ�Ʈ Enable
   ScibRegs.SCIFFRX.bit.RXFFIL = 16;	// (0x10h) SCI ���� FIFO ���ͷ�Ʈ ���� ���� (16�� data�� �ԷµǸ� ���� ���ͷ�Ʈ ����)
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
 //   * working well, but some problems: DSP ���ſ� �������� ������. PC ���Ű��� ���Ŀ� ����.

   // LOSCLK (LOSPCP) = 200/4 = 50 MHz �� ����ؼ� Baud Rate ���.
   ///////// 921600 bps ���� (6 (6h) BRR): 3.1188 % Error at 50 MHz
   /// BRR: 0x0006
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x0006;
*/


//////////////////// 100 MHz �� Ŭ�� ������ /////////////////////////////////

   // LOSCLK (LOSPCP) = 200/2 = 100 MHz �� ����ؼ� Baud Rate ���.
     ///////// 460800 bps ���� (26 (1Ah) BRR): 0.47 % Error at 100 MHz
     /// BRR: 0x001A
     SciaRegs.SCIHBAUD.all = 0x0000;
     SciaRegs.SCILBAUD.all = 0x001A;


/////////////////////////////////////////////////////////////////////////////


   //////////////////// 200 MHz �� Ŭ�� ������ /////////////////////////////////

      // LOSCLK (LOSPCP) = 200 MHz �� ����ؼ� Baud Rate ���.
        ///////// 921600 bps ���� (26 (1Ah) BRR): 0.47 % Error at 200 MHz
        /// BRR: 0x001A
     //   SciaRegs.SCIHBAUD.all = 0x0000;
    //    SciaRegs.SCILBAUD.all = 0x001A;


   /////////////////////////////////////////////////////////////////////////////




   /*
    * �ۼ��� ��� �� �Ǵ� �� ����.
    * baud rate�� error �� �۾ƾ� �ϴ°ɱ�???

   // LOSCLK (LOSPCP) = 200/4 = 50 MHz �� ����ؼ� Baud Rate ���.
   ///////// 230400 bps ���� (26 (1Ah) BRR): 0.4694 % Error at 50 MHz
   /// BRR: 0x001A
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x001A;
*/

   /*
    * ���� DSP ���� ������ �߻���.

   // LOSCLK (LOSPCP) = 200/4 = 50 MHz �� ����ؼ� Baud Rate ���.
   ///////// 460800 bps ���� (13 (Dh) BRR): 3.1188 % Error at 50 MHz
   /// BRR: 0x000D
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x000D;
   */


   /*
   // LOSCLK (LOSPCP) = 200/4 = 50 MHz �� ����ؼ� Baud Rate ���.
   ///////// 115200 bps ���� (53 (35h) BRR): -0.469 % Error at 50 MHz
   /// BRR: 0x35
   SciaRegs.SCIHBAUD.all = 0x0000;
   SciaRegs.SCILBAUD.all = 0x0035;
   */




   /*
   100 MHz
   BRR = 107.507 => 108
   actual Baud rate: 114678.9, 0.45 % (����)

   200 MHz
   BRR = 216.014 => 216
   actual Baud rate: 115207.3733, -0.0064% (����)
   */

   //SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
   // SCI�� �۽� FIFO ����
   SciaRegs.SCIFFTX.bit.SCIFFENA = 1;		// SCI FIFO ��� ���� Enable
   SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;	// SCI �۽� FIFO RE-enable

   // SCI�� ���� FIFO ���� with ���ͷ�Ʈ
   SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;	// SCI ���� FIFO ���ͷ�Ʈ �÷��� Ŭ����
   SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;	// SCI ���� FIFO RE-enable
   SciaRegs.SCIFFRX.bit.RXFFIENA = 1;		// SCI ���� FIFO ���ͷ�Ʈ Enable
   SciaRegs.SCIFFRX.bit.RXFFIL = 16;	// (0x10h) SCI ���� FIFO ���ͷ�Ʈ ���� ���� (16�� data�� �ԷµǸ� ���� ���ͷ�Ʈ ����)

   SciaRegs.SCIFFCT.all=0x00;

   // After a system reset, re-enable the SCI by writing a 1 to this bit.
   SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

   //SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
   //SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

}






//============================================================================================
// RS-232C ������ �ۼ��� �Լ�
//--------------------------------------------------------------------------------------------

//============================================================================================
//	Local Interrupt Service Routine : SCI ���� FIFO ���ͷ�Ʈ ���� ��ƾ
//--------------------------------------------------------------------------------------------
//	SCIFFRX ���������� RSFFST�� ���� RXFFIL�� ������ ������ ũ�ų� ������ ����.
//  SCI RXFIFO�� ���ŵ� �����͸� �޷θ��� ������.
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
		// PC�κ��� ���Ź��� ��ɾ�
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

			// Motor Stop �� ���� ����.
			bMotorRun = false;

		}
		// PC�κ��� ���Ź��� ��ɾ� (Run/Stop)
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
		// PC�κ��� ���Ź��� ��ɾ� (Run/Stop)
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





		// ���� �÷��� On
		SCI_RxFlag = true;
	}
	if(RxBuffA[0] == 'F' && RxBuffA[1] == '&' && RxBuffA[2] == 'T')
	{
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



