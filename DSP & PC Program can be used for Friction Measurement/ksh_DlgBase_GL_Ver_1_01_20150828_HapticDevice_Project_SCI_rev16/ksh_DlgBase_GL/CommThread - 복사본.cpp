
#include "CommThread.h"



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

	// TEST
	TEST_var1 = count;

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
	
	// ��Ʈ ���� (Overlapped: �񵿱� ����� �ǹ���.)
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
	SetupComm( m_hComm, 4096, 4096);	

	// ��Ʈ ����.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	/////////////////
	// Setting the members of the COMMTIMEOUTS structure in the following manner causes read operations
	// to complete immediately without waiting for any new data to arrive

	// �ֳ��ϸ�, data ���� ���θ� �̸� �� �� �����Ƿ�!!!

	// timeout ����.
	GetCommTimeouts( m_hComm, &timeouts);
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	//timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / atol(m_stBaud);
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_115200 / atol(m_stBaud);
	timeouts.WriteTotalTimeoutConstant = 0;
	

	
	/*
	// ��� �ӵ�: 115200 bps�� ���, 8.681 us / bit  ������ �ð��� �ҿ��.
	// �ѹ� �ۼ��Ž� ��Ʈ ��: 1 (���ۺ�Ʈ) + 8 (Data) + 1 (��ž��Ʈ) = 10 ��Ʈ
	// �ᱹ: �ѹ� �ۼ��Ž�, 0.8681 us / �ϳ��� data ����
	// DSP���� FIFO�� �ѹ��� 16���� data�� �ۼ����ϹǷ�, �� 1.389 ms �ð��� �ҿ��.
	// ���: �� �̻��� ������ ���� ���, ���ͷ�Ʈ�� ���� �ð� ������ �߰� ����ؾ� ��.


	// DSP �����ֱ� (4 kHz)�� �� ���� �ֱ�� 0.25 ms �̴�.
	// ����, DSP���� �۽��� ���, ���ӵǴ� �д� ���� ���� 1 ms �ȿ� ���;� ��.
	
	// �����Ͱ� ���� �� �ι���Ʈ�� �Է��� �̷�����½ð�����
	// �� �̻� �ð���ŭ ��������Ʈ�� �Է��� ������ �Է°��� �����ϰ� RX_CHAR �޽����� �߻���Ų��.
	// [ms] ����
	timeouts.ReadIntervalTimeout = 1;	// 1 ms
		
	// �б⿡�� ���� ���� ���� ����Ͽ� �� ��ٸ��� �ð��� �ÿ� �ݴϴ�.
	timeouts.ReadTotalTimeoutMultiplier = 0;

	// DSP���� 200 Hz�� �۽��Ѵٰ� �����ϸ�, �̴� �ѹ� �۽� (16�� data)�� �� 5 ms ������ �߻���.

	// ���� 4���� ������ �ް� �����Ƿ�!
	// �ð��� 2 ms�� ����

	// �б⿡�� �� ��ٸ��� �ð��� �и� �ʷ� �����մϴ�.
	timeouts.ReadTotalTimeoutConstant = 5;


	// ���⿡�� �� ���� ���� ����Ͽ� �� ��ٸ��� �ð��� �ÿ� �ݴϴ�.
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_115200 / atol(m_stBaud);
	// ���⿡�� �� ��ٸ��� �ð��� �и� �ʷ� �����մϴ�.
	timeouts.WriteTotalTimeoutConstant = 0;
	*/


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

	if (! WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeouts�� ������ �ð���ŭ ��ٷ��ش�.
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}


	comm_test1++;

	return dwWritten;
}

// ��Ʈ�κ��� pBuff�� nToRead��ŭ �д´�.
// ������ ������ Byte���� �����Ѵ�.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//----------------- system queue�� ������ byte���� �̸� �д´�.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);
	
	// comStat.cbInQue bytes have been received, but not read
	dwRead = comstat.cbInQue;		// �߿�! �̰͸�ŭ�� ������ ���� �ʳ�?
	
	if (dwRead > 0)
	{
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
		if (! ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead))
		{
			// Fault what?
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts�� ������ �ð���ŭ ��ٷ��ش�.
				// 3��° �μ�: dwRead: ������� ������� ����Ʈ ���̵� �Ϸ� �ÿ��� ����� ����ȴ�.
				// 4��° �μ�: ������� �Ϸ�� ������ ����� ���ΰ� �ƴѰ��� ������.
				//			TRUE: �񵿱� ������� �߰��� �����ϰ� ������� �Ϸ�� ������ �������� �ʴ´�.
				//			FALSE: ����� ������ �����ϰ� �ٷ� �����ϴ� ���.

				// �׷� �̰� �񵿱� �Է��� �����Ѱ� �ƴѰ�? TRUE�� ���������Ƿ�!
				while (! GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}


	comm_test2++;

	return dwRead;
}

// ��Ʈ�� �����ϰ�, ���� ������ ������ 
// m_ReadData�� ������ �ڿ� MainWnd�� �޽����� ������ Buffer�� ������
// �о��� �Ű��Ѵ�.

DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD		dwEvent;
	OVERLAPPED	osStatus;
	BOOL		bOk = TRUE;
	BYTE		buff[2048];	 // �б� ����
	DWORD		dwRead;	 // ���� ����Ʈ��.

	BOOL       fWaitingOnRead = FALSE;
	BOOL       fWaitingOnStat = FALSE;
	DWORD	   dwRes;			// result from WaitForSingleObject
	DWORD      dwOvRes;         // result from GetOverlappedResult

	
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
//		comm_test4++;


		dwEvent = 0;

		// ��Ʈ�� ���� �Ÿ��� �ö����� ��ٸ���. (����ȭ ���: blocking!)
		//WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
		// ����: return value is nonzero.
		// ����: return value is zero.
		// 2��° �μ�: A pointer to a variable that receives a mask indicating the type of event that occurred.
		//             If an error occurs, the value is zero; otherwise, it is one of the following values.
		//if (!WaitCommEvent(pComm->m_hComm, &dwEvent, NULL))


		//// Issue a status event check if one hasn't been issued already.
		if (!fWaitingOnStat)
		{
			// �̷��� �ϸ� block�� �ȵ�. overlapped�� �����ϴ� �ڵ�
			// �񵿱��̹Ƿ�, ����� �����ϴ� ���: false�� return �� & ERROR_IO_PENDING
			if (!WaitCommEvent( pComm->m_hComm, &dwEvent, &osStatus))
			{
				if (GetLastError() != ERROR_IO_PENDING)
					// Error
				else	// ERROR_IS_PENDING: Overlapped I/O operation is in progress
					fWaitingOnStat = true;			// �������� ������.
			}
			else
				fWaitingOnStat = true;			// �������� ������.
		}

				
		//		comm_test5++;

		// check on overlapped operation (CommEvent)
		if (fWaitingOnStat)
		{
			// Wait a little while for an event to occur.
			// 1��° �μ�: ����ȭ ��ü�� ��ȣ���°� �Ǳ⸦ ��ٸ���.
			// 2��° �μ�: Ÿ�� �ƿ� �ð� [ms]
			dwRes = WaitForSingleObject(osStatus.hEvent, STATUS_CHECK_TIMEOUT);

			switch(dwRes)
			{
				// Event occurred
				// check EV_RXCHAR
			case WAIT_OBJECT_0:
				// 4��° �μ�: FALSE: ����� ������ �����ϰ� �ٷ� ����
				//             TRUE: �Ϸ� �������� ���.
				if (!GetOverlappedResult(pComm->m_hComm, &osStatus, &dwOvRes, FALSE))
				{
					if (GetLastError() == ERROR_OPERATION_ABORTED)
						//UpdateStatus("WaitCommEvent aborted\r\n");
						// The I/O operation has been aborted because of either a thread exit or an application request.
					//else
						//ErrorInComm("GetOverlappedResult (in Reader)");
				}
				else       // status check completed successfully
				{
					// ���ŵ� data�� ����.
					// EV_RXCHAR: A new character was received and placed in the input buffer. See the "Caveat" section below for a disscussion of this flag.
					if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
					{
						// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
						do	
						{
							dwRead = pComm->ReadComm( buff, 2048);

							if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
							{
								for ( WORD i = 0; i < dwRead; i++)
									pComm->m_QueueRead.PutByte(buff[i]);	// ���ŵ� data�� circular queue ���ۿ� ����
							}
							else
								AfxMessageBox("m_QueueRead FULL!");

						} while (dwRead);

						comm_test6++;

						//	�о� ������ �޽����� ������.
						if (!(pComm->m_bReserveMsg))
						{

							comm_test7++;

							pComm->m_bReserveMsg = true;



							fTEST = 100;


							// Nonzero if the message is posted; otherwise 0.

							if (!PostMessage(pComm->hCommWnd, WM_COMM_READ, pComm->m_wPortID, 0))
							{
								// Failed
								comm_test8++;
							}


						}

						comm_test9++;
					}







					

				}




				fWaitingOnStat = FALSE;
				break;


			}


		}


		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			do	
			{
				dwRead = pComm->ReadComm( buff, 2048);

				if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for ( WORD i = 0; i < dwRead; i++)
						pComm->m_QueueRead.PutByte(buff[i]);	// ���ŵ� data�� circular queue ���ۿ� ����
				}
				else
					AfxMessageBox("m_QueueRead FULL!");

			} while (dwRead);

			comm_test6++;

			//	�о� ������ �޽����� ������.
			if (!(pComm->m_bReserveMsg))
			{

				comm_test7++;

				pComm->m_bReserveMsg = true;



				fTEST = 100;


				// Nonzero if the message is posted; otherwise 0.
				
				if (!PostMessage(pComm->hCommWnd, WM_COMM_READ, pComm->m_wPortID, 0))
				{
					// Failed
					comm_test8++;
				}
			
				
			}

			comm_test9++;
		}

		
	}	


	comm_test11++;
	
	// ��Ʈ�� ClosePort�� ���� ������ m_bConnected �� FALSE�� �Ǿ� ����.
	CloseHandle( osStatus.hEvent);
	pComm->m_hThreadWatchComm = NULL;


	

	return TRUE;
}

CCommThread::CCommThread()
{
	m_bConnected = FALSE;
	m_bReserveMsg = FALSE;
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
