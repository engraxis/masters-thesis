
#include "CommThread.h"

#include "Data_Save.h"

#include "Parameters.h"


// ���� ������ ���� ����
double Buf_Data[NUM_OF_DATA];



//////////////////////////////////////////////////////////////////////////
// test variable
int comm_test1, comm_test2, comm_test3, comm_test4, comm_test5, comm_test6, comm_test7, comm_test8, comm_test9, comm_test10, comm_test11;



// �޼����� ���� ������ �ڵ�, �θ� �����쿡�� HWND hCommWnd= this->m_hWnd��
// ������ �ش�.


// CQueue ������========================================= 

// Queue�� ������
CQueue::CQueue()
{
	front = back = -1;

	// size�� �̹� -1�� ���� ������.
	CQueue::buf_size = BUFF_SIZE - 1;

	array = new BYTE[BUFF_SIZE];
	count = 0;

	for(int i = 0; i < BUFF_SIZE; i++) {
		array[i] = 0;
	}
		
}

// Queue�� �Ҹ���
CQueue::~CQueue()
{
	// ���� �޸� ����
	delete [] array;
}

/*
// Queue�� �ʱ�ȭ
void CQueue::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff, 0, BUFF_SIZE);
}

// Queue�� ��� �ִ� �ڷ� ����.
int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}
*/

// Queue�� 1 byte ����.
BOOL CQueue::PutByte(BYTE item)
{
	// ���� �� ť�� ��� �ִ� ť�� �����ϱ� ����, �̰� ���� ���� �;� ��.
	if (front == 0 && back == buf_size || front == back + 1) {
		//cout << "Queue is full\n";
		return FALSE;
	}
	// �ʱ� ���� �ܰ�
	else if (front == -1 && back == -1) {
		front = 0;
		back = 0;
		array[back] = item;
		count++;
	}
	// ���� ť�� �ǵ��� �ϴ� �ܰ�
	else if (back == buf_size) {
		back = 0;
		array[back] = item;
		count++;
	}
	// �Ϲ����� ���� �ܰ�
	else {
		back++;
		array[back] = item;
		count++;
	}

	/*
	if (GetSize() == (BUFF_SIZE-1)) return FALSE;
	buff[m_iHead++] = b;
	m_iHead %= BUFF_SIZE;
	*/

	return TRUE;
}

// Queue���� 1 byte ����.
BOOL CQueue::GetByte(BYTE* pb)
//BYTE CQueue::GetByte()
{
	// ��� �ִ� ť��. (���� �ܰ�)
	if (front == -1 && back == -1) {
		//cout << "Queue is empty\n";
		return FALSE;
	}
	else {
		// data�� ������ ��
		if (front == back) {
			// data ������
			*pb = array[front];
			
			front = -1;
			back = -1;
						
			count--;
			
			// ����
			return FALSE;
		}
		// ���� ť ����� �κ�
		else if (front == buf_size) {
			// data ������
			*pb = array[front];

			front = 0;
			count--;
		}
		// �Ϲ����� dequeue (data ������)
		else {

			// data ������
			*pb = array[front];
						
			front++;
			count--;
		}
	}

/*
	if (GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= BUFF_SIZE;
*/

	return TRUE;
}


// Queue�� ��� �ִ� �ڷ� ����.
int CQueue::GetSize()
{
	//return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
		

	return count;
}



// ��Ʈ sPortName�� dwBaud �ӵ��� ����.
// ThreadWatchComm �Լ����� ��Ʈ�� ���� ������ �� MainWnd�� �˸���
// ���� WM_COMM_READ�޽����� ������ ���� ���� wPortID���� ���� �޴´�.

BOOL CCommThread::OpenPort(CString m_stPort,CString m_stBaud,CString m_stDatabit,
						   	CString m_stFlow,CString m_stParity,CString m_stStop)
{
	// Local ����.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// ���� �ʱ�ȭ


	// overlapped structure ���� �ʱ�ȭ.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (! (m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))	
		return FALSE;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		return FALSE;
	
	// ��Ʈ ���� (Overlapped: �񵿱� ���)
	m_hComm = CreateFile( m_stPort, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);

	// error check: error opening port; abort
	if (m_hComm == INVALID_HANDLE_VALUE) return FALSE;

	// ��Ʈ ���� ����.

	// EV_RXCHAR event ����
//	SetCommMask( m_hComm, EV_RXCHAR);	

//	WaitCommEvent( m_hComm, NULL, NULL);// ********
	// InQueue, OutQueue ũ�� ����.
	//SetupComm( m_hComm, 4096, 4096);	
	if(!SetupComm( m_hComm, BUFF_SIZE, BUFF_SIZE))
	{
		// Error
		sprintf_s(str_TEST3, size_buf, "Error SetupComm(%d)!", GetLastError());
	}

	// ��Ʈ ����.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	/////////////////
	// Setting the members of the COMMTIMEOUTS structure in the following manner causes read operations
	// to complete immediately without waiting for any new data to arrive

	// �ֳ��ϸ�, data ���� ���θ� �̸� �� �� �����Ƿ�!!!

	// timeout ����.
	GetCommTimeouts( m_hComm, &timeouts);

	// A value of MAXDWORD, combined with zero values for both the ReadTotalTimeoutConstant
	// and ReadTotalTimeoutMultiplier members, specifies that the read operation is to return
	// immediately with the bytes that have already been received, even if no bytes have been received.

	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	//timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / atol(m_stBaud);
	//timeouts.WriteTotalTimeoutMultiplier = 2*CBR_115200 / atol(m_stBaud);
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	// A value of zero for both the WriteTotalTimeoutMultiplier and WriteTotalTimeoutConstant members
	// indicates that total time-outs are not used for write operations.
	
	SetCommTimeouts( m_hComm, &timeouts);

	// dcb ����
	FillMemory(&dcb, sizeof(dcb), 0);

	//dcb.DCBlength = sizeof(DCB);
	GetCommState( m_hComm, &dcb);	// ���� ���� ����.
	dcb.BaudRate = atol(m_stBaud);
	dcb.ByteSize = atoi(m_stDatabit);
	dcb.Parity = m_stParity.GetLength()-2;
	dcb.StopBits = static_cast<int>(atof(m_stStop)*2-2);


	if (m_stFlow.Compare("XON/XOFF")==0) 
	{
		dcb.fInX = dcb.fOutX = 1;		// Xon, Xoff ���.
	}
	else
	{
		dcb.fInX = dcb.fOutX = 0;
	}
	if (m_stFlow.Compare("HARDWARE")==0)
	{
		dcb.fOutxCtsFlow = 1;
		dcb.fOutxDsrFlow = 1;
		dcb.fDtrControl  = 2;
		dcb.fRtsControl  = 2;
	}
	else
	{
		dcb.fOutxCtsFlow = 0;
		dcb.fOutxDsrFlow = 0;
		dcb.fDtrControl  = 1;
		dcb.fRtsControl  = 1;
	}

	dcb.XonChar = ASCII_XON;
	dcb.XoffChar = ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;
	if (! SetCommState( m_hComm, &dcb))	return FALSE;

	// ��Ʈ ���� ������ ����.
	m_bConnected = TRUE;
	m_hThreadWatchComm = CreateThread( NULL, 0, 
		(LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);
	if (! m_hThreadWatchComm)
	{
		CloseHandle(m_hThreadWatchComm);

		ClosePort();
		return FALSE;
	}

	return TRUE;
}
	
// ��Ʈ�� �ݴ´�.
void CCommThread::ClosePort()
{


	m_bReceivedAll = false;

	

	m_bConnected = FALSE;
	SetCommMask( m_hComm, 0);
	PurgeComm( m_hComm,					
		PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	////////////
	if (m_hThreadWatchComm)
		CloseHandle(m_hThreadWatchComm);

	if (m_osRead.hEvent)
		CloseHandle(m_osRead.hEvent);

	if (m_osWrite.hEvent)
		CloseHandle(m_osWrite.hEvent);
	
	/////////////////
	if (m_hComm)
		CloseHandle( m_hComm);
}

// ��Ʈ�� pBuff�� ������ nToWrite��ŭ ����.
// ������ ������ Byte���� �����Ѵ�.
DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;
	BOOL bRes;
	DWORD dwRes;

	if (! WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	// 1��° �μ�: ��� ������ �ڵ�
	// 2��° �μ�: �����Ͱ� ��� �ִ� ����
	// 3��° �μ�: ������ �ϴ� byte ��
	// 4��° �μ�: ������ ������ ����Ʈ ���� return�ϱ� ���� DWORD ������ ������
	// 5��° �μ�: �񵿱� ������� �� �� ����ϴµ�, �񵿱� ������� ���� ���� ���� �ݵ�� NULL�� �����ؾ� ��.

	// ����: return value is nonzero (TRUE)
	// ���� or �񵿱� ���:  return value is zero (FALSE)
	//if (! WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, NULL))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			// WriteFile failed, but it isn't delayed. Report error.
			bRes = true;
		}
		else
		{
			// Write is pending.
			dwRes = WaitForSingleObject(m_osWrite.hEvent, INFINITE);
			switch(dwRes)
			{
				// Overlapped event has been signaled. 
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(m_hComm, &m_osWrite, &dwWritten, FALSE))
				{
					bRes = FALSE;

					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}

				}
				else {
					if (dwWritten != nToWrite) {
						// The write operation timed out. I now need to 
						// decide if I want to abort or retry. If I retry, 
						// I need to send only the bytes that weren't sent. 

						// ���߿� ����غ���.

						// If I want to abort, I would just set fRes to 
						// FALSE and return.
						bRes = FALSE;
					}
					else
						// Write operation completed successfully.
						bRes = TRUE;
				}
				break;

			default:
				// An error has occurred in WaitForSingleObject. This usually 
				// indicates a problem with the overlapped event handle.
				bRes = FALSE;
				break;
			}
		}
		
	// WriteFile if��
	///////////////////////////////////////////////	
	}
	else {
		// WriteFile completed immediately.

		if (dwWritten != nToWrite) {
			// The write operation timed out. I now need to 
			// decide if I want to abort or retry. If I retry, 
			// I need to send only the bytes that weren't sent.

			
			// If I want to abort, then I would just set fRes to 
			// FALSE and return.
			bRes = FALSE;
		}
		else
			bRes = TRUE;
	}
		

	return dwWritten;
}

// ��Ʈ�κ��� pBuff�� nToRead��ŭ �д´�.
// ������ ������ Byte���� �����Ѵ�.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	//DWORD	dwRead, dwError, dwErrorFlags;
	DWORD	dwError, dwErrorFlags;
	DWORD   dwReaded;
	COMSTAT	comstat;
	DWORD	dwRes;

	//----------------- system queue�� ������ byte���� �̸� �д´�.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);
	
	// comStat.cbInQue bytes have been received, but not read
	m_dwRemainedRX = comstat.cbInQue;		// �߿�! �̰͸�ŭ�� ������ ���� �ʳ�?

	TEST_var2 = m_dwRemainedRX;


	fTEST81++;

	
	if (m_dwRemainedRX > 0)
	{

		fTEST82++;

		// check!!!!!!!!!! dwRead ��ŭ ������ ���� ������??????????
		/*
		BOOL WINAPI ReadFile(
			_In_        HANDLE       hFile,
			_Out_       LPVOID       lpBuffer,
			_In_        DWORD        nNumberOfBytesToRead,
			_Out_opt_   LPDWORD      lpNumberOfBytesRead,
			_Inout_opt_ LPOVERLAPPED lpOverlapped
			);
*/
		// m_hComm: �����͸� ���� ��� ������ �ڵ�
		// pBuff: ���� �����͸� ������ ���� (// ThreadWatchComm�� �ִ� Buff[2048] ���������� ������)
		// nToRead: �а��� �ϴ� �� (Maximum number of bytes to be read)
		// dwRead: ������ ������ ��: ������ ����� ������ ��ŭ ������������, ������ ���� ���� �� ���� �����Ͱ� �����Ƿ�,
		// �׷��� ���� ���� �ִ�.
		// m_osRead: �񵿱� ������� �� �� ����ϴµ�, �񵿱� ������� ���� ���� ���� NULL�� �ش�.

		if (!m_bWaitingOnRead)
		{
			fTEST83++;

			
			if (! ReadFile( m_hComm, pBuff, m_dwRemainedRX, &dwReaded, &m_osRead))

				// ����: return value is nonzero (TRUE)
				// ���� or �񵿱� ���:  return value is zero (FALSE)
				//if (! ReadFile( m_hComm, pBuff, m_dwRemainedRX, &dwReaded, NULL))
			{

				m_bWaitingOnRead = true;

				fTEST84++;

				//dwRes = WaitForSingleObject(m_osRead.hEvent, READ_TIMEOUT);
				dwRes = WaitForSingleObject(m_osRead.hEvent, INFINITE);

				fTEST85++;

				m_bWaitingOnRead = false;

				switch(dwRes)
				{

					fTEST86++;

					// Read completed.
				case WAIT_OBJECT_0:
				
					
					fTEST87++;
						
					

					break;
				}

			}
			else
			{
				// ��� ReadFile �Ϸ��.

				m_bWaitingOnRead = false;

				fTEST88++;

			}
		}
	}
	else
	{
		// ���� data ��
		dwReaded = 0;

		fTEST89++;
	}

	
	fTEST90++;
	
	return dwReaded;
}

// ��Ʈ�� �����ϰ�, ���� ������ ������ 
// m_ReadData�� ������ �ڿ� MainWnd�� �޽����� ������ Buffer�� ������
// �о��� �Ű��Ѵ�.
DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD		dwEvent;
	OVERLAPPED	osStatus;
	BOOL		bOk = TRUE;
	//BYTE		buff[2048];	 // �б� ����
	BYTE		buff[BUFF_SIZE];	 // �б� ����
	DWORD		dwReaded;	 // ���� ����Ʈ��.
		
	DWORD	   dwRes;			// result from WaitForSingleObject
	DWORD      dwOvRes;         // result from GetOverlappedResult

	//BOOL     bReceivedAll = false;	// ���ŵ� byte�� �ѹ��� ��� �о��°�?

	
	// EV_RXCHAR: A new character was received and placed in the input buffer. See the "Caveat" section below for a disscussion of this flag.
	if (! SetCommMask( pComm->m_hComm, EV_RXCHAR))
		bOk = FALSE;

	// Event, OS ����.
	memset( &osStatus, 0, sizeof(OVERLAPPED));
	if (! (osStatus.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)))
		bOk = FALSE;		// error creating event; abort


	if (! bOk)
	{
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}

	// ��Ʈ�� �����ϴ� ����. (��� ���� ��: ���� ����)
	while (pComm->m_bConnected)
	{
		
		dwEvent = 0;

		// ��Ʈ�� ���� �Ÿ��� �ö����� ��ٸ���. (����ȭ ���: blocking!)
		//WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
		// ����: return value is nonzero.
		// ����: return value is zero.
		// 2��° �μ�: A pointer to a variable that receives a mask indicating the type of event that occurred.
		//             If an error occurs, the value is zero; otherwise, it is one of the following values.
		//if (!WaitCommEvent(pComm->m_hComm, &dwEvent, NULL))


		//// Issue a status event check if one hasn't been issued already.
//		if (!fWaitingOnStat)
//		{
		// ��Ʈ�� ���� �Ÿ��� �ö����� ��ٸ���. (����ȭ ���: blocking!)
		if (!WaitCommEvent( pComm->m_hComm, &dwEvent, NULL))
		//if (!WaitCommEvent( pComm->m_hComm, &dwEvent, &osStatus))
		{
			// Error
			//if (GetLastError() != ERROR_IO_PENDING)	  // Wait not delayed?
				
		}
		else
		{
			// WaitCommEvent returned immediately

			fTEST1++;
				
			if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
			{
				// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
				do	
				{

					fTEST2++;

					// ��Ʈ���� ���ŵ� data �б�
					dwReaded = pComm->ReadComm( buff, 2048);

					TEST_var6 = dwReaded;

					// TEST
					//TEST_var1 = pComm->m_QueueRead.GetSize();
					

					fTEST3++;

					if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwReaded)
					{

						fTEST4++;

						// ���ŵ� data�� �ִ��� �Ǵ�!
						if (dwReaded != 0)
						{
							// ���ŵ� data�� buffer�� ����
							for ( WORD i = 0; i < dwReaded; i++)
								pComm->m_QueueRead.PutByte(buff[i]);
						}

						// TEST
						TEST_var1 = pComm->m_QueueRead.GetSize();

												
						fTEST5++;

						// ���ź� data�� �ѹ��� ��� �о��°�?
						if (dwReaded == pComm->m_dwRemainedRX)
							pComm->m_bReceivedAll = false;				// ���� ����
						else
						{
							pComm->m_bReceivedAll = true;				// ��� ���ŵ� data �ۿ���
						}

						
					}
					else
					{

						fTEST6++;

						//AfxMessageBox("m_QueueRead FULL!");

						//strcpy(str_TEST2, str_BaudRate);

						//str_TEST2 = "m_QueueRead FULL!";
						sprintf_s(str_TEST2, size_buf, "m_QueueRead FULL!");

						pComm->m_bReceivedAll = false;

						
					}

				} while (pComm->m_bReceivedAll);


				fTEST7++;
				
/////////////////////////////////////////////////////////////////////////////////////////
				// ���⼭ ������ data �� ó������.

				BYTE aByte;
						

	//	���� ���ۿ� ����� ���� data byte �� ȹ��
	int size = (pComm->m_QueueRead).GetSize();

	// ���� QUEUE �� empty �̸�, ������.
	//if (size == 0) return 0;
	if (size == 0)
		continue;



	for (int i=0;i<size;i++)
	{
		// 1 Byte �� data �� �б� & ó��
		(pComm->m_QueueRead).GetByte(&aByte);
		
		
		//////////////////////////////////////////////////////////////////
		//aByte �̿� ���⼭ ������ ó��
	
		

		//////////////////////////////////////////////////////////////////
		// �ι��ڸ� ������� �ʱ�. �ֳ��ϸ�, ���� ������ DSP�� �ޱ� ������!
		//		if(aByte != '\0')	// NULL ���ڰ� �ƴϸ�
		//		{
		//		}
	
		// 1 Byte data��  ���� ���ۿ� ����
		rBuf[rCounter] = aByte;

		// ���ŵ� data �� ī����
		rCounter++;

		// TEST
		TEST_var4 = rCounter;


		// DSP�κ��� 32����data�� �����ߴ��� üũ
		if (rCounter == TX_BUF)
		{
			// DSP�κ��� 4���� data�� ������.
			/////////////////////////////////////////////
			// data ó�� �ڵ�

			// TEST
			cTEST[0] = rBuf[0];
			cTEST[1] = rBuf[4];
			cTEST[2] = rBuf[8];
			cTEST[3] = rBuf[12];
			cTEST[4] = rBuf[16];
			cTEST[5] = rBuf[20];
			cTEST[6] = rBuf[24];
			cTEST[7] = rBuf[28];


			if (rBuf[0] == 101 && rBuf[4] == 102 && rBuf[8] == 103 && rBuf[12] == 104)
			{
				// �̹� ���� if������ ������ �����ϹǷ�, data processing ��.

				// e (101)  Vel 1
				// rBuf 17, 18, 19�� ��Ʈ ����
				if (rBuf[1] & 0x80)			
				{
					RxBuf_Temp[0] = 0xFF000000;		// 31 ~ 24 bit �� ����
//																										31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//																									   |-----------------------|-----------------------|-------------------|-----------------|
					RxBuf_Temp[0] |= (rBuf[1] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit �� ����
				}
				else
				{
					RxBuf_Temp[0] = (rBuf[1] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit �� ����
				}

				// f (102)  Accel 0
				// rBuf 21, 22, 23�� ��Ʈ ����
				if (rBuf[5] & 0x80)
				{
					RxBuf_Temp[1] = 0xFF000000;		// 31 ~ 24 bit �� ����

					RxBuf_Temp[1] |= (rBuf[5] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit �� ����
				}
				else
				{
					RxBuf_Temp[1] = (rBuf[5] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit �� ����
				}

				// g (103)  Accel 1
				// rBuf 25, 26, 27�� ��Ʈ ����
				if (rBuf[9] & 0x80)
				{
					RxBuf_Temp[2] = 0xFF000000;		// 31 ~ 24 bit �� ����

					RxBuf_Temp[2] |= (rBuf[9] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit �� ����
				}
				else
				{
					RxBuf_Temp[2] = (rBuf[9] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit �� ����
				}

				// h (104)  ??? temp
				// rBuf 29, 30, 31�� ��Ʈ ����
				if (rBuf[13] & 0x80)
				{
					RxBuf_Temp[3] = 0xFF000000;		// 31 ~ 24 bit �� ����

					RxBuf_Temp[3] |= (rBuf[13] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit �� ����
				}
				else
				{
					RxBuf_Temp[3] = (rBuf[13] << 16);		// 23 ~ 16 bit �� ����
					RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit �� ����
					RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit �� ����
				}


				//////////////////////////////
				fHD_FT[0] = (((float)(long)RxBuf_Temp[0]) * 0.001)*10;

				fHD_Pos[1] = (((float)(long)RxBuf_Temp[1]) * 0.001)*10;
				fHD_Vel[1] = (((float)(long)RxBuf_Temp[2]) * 0.001)*100;

				// Vel_Err
				fHD_Test = (((float)(long)RxBuf_Temp[3]) * 0.001)*10;

				////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////////////
				////////// Data Save ()????????????? /////////////////////////////
							
				//////////////////////////////////////////////////////////////////////////
				// SCI Data ���� test
				Buf_Data[0] = dDSPTxIndex;
				Buf_Data[1] = fHD_Pos[0];
				Buf_Data[2] = fHD_Pos[1];
				Buf_Data[3] = fHD_Vel[0];
				Buf_Data[4] = fHD_Vel[1];
				Buf_Data[5] = fHD_Accel[0];
				Buf_Data[6] = fHD_Accel[1];
				Buf_Data[7] = fHD_FT[0];
				Buf_Data[8] = fHD_FT[1];
				Buf_Data[9] = fHD_Test;

				// Remained Data: Reserved
				Buf_Data[10] = 0.0;


				// Test: trash data
				//for (int i = 4; i < NUM_OF_DATA-1; i++)
/*
				for (int i = 10; i < NUM_OF_DATA; i++)
					Buf_Data[i] = 0.0;
*/				

				DataSave(Buf_Data);
				
				
///////////////////////////////// ���� ������ ���ؼ�Reset!!!
				rCounter = 0;
				

			}
			
			// 16�� data�� ����� ������� ����Ǿ����� üũ
			//if (rBuf[0] == 97 && rBuf[4] == 98 && rBuf[8] == 99 && rBuf[12] == 100 && rBuf[16] == 101 && rBuf[20] == 102 && rBuf[24] == 103 && rBuf[28] == 104)
			else if (rBuf[0] == 97 && rBuf[4] == 98 && rBuf[8] == 99 && rBuf[12] == 100)
			{
				// data processing
				if (rBuf[0] == 97)
				{
					// a (97)  Pos 0
					// rBuf 1, 2, 3 �� ��Ʈ ����
					// Shift ���� �� OR ��Ʈ ����

					// ��ȣ ��Ʈ�� 1 ���� ���� üũ
					if (rBuf[1] & 0x80)
					{
						// ����!
						// 31 ~ 24 bit�� 1�� ä���
						RxBuf_Temp[0] = 0xFF000000;		// 31 ~ 24 bit �� ����

						RxBuf_Temp[0] |= (rBuf[1] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit �� ����
					}
					else
					{
						// ���
						RxBuf_Temp[0] = (rBuf[1] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit �� ����
					}
				}
			
				if (rBuf[4] == 98)
				{
					// b (98)  Pos 1
					// rBuf 5, 6, 7�� ��Ʈ ����

					if (rBuf[5] & 0x80)
					{
						// ����!
						// 31 ~ 24 bit�� 1�� ä���
						RxBuf_Temp[1] = 0xFF000000;		// 31 ~ 24 bit �� ����

						RxBuf_Temp[1] |= (rBuf[5] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit �� ����

					}
					else
					{
						RxBuf_Temp[1] = (rBuf[5] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit �� ����

					}
				}

				if (rBuf[8] == 99)
				{
					// c (99)  Vel 0
					// rBuf 9, 10, 11�� ��Ʈ ����

					if (rBuf[9] & 0x80)
					{
						// ����!
						// 31 ~ 24 bit�� 1�� ä���
						RxBuf_Temp[2] = 0xFF000000;		// 31 ~ 24 bit �� ����

						RxBuf_Temp[2] |= (rBuf[9] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit �� ����

					}
					else
					{
						RxBuf_Temp[2] = (rBuf[9] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit �� ����

					}
				}

				if (rBuf[12] == 100)
				{
					// d (100)  Vel 1
					// rBuf 13, 14, 15�� ��Ʈ ����

					if (rBuf[13] & 0x80)
					{
						RxBuf_Temp[3] = 0xFF000000;		// 31 ~ 24 bit �� ����

						RxBuf_Temp[3] |= (rBuf[13] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit �� ����

					}
					else
					{
						RxBuf_Temp[3] = (rBuf[13] << 16);		// 23 ~ 16 bit �� ����
						RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit �� ����
						RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit �� ����

					}
				}

						
								


				///////////////////////////////////////////////////////////////////////
				// ����� 4���� data (Pos[2], Vel[2])�� �����Ƿ�, rCounter �� 16���� ���µǾ�� ��.
				///////////////////////////////////////////////////////////////////////
				//// DWORD �� data�� float �� data�� ��ȯ
				

				// DSP Tx Index (�۽��� 1 kHz�� �ϰ� ����.)
				dDSPTxIndex = (((float)(long)RxBuf_Temp[0]) * 0.001);
				
				fHD_Pos[0] = (((float)(long)RxBuf_Temp[1]) * 0.001)*10;
				//fHD_Pos[1] = ((float)(long)RxBuf_Temp[2]) * 0.001;

				fHD_Vel[0] = (((float)(long)RxBuf_Temp[2]) * 0.001)*100;

				fHD_Accel[0] = (((float)(long)RxBuf_Temp[3]) * 0.001)*10000;
				
			
				////////////////////////////////////
/*
				////////// Data Save ()????????????? /////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// SCI Data ���� test
				Buf_Data[0] = dDSPTxIndex;
				Buf_Data[1] = fHD_Pos[0];
				Buf_Data[2] = fHD_Pos[1];
				Buf_Data[3] = fHD_Vel[0];
				Buf_Data[4] = fHD_Vel[1];
				Buf_Data[5] = fHD_Accel[0];
				Buf_Data[6] = fHD_Accel[1];
				Buf_Data[7] = fHD_Test;



				// Test: trash data
				//for (int i = 4; i < NUM_OF_DATA-1; i++)
				for (int i = 8; i < NUM_OF_DATA-1; i++)
					Buf_Data[i] = 0.0;


				DataSave(Buf_Data);

				*/



					
				//////////////////////////////////////////////////////////////////////////
				///////////////// Reset: ���� data ��ȯ�� ���ؼ� ///////////////////////////////////////////////////
/*
				for (int i = 0; i < TX_BUF; i++)
				{
					rBuf[i] = 0;

					RxBuf_Temp[i] = 0;
				}
*/			
				// Reset rCounter
				rCounter = 0;
				///////////////////////////////////////////////////////////

			}











			else		// data�� ������� ������� �ʾ���.
			{
				// ����� buff ������ �������� ��ĭ shift & rCounter 1 ����
				for (int i = 0; i < (TX_BUF-1); i++)
				{
					rBuf[i] = rBuf[i+1];
				}

				
				cTEST[0] = 'E';
				cTEST[1] = 'R';
				cTEST[2] = 'R';
				cTEST[3] = 'O';
				cTEST[4] = 'R';
				

// 				cTEST[0] = rBuf[0];
// 				cTEST[1] = rBuf[4];
// 				cTEST[2] = rBuf[8];
// 				cTEST[3] = rBuf[12];
// 				cTEST[4] = rBuf[16];
				cTEST[5] = rBuf[20];
				cTEST[6] = rBuf[24];
				cTEST[7] = rBuf[28];

				

				rCounter--;

				// TEST
				TEST_var5++;

				// ���� ���� => �ٽ� �ݺ��� (for ��)���� �ǵ��ư��� ��� ����
				continue;
			}

			// End of TX_BUF (32) data processing part
			////////////////////////////////////////////////////////////////////
		}


		// End of for �� (������ data ó�� ��)
		/////////////////////////////////////////////////////////////
	}
	




////////////////////////////////////////////////////////////////


			}
	
		//	}
		}


	fTEST9++;
	// End of while �ݺ���
/////////////////////////////////////////////////
	}	

	fTEST10++;
	
	// ��Ʈ�� ClosePort�� ���� ������ m_bConnected �� FALSE�� �Ǿ� ����.
	CloseHandle( osStatus.hEvent);
	pComm->m_hThreadWatchComm = NULL;
	

	return TRUE;
}

CCommThread::CCommThread()
{
	m_bConnected = FALSE;
	m_bReserveMsg = FALSE;

	m_bWaitingOnRead = false;
	m_bReceivedAll = false;

	hCommWnd = NULL;
		
}
CStringArray* CCommThread::GetPorts(){
	CStringArray* ComPorts = new CStringArray;
    HKEY  hSerialCom;
    TCHAR buffer[_MAX_PATH], data[_MAX_PATH];
    DWORD len, type, dataSize;
    long  i;

    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                       _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 
                       0, 
                       KEY_ALL_ACCESS, 
                       &hSerialCom) == ERROR_SUCCESS)
    {
        for (i=0, len=dataSize=_MAX_PATH; 
            ::RegEnumValue(hSerialCom, 
                           i, 
                           buffer, 
                           &len, 
                           NULL, 
                           &type, 
                           (unsigned char*)data,
                           &dataSize)==ERROR_SUCCESS; i++, len=dataSize=_MAX_PATH)
        {
                data[dataSize-1] = NULL;
                //if (strncmp(data, "COM", 3) == 0)
				ComPorts->Add(data);
        }
 
 
        ::RegCloseKey(hSerialCom);
    }
	return ComPorts;
}
