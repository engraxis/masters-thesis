//-----------------------------------------------------------------------------
//	������� ���� �� ����ó�� ���ñ��� ����
//-----------------------------------------------------------------------------
#ifndef __TLV5638_API_H__
#define __TLV5638_API_H__

// TLV5638 DAC IC�� �������� �� ������
#define	EXT_REF			0	// �ܺ�
#define	INT_REF_2048	1	// ���� 2.048V
#define	INT_REF_1024	2	// ���� 1.024V

//#define	REF_SEL			INT_REF_2048
#define	REF_SEL			EXT_REF



// ������ ���� ��, �����ð� ����
//#define DELAY_LENGTH	3L

//#define DELAY_LENGTH	6L

#define DELAY_LENGTH	3L


// 5kHz �� ���, 6 �� ���� ���� ���� �߻���
//#define DELAY_LENGTH	9L

//#define DELAY_LENGTH	4L


//#define DELAY_LENGTH	10L
//#define DELAY_LENGTH	11L
//#define DELAY_LENGTH	12L

//#define DELAY_LENGTH	15L



//-----------------------------------------------------------------------------
//	TLV5638 API ��� ����
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
//	TLV5638 API ��� �Լ� ����
//----------------------------------------------------------------------------- 
void TLV5638_Init(TLV5638_handle);
void TLV5638_Update(TLV5638_handle);


#endif	// __TLV5638_API_H__  


//-----------------------------------------------------------------------------
//	�� - �ڵ� ����
//----------------------------------------------------------------------------- 

