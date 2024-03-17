//-----------------------------------------------------------------------------
//	헤더파일 선언 및 선행처리 지시구문 정의
//-----------------------------------------------------------------------------
//#include "DSP2833x_Device.h"
//#include "DSP2833x_Examples.h"
#include "F2837xS_device.h"        // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"    // F2837xS Examples Include File
#include "TLV5638_API_F2837xS.h"


Uint16 SPI_Rev_dummy;
int SPI_i;

//-----------------------------------------------------------------------------
//	TLV5638 API 모듈 함수 정의
//-----------------------------------------------------------------------------
void TLV5638_Init(TLV5638 *v)
{
	// Low-speed Clock 분주비율 설정  
	//EALLOW;
	//SysCtrlRegs.LOSPCP.all = 0x0000;		// LSPCLK = SYSCLKOUT
	//EDIS;

	// GPIO 54, 55, 56, 57을 SPI 통신용으로 설정
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;		// Enable pull-up on GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;		// Enable pull-up on GPIO55 (SPISOMIA)
	GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;		// Enable pull-up on GPIO56 (SPICLKA)
	//GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;		// Enable pull-up on GPIO57 (SPISTEA)

	GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3;	// Asynchronous input GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3;	// Asynchronous input GPIO55 (SPISOMIA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3;	// Asynchronous input GPIO56 (SPICLKA)

//	GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3;	// Asynchronous input GPIO57 (SPISTEA)

	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;	// Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;	// Configure GPIO55 as SPISOMIA
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;	// Configure GPIO56 as SPICLKA
	//GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;	// Configure GPIO57 as SPISTEA

	// GPIO pin
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;	// Configure GPIO57 as SPISTEA
	// Output set
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;

	// High: DAC OFF
	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;

	EDIS;


	// SPI 통신모듈 초기화  
	SpiaRegs.SPICCR.bit.SPISWRESET = 0;	// Reset SPI  

	// 한번 shift 시 16-bit를 shift 시킴!
	//////////SpiaRegs.SPICCR.all = 0x004F;		// Data out on falling edge, 16-bit character, Loopback mode Disable


	////////////////////////
	/// Note.  1.   falling edge without delay  (Not Work) (0V 가 자주 발생함)
	///        2. falling edge with delay (Right choice)
	//			3. rising edge without delay (문제 있음...)
	//			4. rising edge with delay (문제 있음...)


	// Data is output on falling edge and input on rising edge
	SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;

	// 16-bit character, Loopback mode Disable (초기값)
	SpiaRegs.SPICCR.bit.SPICHAR = 15;

	// phase delay
	SpiaRegs.SPICTL.bit.CLK_PHASE = 1;

	// Master
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;

	// Enable Transmission
	SpiaRegs.SPICTL.bit.TALK = 1;


	// SPICLK symmetry 를 위해서는 (SPIBRR + 1) 이 even value 이어야 함.
	// For SPIBRR = 3 to 127, SPI Baud Rate = LSPCLK / (SPIBRR + 1)

	/////////////////////////
	// 50 MHz 기준


	// Baud rate, 50.0M/40 = 1.25 Mbps
	// 50 MHz 기준이면, Baud rate = 1.25 Mbps
//////	SpiaRegs.SPIBRR.all = 39;			// Baud rate, 50M/ (39+1) = 1.25 Mbps

//////////////////////////// 100 MHz 기준  //////////////////////////////////////////
	// Baud rate, 100.0M/80 = 1.25 Mbps
	// 100 MHz 기준이면, Baud rate = 1.25 Mbps
	SpiaRegs.SPIBRR.all = 79;			// Baud rate, 100M/ (79+1) = 1.25 Mbps

	//////////////////////////// 200 MHz 기준  //////////////////////////////////////////
	// Baud rate, 200.0M/128 = 1.5625 Mbps
	// 200 MHz 기준이면, Baud rate = 1.5625 Mbps
	//SpiaRegs.SPIBRR.all = 127;			// Baud rate, 200M/ (127+1) = 1.5625 Mbps


/////////////////////////////////////////////////////




	// 디버깅에서 STOP 이 발생해도, SPI통신 계속 하기
	// Free run, continue SPI operation regardless of suspend or when the suspend occurred.
	SpiaRegs.SPIPRI.all = 0x0010;		// Enable FREE
    
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;	// Enable SPI  


	// TLV5638 초기화  
	v->ContRegister = 0xD000;		// 1101(2) = Write Data to Control Register, SPD/PWR = Fast Mode/Normal Operation

	#if(REF_SEL == EXT_REF)
		v->DataRegisterA = 0x0000;	// D1/D0 = 00(0) = Set Reference Voltage to External  
	#endif

	#if(REF_SEL == INT_REF_2048)
		v->DataRegisterA = 0x0002;	// D1/D0 = 10(2) = Set Reference Voltage to 2.048V  
	#endif

	#if(REF_SEL == INT_REF_1024)
		v->DataRegisterA = 0x0001;	// D1/D0 = 01(1) = Set Reference Voltage to 1.024V  
	#endif
			
	v->Output = v->ContRegister | v->DataRegisterA;

	// ksh 수정

	//while (SpiaRegs.SPISTS.bit.BUFFULL_FLAG);

//	DELAY_US(DELAY_LENGTH);

	// activate /CS
	GpioDataRegs.GPBDAT.bit.GPIO57 = 0;

	// Transmit FIFO is now empty
	SpiaRegs.SPITXBUF = v->Output;

	while (SpiaRegs.SPISTS.bit.INT_FLAG == 0);

	//for (SPI_i=0; SPI_i < 100; SPI_i++);

	DELAY_US(DELAY_LENGTH);

	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;

	SPI_Rev_dummy = SpiaRegs.SPIRXBUF;


//	DELAY_US(DELAY_LENGTH);
	//////////////////////////////////
}

void TLV5638_Update(TLV5638 *v)
{
	// DAC A & B 동시 출력

	/////////////////////////////////////////////////////////
	//////////		DAC B Part
	//////////////////////////////////////////////////////////////


	// DAC B: Write data to BUFFER
	// 0001
	v->ContRegister = 0x1000;		// 0001(2) = Write Data to Buffer
	//v->ContRegister = 0x5000;		// 0101(2) = Write Data to Buffer


	// 상위 4비트는 0 으로 만들기.
	//v->DataRegisterB = v->DataRegisterB & 0x0FFF;
	v->DataRegisterB &= 0x0FFF;
					
	v->Output = v->ContRegister | v->DataRegisterB;


////////////////////////////////////////////////////////////////////

	// activate /CS
	GpioDataRegs.GPBDAT.bit.GPIO57 = 0;

	// DAC B
	SpiaRegs.SPITXBUF = v->Output;


	while (SpiaRegs.SPISTS.bit.INT_FLAG == 0);

	//for (SPI_i=0; SPI_i < 100; SPI_i++);

	DELAY_US(DELAY_LENGTH);

	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;

	SPI_Rev_dummy = SpiaRegs.SPIRXBUF;



	////////////////////////////////////////////////////////////////
	////////		DAC A Part  (A & B 동시 갱신)
	////////////////////////////////////////////////////////////////

	// Write data to DAC A and update DAC B with BUFFER content
	// 1000
	v->ContRegister = 0x8000;		// 1000(2) = Write Data to DAC A and Update DAC B with Buffer Content
	//v->ContRegister = 0xC000;		// 1100(2) = Write Data to DAC A and Update DAC B with Buffer Content


	// 상위 4bit 는 0으로 처리
	v->DataRegisterA &= 0x0FFF;

	v->Output = v->ContRegister | v->DataRegisterA;


	////////////////////////////////////////////////////////////////////////////
	// activate /CS
	GpioDataRegs.GPBDAT.bit.GPIO57 = 0;
	// DAC A & B
	SpiaRegs.SPITXBUF = v->Output;
	while (SpiaRegs.SPISTS.bit.INT_FLAG == 0);

	//for (SPI_i=0; SPI_i < 100; SPI_i++);
	DELAY_US(DELAY_LENGTH);

	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;
	SPI_Rev_dummy = SpiaRegs.SPIRXBUF;
	////////////////////////////////////////////////////////////////////////////

/*
	// 한번 더 !!! (만약 0V 문제가 생길시, 0V 지속 시간을 줄일 수 있음.)
	////////////////////////////////////////////////////////////////////////////
		// activate /CS
		GpioDataRegs.GPBDAT.bit.GPIO57 = 0;
		// DAC A & B
		SpiaRegs.SPITXBUF = v->Output;
		while (SpiaRegs.SPISTS.bit.INT_FLAG == 0);
		for (SPI_i=0; SPI_i < 100; SPI_i++);
		GpioDataRegs.GPBDAT.bit.GPIO57 = 1;
		SPI_Rev_dummy = SpiaRegs.SPIRXBUF;
		////////////////////////////////////////////////////////////////////////////
*/
}


//-----------------------------------------------------------------------------
//	끝 - 코드 종료
//----------------------------------------------------------------------------- 


