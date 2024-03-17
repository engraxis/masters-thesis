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

// Motor Enable ON/OFF ����� ��� ���� ----------------------------------------------
#define MOTOR_ON		GpioDataRegs.GPBSET.bit.GPIO33=1	// Motor ON
#define MOTOR_OFF		GpioDataRegs.GPBCLEAR.bit.GPIO33=1	// Motor OFF
#define TWIST_MOTOR_ON	GpioDataRegs.GPBSET.bit.GPIO34=1
#define TWIST_MOTOR_OFF	GpioDataRegs.GPBCLEAR.bit.GPIO34=1
// -----------------------------------------------------------------------------------

#define START_MOTOR_CONTROL GpioDataRegs.GPBDAT.bit.GPIO43;
#define STOP_MOTOR_CONTROL	GpioDataRegs.GPBDAT.bit.GPIO42;

// �ݿø� ��ũ�� �Լ� (�Ҽ��� ù°�ڸ�)-----------------------------------------------
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
Uint16 SCI_RxFlag;
Uint16 SCIB_RxFlag;
Uint16 i;

Uint16 SCI_cnt, SCI_cnt1;

// 2��° �۽��� ���� flag ����


//////////////////////////////////////////////////
/////////////		variables for data communication between DSP and PC
//////////////////////////////////////////////////////////////////////////////
//	Ư�� float data �� ���ؼ�.
#define NUM_OF_BUF	16
// float -> Uint32 ������ ���� (�۽ſ�)

// 8�� �۽��� data �ӽ� �����
float fTempTxBuf[8];

// TEST variables
Uint32 test_Buf[NUM_OF_BUF];
Uint32 test_BufB[NUM_OF_BUF];
Uint32 test_Rev_Buf[NUM_OF_BUF];
Uint32 test_Rev_BufB[NUM_OF_BUF];
Uint32 MasterPos[2];

/////////////////////////////////////////////////////////////////////////
/////////				DAC (TLV5638)
////////////////////////////////////////////////////////////////////////
// DAC (TLV5638): SPI ������� ����
#include "TLV5638_API_F2837xS.h"

Uint16	BackTicker;
Uint16	TestDataA;
Uint16	TestDataB;
TLV5638 DAC = TLV5638_DEFAULTS;




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
float32 fSL_CurrentPos[2];

// ������ ȸ���� ���� �ӽ� ����
float32 fTransRad;


////////////////////////////////////
///////////		��ġ ����ġ		////////
Uint16 bInput_TouchSW;		// ��ġ ����ġ �Է�




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
#define TORQUE_TO_X1(TORQUE1)	(((1.0*(TORQUE1))-((fKt[1])*(fG_Ia[1])*(fb[1])))/((fKt[1])*(fG_Ia[1])*(fa[1])))

// �������� Force => Torque ��ȯ
#define TRANS_FOR_TO_TOR(FORCE0)		((FORCE0)*(TRANS_R))

// �������� Force
float32 fTrans_Force;




// Motor Torque
float32 fMotorTorque[2];

// Motor Enable flag
//bool bMotorEnable;
Uint16 bMotorEnable;

// Motor Run/Stop flag
//bool bMotorRun;
Uint16 bMotorRun;




// Time variable
float fControl_Time;




/////////////////////////

///////////////////////////////////////////////////
///////////SLAVE VARIABLES/////////////////////////
// PID Loop
float32 fSL_Error[2];
float32 fSL_AccError[2];
float32 fSL_PreError[2];
float32 fSL_ChangeError[2];
float32 fSL_PID_Kp[2];
float32 fSL_PID_Ki[2];
float32 fSL_PID_Kd[2];
float32 fSL_PID_OP[2];
// Globally defined
#define TRANS_SL_R 48 //mm
#define fSL_ArcGain	48 //48mm approx.
#define TRANS_FOR_TO_TOR_SL(FORCE0)		((FORCE0)*(TRANS_SL_R))
#define SL_TRANS_POS(SL_RAD0)		    (fSL_ArcGain*SL_RAD0)


//float32 transfortotorsl;
//float32		torquetox0;
///////////////////////////////////////////////////

Uint16 bHandShake;
Uint16 bStartMotorControl;
Uint16 bStopMotorControl;



float32 fMasterPos[2];
Uint32 xa;
Uint32 manualswitch;
float32 spos;
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


	GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(42, GPIO_INPUT, GPIO_QUAL6);
	GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_QUAL6);

	/////////////////////////////////////////////////////
	/////////		EQEP A & B ��Ʈ ����		/////////////
	////////////////////////////////////////////////////

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

			test_Buf[i] = 0;


			// TEST
			test_Rev_Buf[i] = 0;

		}
		for(i=0; i < NUM_OF_BUF; i++)
		{
			test_Rev_BufB[i] = 0;
		}

		for(i=0; i<NUM_OF_BUF; i++)
		{
			test_BufB[i] = 0;
		}

		for (i = 0; i < 8; i++)
			// 8�� �۽��� data �ӽ� �����
			fTempTxBuf[i] = 0.0;


		// 2��° �۽��� ���� flag ����














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



	}
bHandShake = false;
bStartMotorControl = false;
bStopMotorControl = false;
	fTransRad = 0.0;
xa = 2043;
manualswitch = 0;
	////////////////////////////////
	//// ��ġ ����ġ
	////////////////////
	bInput_TouchSW = false;

	fMasterPos[0] = 0.0;
	fMasterPos[1] = 0.0;

	///////////////////////////////////////////////////
	///////////SLAVE VARIABLES/////////////////////////
	fSL_CurrentPos[0] = 0.0;
	fSL_CurrentPos[1] = 0.0;

	// PID Loop
	for(i=0;i<2;i++)
	{
		fSL_Error[i] = 0;
		fSL_AccError[i] = 0;
		fSL_PreError[i] = 0;
		fSL_ChangeError[i] = 0;
	}

	fSL_PID_Kp[0] = 0.4;
	fSL_PID_Ki[0] = 0.5;
	fSL_PID_Kd[0] = 0.003;
	fSL_PID_OP[0] = 0;

	fSL_PID_Kp[1] = 0.05;
	fSL_PID_Ki[1] = 0.03;
	fSL_PID_Kd[1] = 0.009;
	fSL_PID_OP[1] = 0;
	///////////////////////////////////////////////////










	//////////////////////////////////////////////////////////
	///////			���� ���� ������		////////////////////////
	///////////////////////////////////////////////////////////
	// ���� & Twisting

	// DSP value (X: 0 ~ 4095) to bipolar 10V DAC output (Vo: motor control input)
	// Vo = aX + b

	// DSP to DAC parameters
	// ������ (a: 0.004898, b: -9.9971)
	fa[0] = 0.004886;
	fb[0] = -9.981;
	fa[1] = 0.004894;
	fb[1] = -9.983;

	// Voltage to current relationship
	// ������: Old motor: 12/10 = 0.12, New motor: 12/4
	fG_Ia[0] = 3; //Translation

	// Twisting: 3.48/10 = 0.348
	fG_Ia[1] = 0.348;

	// Torque constant
	// ������: Old motor: 29.2 [mNm/A], New motor: 13.9
	fKt[0] = 13.9;	// [mNm/A] // Translation

	// Twisting: 23.4 [mNm/A]
	fKt[1] = 23.4;		// [mNm/A]


	// Motor Enable: Disabled
	//bMotorEnable = false;
	bMotorEnable = 0;

	// Motor Run/Stop flag
	bMotorRun = false;






	// circular helix ��ũ ���� ����






	// Time variable
	fControl_Time = 0.0;








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

	TestDataA = ROUND(TORQUE_TO_X0(0.0));		// actual: 2041
	TestDataB = ROUND(TORQUE_TO_X1(0.0));		// actual: 2041


	// SPI ������� motor torque ���� (0.0 ��ũ ����)
	DAC.DataRegisterA = 2043;
	DAC.update(&DAC);



	fMotorTorque[0] = 0.0;
	fMotorTorque[1] = 0.0;


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
	//CpuTimer0.InterruptCount++;	// ���ͷ�Ʈ Ƚ�� ���: ���ʿ���.

bStartMotorControl = START_MOTOR_CONTROL;
bStopMotorControl = STOP_MOTOR_CONTROL;

if(bStartMotorControl)
{
	manualswitch = 1;	//For PID Contorl and Analog Data
	bMotorEnable = 1;	//HighSignal to Digital Pin of ESCON
}
if(bStopMotorControl)
{
	manualswitch = 0;
	bMotorEnable = 0;
}


	if(!bHandShake)
	{
		while(ScibRegs.SCIFFTX.bit.TXFFST != 0);
		ScibRegs.SCITXBUF.all = 'H';
		ScibRegs.SCITXBUF.all = 'S';
		ScibRegs.SCITXBUF.all = 'T';
		for(i=3;i<16;i++)
		{
			ScibRegs.SCITXBUF.all = '*';
		}
		bHandShake = true;
	}




if(bHandShake)
{




		GREEN_LED_ON;

	// ���� �ֱ� üũ!
//	RED_LED_TOGGLE;

	// Time variable (Since 4 kHz �� �����ϹǷ�!)
	fControl_Time += 0.00025;


	// Motor Enable GPIO 33�� ��� ����
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
	if (SCIB_RxFlag == true) //For SCI-B
	{
		for(i=0;i<RxFifoLevel;i++)
		{
			test_Rev_BufB[i] = RxBuffB[i] & 0x000000FF;
		}
		SCIB_RxFlag = false;
	}

	if(test_Rev_BufB[0] == 'a' && test_Rev_BufB[4] == 'b') //Extracting Linear Motion Position
	{
		if( (test_Rev_BufB[1] & 0x00000080) == 0x00000080)
		{
			MasterPos[0] = 0xFF000000;
			MasterPos[0] |= (test_Rev_BufB[1] << 16);
			MasterPos[0] |= (test_Rev_BufB[2] << 8);
			MasterPos[0] |= (test_Rev_BufB[3]);
		}
		else
		{
			MasterPos[0] = 0;
			MasterPos[0] = (test_Rev_BufB[1] << 16);
			MasterPos[0] |= (test_Rev_BufB[2] << 8);
			MasterPos[0] |= (test_Rev_BufB[3]);
		}
		if( (test_Rev_BufB[5] & 0x00000080) == 0x00000080)
		{
			MasterPos[1] = 0xFF000000;
			MasterPos[1] |= (test_Rev_BufB[5] << 16);
			MasterPos[1] |= (test_Rev_BufB[6] << 8);
			MasterPos[1] |= (test_Rev_BufB[7]);
		}
		else
		{
			MasterPos[1] = 0;
			MasterPos[1] = (test_Rev_BufB[5] << 16);
			MasterPos[1] |= (test_Rev_BufB[6] << 8);
			MasterPos[1] |= (test_Rev_BufB[7]);
		}
	}

	fMasterPos[0] = -1.0*(((float)(long)MasterPos[0]) * 0.001 * 10.0);
	fMasterPos[1] = -1.0*((((float)(long)(MasterPos[1])) * 0.001)*10.0);

	spos = fMasterPos[1] ;


	//////////////////////////////////////////////////////////////////
	/////		Receive data or command from PC.
	//////////////////////////////////////////////////////////////////

	if (SCI_RxFlag == true) //For SCI-A
	{
		// ������ test
		// ������ data�� �ٽ� �۽���
		//---------------------------------------------------------------------------
		//			���ŵ� Data Processing!
		//		---------------------------------------------------------------------------/
		for(i=0;i<RxFifoLevel;i++)
		{
			test_Rev_Buf[i] = RxBuffA[i];
		}

		// for�� Ǯ���

		SCI_RxFlag = false;
	}

	//////////////////////////////////////////////////////////////
	/////////	Read Sensors
///////////////////////////////////////////////////////////////////////////////////

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

	// ���� ���� ��ġ ��� [deg]
	// ���� ������ ���߱� ���ؼ�, ������ ��ġ ������ �ٲ�.
	//fCurrentPos[0] = -(RotateAngleDegree[0] + 360.0*NumOfTurns[0]);
	//fCurrentPos[0] = -(RotateAngleRad[0] + 2*PI*NumOfTurns[0]);
	fCurrentPos[0] = -(RotateAngleUnit[0] + (float)NumOfTurns[0])*2*PI;

	// TEST
	fTransRad = fCurrentPos[0];

	// [mm]�� ���� ���� ���
//	fCurrentPos[0] = TRANS_POS(fCurrentPos[0]);

	fSL_CurrentPos[0] = SL_TRANS_POS(fCurrentPos[0]);

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
	fSL_CurrentPos[1] = fCurrentPos[1];


	//////////////////////// ���� ��Ʈ ///////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	// Commanding the Slave to Follow Master
if(manualswitch)
{
	//PID Loop for Slave Translation to follow Master Translation
	fSL_Error[0] = fMasterPos[0]  -  fSL_CurrentPos[0];
	fSL_AccError[0] = fSL_Error[0] + fSL_AccError[0];
	fSL_ChangeError[0] = fSL_Error[0] - fSL_PreError[0];

	fSL_PID_OP[0] = (fSL_PID_Kp[0] * fSL_Error[0]) + (fSL_PID_Ki[0] * fSL_AccError[0] * Ts) + (fSL_PID_Kd[0] * fSL_ChangeError[0] * CONTROL_HZ);
	TestDataA = TORQUE_TO_X0( TRANS_FOR_TO_TOR_SL(fSL_PID_OP[0]) );
	DAC.DataRegisterA = TestDataA;
	DAC.update(&DAC);



	//PID Loop for Slave Twisting to follow Master Twisting
	fSL_Error[1] = fMasterPos[1]  -  fSL_CurrentPos[1];
	fSL_AccError[1] = fSL_Error[1] + fSL_AccError[1];
	fSL_ChangeError[1] = fSL_Error[1] - fSL_PreError[1];

	fSL_PID_OP[1] = (fSL_PID_Kp[1] * fSL_Error[1]) + (fSL_PID_Ki[1] * fSL_AccError[1] * Ts) + (fSL_PID_Kd[1] * fSL_ChangeError[1] * CONTROL_HZ);
	TestDataB = TORQUE_TO_X1(TRANS_FOR_TO_TOR_SL(fSL_PID_OP[1]));
	xa = TestDataB;
	DAC.DataRegisterB = TestDataB;
	DAC.update(&DAC);


}
else
{
	DAC.DataRegisterA = 2043;
	DAC.DataRegisterB = 2043;
	DAC.update(&DAC);
	}








		// 12 bit DAC
		// ��� ���� [V] = 2 * REF * CODE / 0x1000
		// CODE: 0x000 ~ 0xFFF
		// x [V] = x * 10^-3
		//	DAC.DataRegisterA = TestDataA++;

		// DAC output: 2 V
		//DAC.DataRegisterA = 2048;


//////////////////	// ���� ������ ���߱� ���ؼ� Twisting ��ũ ��ȣ �ٲٱ�.
	//TestDataB = ROUND(TORQUE_TO_X1(-1.0*fMotorTorque[1]));		// actual: 2041

	DAC.DataRegisterA = TestDataA;
	DAC.DataRegisterB = TestDataB;

	// �ִ� ��ũ
	// ������: 317.112 [mNm]
	// Twisting: 96.18 [mNm]


	// SPI ������� motor torque ����
//	DAC.update(&DAC);



	// SCI-A ��� �ֱ�� ī����
	++SCI_cnt;
	++SCI_cnt1;

	// Save Data �۽� Index; (�ʸ����� ����: 1 kHz �� ���, 100��)




	// 1�ʿ� 1�� �۽� ������
	//if (!(SCI_cnt % CONTROL_HZ))
		// 1�ʿ� 10�� �۽� ������
		//if (!(SCI_cnt % 400))
		// 1�ʿ� 100�� �۽�: (100 Hz)
		//if (!(SCI_cnt % 50))

	// 4 kHz�� �۽��ϱ�! Test
	// 921600 bps: 4kHz ���� �ֱ� ������. (Good)
	// 460800 bps: 4kHz ���� �ֱ� ���� �ȵ�. �� 1.3 kHz �ֱⰡ ���Ƿο� ����. ��, �� 2.6 kHz ������ �ֱ�� ������ �� ����.
	//if (true)

		// 1 kHz�� ������. ��, 1�ʿ� 1000�� ������ (���� 4kHz ���� �ֱ� => 1�ʿ� 4õ��)
		// 921600 bps�δ� 4kHz ������. (Good)
	if (!(SCI_cnt % 4))

		// 800 Hz�� ������. (4 kHz �����ֱⰡ ���� �ȵ�.)
		// 115200 �ӵ��ε� 4kHz ���� �ȵ�.
		// 921600 bps�δ� 4kHz ������. (Good)
		//if (!(SCI_cnt % 5))

		// ���� �ְ� �۽� �ӵ�: 115200 �ӵ��� 4 kHz ���� �ֱ� ������.
		//if (!(SCI_cnt % 9))

		// 500 Hz�� ������.
		// 115200 �ӵ��δ� 4kHz ������.
//	if (!(SCI_cnt % 8))

		// 400 Hz
		//if (!(SCI_cnt % 10))

		// 250 Hz (38400 bps: 4kHz ���� �ֱⰡ ���� �ȵ�.)
		// (115200 ������ 4kHz ������.)
		//if (!(SCI_cnt % 16))

		// 200 Hz (4kHz ���� �ֱⰡ ������.)
	//if (!(SCI_cnt % 20))
	{
		// Reset ī����
		SCI_cnt = 0;



			fTempTxBuf[1] = fSL_CurrentPos[0]*0.1;


			//fTempTxBuf[4] = fMotorTorque[0];
			fTempTxBuf[4] = fTrans_Force*0.1;			// ���� Force
			fTempTxBuf[5] = fCurrentPos[1]*0.1;




		// Sending only the position information of Linear and Rotary Motion
		test_BufB[0] = (long)(1000 * fTempTxBuf[1]);
		test_BufB[1] = (long)(1000 * fTempTxBuf[5]);
		// Translation Motion Position
		TxBuffB[0] = 'a';
		TxBuffB[1] = (test_BufB[0] >> 16) & 0x00FF;
		TxBuffB[2] = (test_BufB[0] >> 8) & 0x00FF;
		TxBuffB[3] = test_BufB[0] & 0x00FF;
		// Rotation Motion Position
		TxBuffB[4] = 'b';
		TxBuffB[5] = (test_BufB[1] >> 16) & 0x00FF;
		TxBuffB[6] = (test_BufB[1] >> 8) & 0x00FF;
		TxBuffB[7] = test_BufB[1] & 0x00FF;

		//---------------------------------------------------------------------------
		//		A. SCI ������ �۽�
		//	---------------------------------------------------------------------------//
		// Trnasmit FIFO�� empty �� �ɶ�����, ���!
		while(SciaRegs.SCIFFTX.bit.TXFFST != 0);

		// Transmit FIFO �� empty �̸�, �۽��� �����Ѵ�.
		for(i=0;i<RxFifoLevel;i++)
		{
			//SciaRegs.SCITXBUF.all = TxBuffA[i] & 0x00FF;//Sehun
			SciaRegs.SCITXBUF.all = TxBuffB[i] & 0x00FF;
		}


		// Sending Slave Position to Master
		while(ScibRegs.SCIFFTX.bit.TXFFST != 0);
		for(i=0;i<RxFifoLevel;i++)
		{
			ScibRegs.SCITXBUF.all = TxBuffB[i] & 0x00FF;
		}




	// SCI ��� if �� End
	///////////////////////////////////////////////////////
	}


	fSL_PreError[0] = fSL_Error[0];
	fSL_PreError[1] = fSL_Error[1];
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
	// 16 ���� data�� FIFO�� ����, ���ͷ�Ʈ ����
	for(i=0;i<RxFifoLevel;i++)
	{
		RxBuffA[i] = SciaRegs.SCIRXBUF.bit.SAR;
	}


		SCI_RxFlag = true;





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

	for(i=0;i<RxFifoLevel;i++)
	{
//		TxBuffB[i] = RxBuffB[i];
	}

	SCIB_RxFlag = true;

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    //////////////////////////////////////////////////////////////////////
    //PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;			// Acknowledge interrupt to PIE
}



