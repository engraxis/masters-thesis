#include "stdafx.h"

/*
//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////		�ٿ���� �� �ڵ��� ������!
///////////////////////////////////////////////////////////
// 1. �׳� Queue�� ���� �ȵ�. ��� ä��⸸ �ϱ� ������, ���۰� �� ����, �޸� ������ �߻���.

	�ذ� ���: Circular Queue �� ����ؾ� ��.


*/



//---------------------------- ��� ���� --------------------------//
//#define	BUFF_SIZE		8196
//#define	BUFF_SIZE		1000
//#define	BUFF_SIZE		8000
//#define	BUFF_SIZE			10000		// ���� circular queue ���� ������
#define	BUFF_SIZE			15000		// ���� circular queue ���� ������


#define	WM_COMM_READ	(WM_USER+1)		

#define	ASCII_LF		0x0a
#define	ASCII_CR		0x0d
#define	ASCII_XON		0x11
#define	ASCII_XOFF		0x13


#define READ_TIMEOUT            500
#define STATUS_CHECK_TIMEOUT    500
#define WRITE_CHECK_TIMEOUT     500


// TEST
#include "Parameters.h"



// Circular Queue Ŭ���� ����
class	CQueue
{
private:
	int front, back, buf_size, count;
	// ��ſ��̹Ƿ�, BYTE ������ �������� ���� �ۼ�
	BYTE *array;
public:
	CQueue();
	~CQueue();
	BOOL PutByte(BYTE b);		// 1 byte �ֱ�
	BOOL GetByte(BYTE *pb);		// 1 byte ������
	//BYTE GetByte();		// 1 byte ������
	int	GetSize();
	

/*
public:
	BYTE	buff[BUFF_SIZE];
	int 	m_iHead, m_iTail;
	CQueue();
	void	Clear();
	int	GetSize();
	BOOL	PutByte(BYTE b);		// 1 byte �ֱ�
	BOOL	GetByte(BYTE *pb);		// 1 byte ������
*/
};


//	��� Ŭ����	CCommThread 

// ��Ʈ ���� : OpenPort("COM1", CBR_9600);
// ��Ʈ���� �б� :
//   ��Ʈ�� �� �Ŀ� ��Ʈ�� �ڷᰡ �����ϸ� WM_COMM_READ �޽����� ���� 
//   �����쿡 ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿�, �Լ��� �����ϰ�
//   m_ReadData String�� ����� �����͸� �̿� �б�
// ��Ʈ�� ���� : WriteComm(buff, 30)�� ���� ���ۿ� �� ũ�⸦ �ǳ׸� �ȴ�.

class	CCommThread
{
public:
	CCommThread();
	//--------- ȯ�� ���� -----------------------------------------//
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	BOOL		m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// WM_COMM_READ�� �Բ� ������ �μ�.
	DWORD		m_dwRemainedRX;			// ���źο� �����ִ� byte ��
	//BOOL		bWriteRes;				// �۽�

	BOOL        m_bReceivedAll;			// data�� ��� �ޱ� ���ؼ�
	BOOL		m_bWaitingOnRead;		// ������ ���������� ��� flag

	//--------- ��� ���� -----------------------------------------//
	CQueue	m_QueueRead;

	//--------- �ܺ� ��� �Լ� ------------------------------------//
	BOOL OpenPort(CString m_stPort, CString m_stBaud,CString m_stDatabit,
						   	CString m_stFlow,CString m_stParity,CString m_stStop);
	void	ClosePort();
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);

	//--------- ���� ��� �Լ� ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);

	//--------- �޼��� ���� Ȯ�ο� ------------------------------------//
	BOOL m_bReserveMsg;

	//--------- �޼��� ���� Ÿ�� ------------------------------------//
	HWND hCommWnd;

	CStringArray* GetPorts();
};

// Thread�� ����� �Լ� 
DWORD	ThreadWatchComm(CCommThread* pComm);





