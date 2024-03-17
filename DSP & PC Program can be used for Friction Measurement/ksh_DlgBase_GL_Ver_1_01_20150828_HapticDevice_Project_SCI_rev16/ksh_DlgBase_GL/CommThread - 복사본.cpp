
#include "CommThread.h"



//////////////////////////////////////////////////////////////////////////
// test variable
int comm_test1, comm_test2, comm_test3, comm_test4, comm_test5, comm_test6, comm_test7, comm_test8, comm_test9, comm_test10, comm_test11;



// 메세지를 받을 윈도우 핸들, 부모 윈도우에서 HWND hCommWnd= this->m_hWnd로
// 설정해 준다.


// CQueue 구현부========================================= 

// Queue의 생성자
CQueue::CQueue()
{
	front = back = -1;

	// size가 이미 -1을 빼고 시작함.
	CQueue::buf_size = BUFF_SIZE - 1;

	array = new BYTE[BUFF_SIZE];
	count = 0;

	for(int i = 0; i < BUFF_SIZE; i++) {
		array[i] = 0;
	}
		
}

// Queue의 소멸자
CQueue::~CQueue()
{
	// 동적 메모리 해제
	delete [] array;
}

/*
// Queue를 초기화
void CQueue::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff, 0, BUFF_SIZE);
}

// Queue에 들어 있는 자료 개수.
int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}
*/

// Queue에 1 byte 넣음.
BOOL CQueue::PutByte(BYTE item)
{
	// 가득 찬 큐와 비어 있는 큐를 구별하기 위해, 이게 가장 먼저 와야 함.
	if (front == 0 && back == buf_size || front == back + 1) {
		//cout << "Queue is full\n";
		return FALSE;
	}
	// 초기 시작 단계
	else if (front == -1 && back == -1) {
		front = 0;
		back = 0;
		array[back] = item;
		count++;
	}
	// 원형 큐가 되도록 하는 단계
	else if (back == buf_size) {
		back = 0;
		array[back] = item;
		count++;
	}
	// 일반적인 증분 단계
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

// Queue에서 1 byte 꺼냄.
BOOL CQueue::GetByte(BYTE* pb)
//BYTE CQueue::GetByte()
{
	// 비어 있는 큐임. (시작 단계)
	if (front == -1 && back == -1) {
		//cout << "Queue is empty\n";
		return FALSE;
	}
	else {
		// data를 꺼내야 함
		if (front == back) {
			// data 꺼내기
			*pb = array[front];
			
			front = -1;
			back = -1;
						
			count--;
			
			// 에러
			return FALSE;
		}
		// 원형 큐 만드는 부분
		else if (front == buf_size) {
			// data 꺼내기
			*pb = array[front];

			front = 0;
			count--;
		}
		// 일반적인 dequeue (data 꺼내기)
		else {

			// data 꺼내기
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


// Queue에 들어 있는 자료 개수.
int CQueue::GetSize()
{
	//return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;

	// TEST
	TEST_var1 = count;

	return count;
}



// 포트 sPortName을 dwBaud 속도로 연다.
// ThreadWatchComm 함수에서 포트에 무언가 읽혔을 때 MainWnd에 알리기
// 위해 WM_COMM_READ메시지를 보낼때 같이 보낼 wPortID값을 전달 받는다.

BOOL CCommThread::OpenPort(CString m_stPort,CString m_stBaud,CString m_stDatabit,
						   	CString m_stFlow,CString m_stParity,CString m_stStop)
{
	// Local 변수.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// 변수 초기화

	// overlapped structure 변수 초기화.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (! (m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))	
		return FALSE;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		return FALSE;
	
	// 포트 열기 (Overlapped: 비동기 방식을 의미함.)
	m_hComm = CreateFile( m_stPort, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);
	
	// error check: error opening port; abort
	if (m_hComm == INVALID_HANDLE_VALUE) return FALSE;

	// 포트 상태 설정.

	// EV_RXCHAR event 설정
//	SetCommMask( m_hComm, EV_RXCHAR);	

//	WaitCommEvent( m_hComm, NULL, NULL);// ********
	// InQueue, OutQueue 크기 설정.
	SetupComm( m_hComm, 4096, 4096);	

	// 포트 비우기.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	/////////////////
	// Setting the members of the COMMTIMEOUTS structure in the following manner causes read operations
	// to complete immediately without waiting for any new data to arrive

	// 왜냐하면, data 수신 여부를 미리 알 수 있으므로!!!

	// timeout 설정.
	GetCommTimeouts( m_hComm, &timeouts);
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	//timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / atol(m_stBaud);
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_115200 / atol(m_stBaud);
	timeouts.WriteTotalTimeoutConstant = 0;
	

	
	/*
	// 통신 속도: 115200 bps의 경우, 8.681 us / bit  정도의 시간이 소요됨.
	// 한번 송수신시 비트 수: 1 (시작비트) + 8 (Data) + 1 (스탑비트) = 10 비트
	// 결국: 한번 송수신시, 0.8681 us / 하나의 data 전송
	// DSP에서 FIFO로 한번에 16개의 data를 송수신하므로, 약 1.389 ms 시간이 소요됨.
	// 비고: 그 이상의 정보를 보낼 경우, 인터럽트에 의한 시간 지연을 추가 고려해야 함.


	// DSP 제어주기 (4 kHz)의 한 제어 주기는 0.25 ms 이다.
	// 따라서, DSP에서 송신할 경우, 연속되는 읽는 문자 수는 1 ms 안에 들어와야 함.
	
	// 데이터가 들어올 때 두바이트의 입력이 이루어지는시간으로
	// 이 이상 시간만큼 다음바이트의 입력이 없으면 입력값을 리턴하고 RX_CHAR 메시지를 발생시킨다.
	// [ms] 단위
	timeouts.ReadIntervalTimeout = 1;	// 1 ms
		
	// 읽기에서 읽은 문자 수에 비례하여 총 기다리는 시간을 늘여 줍니다.
	timeouts.ReadTotalTimeoutMultiplier = 0;

	// DSP에서 200 Hz로 송신한다고 가정하면, 이는 한번 송신 (16개 data)에 약 5 ms 간격이 발생함.

	// 현재 4개의 정보만 받고 있으므로!
	// 시간은 2 ms로 지정

	// 읽기에서 총 기다리는 시간을 밀리 초로 설정합니다.
	timeouts.ReadTotalTimeoutConstant = 5;


	// 쓰기에서 쓴 문자 수에 비례하여 총 기다리는 시간을 늘여 줍니다.
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_115200 / atol(m_stBaud);
	// 쓰기에서 총 기다리는 시간을 밀리 초로 설정합니다.
	timeouts.WriteTotalTimeoutConstant = 0;
	*/


	SetCommTimeouts( m_hComm, &timeouts);

	// dcb 설정
	FillMemory(&dcb, sizeof(dcb), 0);

	//dcb.DCBlength = sizeof(DCB);
	GetCommState( m_hComm, &dcb);	// 예전 값을 읽음.
	dcb.BaudRate = atol(m_stBaud);
	dcb.ByteSize = atoi(m_stDatabit);
	dcb.Parity = m_stParity.GetLength()-2;
	dcb.StopBits = static_cast<int>(atof(m_stStop)*2-2);


	if (m_stFlow.Compare("XON/XOFF")==0) 
	{
		dcb.fInX = dcb.fOutX = 1;		// Xon, Xoff 사용.
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

	// 포트 감시 쓰레드 생성.
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
	
// 포트를 닫는다.
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

// 포트에 pBuff의 내용을 nToWrite만큼 쓴다.
// 실제로 쓰여진 Byte수를 리턴한다.
DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

	if (! WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
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

// 포트로부터 pBuff에 nToRead만큼 읽는다.
// 실제로 읽혀진 Byte수를 리턴한다.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//----------------- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);
	
	// comStat.cbInQue bytes have been received, but not read
	dwRead = comstat.cbInQue;		// 중요! 이것만큼만 읽으면 되지 않나?
	
	if (dwRead > 0)
	{
		// check!!!!!!!!!! dwRead 만큼 읽으면 되지 않을까??????????
		/*
		BOOL WINAPI ReadFile(
			_In_        HANDLE       hFile,
			_Out_       LPVOID       lpBuffer,
			_In_        DWORD        nNumberOfBytesToRead,
			_Out_opt_   LPDWORD      lpNumberOfBytesRead,
			_Inout_opt_ LPOVERLAPPED lpOverlapped
			);
*/
		// m_hComm: 데이터를 읽을 대상 파일의 핸들
		// pBuff: 읽은 데이터를 저장할 버퍼 (// ThreadWatchComm에 있는 Buff[2048] 지역변수의 포인터)
		// nToRead: 읽고자 하는 양 (Maximum number of bytes to be read)
		// dwRead: 실제로 읽혀진 양: 보통의 경우라면 지시한 만큼 읽혀지겠지만, 파일의 끝인 경우는 더 읽을 데이터가 없으므로,
		// 그렇지 않을 수도 있다.
		// m_osRead: 비동기 입출력을 할 때 사용하는데, 비동기 입출력을 하지 않을 때는 NULL로 준다.
		if (! ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead))
		{
			// Fault what?
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				// 3번째 인수: dwRead: 현재까지 입출력한 바이트 수이되 완료 시에만 제대로 조사된다.
				// 4번째 인수: 입출력이 완료될 때까지 대기할 것인가 아닌가를 지정함.
				//			TRUE: 비동기 입출력을 중간에 포기하고 입출력이 완료될 때까지 리턴하지 않는다.
				//			FALSE: 입출력 과정만 조사하고 바로 리턴하는 경우.

				// 그럼 이건 비동기 입력을 포기한게 아닌가? TRUE로 설정했으므로!
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

// 포트를 감시하고, 읽힌 내용이 있으면 
// m_ReadData에 저장한 뒤에 MainWnd에 메시지를 보내어 Buffer의 내용을
// 읽어가라고 신고한다.

DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD		dwEvent;
	OVERLAPPED	osStatus;
	BOOL		bOk = TRUE;
	BYTE		buff[2048];	 // 읽기 버퍼
	DWORD		dwRead;	 // 읽은 바이트수.

	BOOL       fWaitingOnRead = FALSE;
	BOOL       fWaitingOnStat = FALSE;
	DWORD	   dwRes;			// result from WaitForSingleObject
	DWORD      dwOvRes;         // result from GetOverlappedResult

	
	// EV_RXCHAR: A new character was received and placed in the input buffer. See the "Caveat" section below for a disscussion of this flag.
	if (! SetCommMask( pComm->m_hComm, EV_RXCHAR))
		bOk = FALSE;

	// Event, OS 설정.
	memset( &osStatus, 0, sizeof(OVERLAPPED));
	if (! (osStatus.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)))
		bOk = FALSE;		// error creating event; abort
	
	if (! bOk)
	{
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}

	// 포트를 감시하는 루프. (통신 접속 중: 무한 루프)
	while (pComm->m_bConnected)
	{
//		comm_test4++;


		dwEvent = 0;

		// 포트에 읽을 거리가 올때까지 기다린다. (동기화 모드: blocking!)
		//WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
		// 성공: return value is nonzero.
		// 실패: return value is zero.
		// 2번째 인수: A pointer to a variable that receives a mask indicating the type of event that occurred.
		//             If an error occurs, the value is zero; otherwise, it is one of the following values.
		//if (!WaitCommEvent(pComm->m_hComm, &dwEvent, NULL))


		//// Issue a status event check if one hasn't been issued already.
		if (!fWaitingOnStat)
		{
			// 이렇게 하면 block이 안됨. overlapped로 동작하는 코드
			// 비동기이므로, 제대로 동작하는 경우: false를 return 함 & ERROR_IO_PENDING
			if (!WaitCommEvent( pComm->m_hComm, &dwEvent, &osStatus))
			{
				if (GetLastError() != ERROR_IO_PENDING)
					// Error
				else	// ERROR_IS_PENDING: Overlapped I/O operation is in progress
					fWaitingOnStat = true;			// 정상적인 상태임.
			}
			else
				fWaitingOnStat = true;			// 정상적인 상태임.
		}

				
		//		comm_test5++;

		// check on overlapped operation (CommEvent)
		if (fWaitingOnStat)
		{
			// Wait a little while for an event to occur.
			// 1번째 인수: 동기화 객체가 신호상태가 되기를 기다린다.
			// 2번째 인수: 타임 아웃 시간 [ms]
			dwRes = WaitForSingleObject(osStatus.hEvent, STATUS_CHECK_TIMEOUT);

			switch(dwRes)
			{
				// Event occurred
				// check EV_RXCHAR
			case WAIT_OBJECT_0:
				// 4번째 인수: FALSE: 입출력 과정만 조사하고 바로 리턴
				//             TRUE: 완료 시점까지 대기.
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
					// 수신된 data를 읽자.
					// EV_RXCHAR: A new character was received and placed in the input buffer. See the "Caveat" section below for a disscussion of this flag.
					if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
					{
						// 포트에서 읽을 수 있는 만큼 읽는다.
						do	
						{
							dwRead = pComm->ReadComm( buff, 2048);

							if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
							{
								for ( WORD i = 0; i < dwRead; i++)
									pComm->m_QueueRead.PutByte(buff[i]);	// 수신된 data를 circular queue 버퍼에 저장
							}
							else
								AfxMessageBox("m_QueueRead FULL!");

						} while (dwRead);

						comm_test6++;

						//	읽어 가도록 메시지를 보낸다.
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
			// 포트에서 읽을 수 있는 만큼 읽는다.
			do	
			{
				dwRead = pComm->ReadComm( buff, 2048);

				if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for ( WORD i = 0; i < dwRead; i++)
						pComm->m_QueueRead.PutByte(buff[i]);	// 수신된 data를 circular queue 버퍼에 저장
				}
				else
					AfxMessageBox("m_QueueRead FULL!");

			} while (dwRead);

			comm_test6++;

			//	읽어 가도록 메시지를 보낸다.
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
	
	// 포트가 ClosePort에 의해 닫히면 m_bConnected 가 FALSE가 되어 종료.
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
