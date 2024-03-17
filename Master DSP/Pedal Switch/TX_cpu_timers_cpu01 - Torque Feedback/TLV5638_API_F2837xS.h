//-----------------------------------------------------------------------------
//	헤더파일 선언 및 선행처리 지시구문 정의
//-----------------------------------------------------------------------------
#ifndef __TLV5638_API_H__
#define __TLV5638_API_H__

// TLV5638 DAC IC의 참조전압 원 설정용
#define	EXT_REF			0	// 외부
#define	INT_REF_2048	1	// 내부 2.048V
#define	INT_REF_1024	2	// 내부 1.024V

//#define	REF_SEL			INT_REF_2048
#define	REF_SEL			EXT_REF



// 데이터 전송 후, 지연시간 정의
//#define DELAY_LENGTH	3L

//#define DELAY_LENGTH	6L

#define DELAY_LENGTH	3L


// 5kHz 의 경우, 6 은 아주 가끔 문제 발생함
//#define DELAY_LENGTH	9L

//#define DELAY_LENGTH	4L


//#define DELAY_LENGTH	10L
//#define DELAY_LENGTH	11L
//#define DELAY_LENGTH	12L

//#define DELAY_LENGTH	15L



//-----------------------------------------------------------------------------
//	TLV5638 API 모듈 정의
//-----------------------------------------------------------------------------
typedef struct {
	Uint16 ContRegister;	// 4-Bit Control  
	Uint16 DataRegisterA;	// 12-Bit Data for Channel A  
	Uint16 DataRegisterB;	// 12-Bit Data for Channel B  
	Uint16 Output;
	void  (*init)();
	void  (*update)();
} TLV5638;

typedef TLV5638 *TLV5638_handle;

#define TLV5638_DEFAULTS { 0, \
                           0, \
                           0, \
                           0, \
                           (void (*)(Uint32))TLV5638_Init, \
                           (void (*)(Uint32))TLV5638_Update }


//-----------------------------------------------------------------------------
//	TLV5638 API 모듈 함수 선언
//----------------------------------------------------------------------------- 
void TLV5638_Init(TLV5638_handle);
void TLV5638_Update(TLV5638_handle);


#endif	// __TLV5638_API_H__  


//-----------------------------------------------------------------------------
//	끝 - 코드 종료
//----------------------------------------------------------------------------- 

