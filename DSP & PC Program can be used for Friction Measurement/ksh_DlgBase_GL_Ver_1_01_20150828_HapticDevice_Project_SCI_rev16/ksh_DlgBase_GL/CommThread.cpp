
#include "CommThread.h"

#include "Data_Save.h"

#include "Parameters.h"


// 저장 데이터 저장 변수
double Buf_Data[NUM_OF_DATA];



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
	
	// 포트 열기 (Overlapped: 비동기 방식)
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
	//SetupComm( m_hComm, 4096, 4096);	
	if(!SetupComm( m_hComm, BUFF_SIZE, BUFF_SIZE))
	{
		// Error
		sprintf_s(str_TEST3, size_buf, "Error SetupComm(%d)!", GetLastError());
	}

	// 포트 비우기.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	/////////////////
	// Setting the members of the COMMTIMEOUTS structure in the following manner causes read operations
	// to complete immediately without waiting for any new data to arrive

	// 왜냐하면, data 수신 여부를 미리 알 수 있으므로!!!

	// timeout 설정.
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

// 포트에 pBuff의 내용을 nToWrite만큼 쓴다.
// 실제로 쓰여진 Byte수를 리턴한다.
DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;
	BOOL bRes;
	DWORD dwRes;

	if (! WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	// 1번째 인수: 대상 파일의 핸들
	// 2번째 인수: 데이터가 들어 있는 버퍼
	// 3번째 인수: 쓰고자 하는 byte 수
	// 4번째 인수: 실제로 쓰여진 바이트 수를 return하기 위한 DWORD 변수의 포인터
	// 5번째 인수: 비동기 입출력을 할 때 사용하는데, 비동기 입출력을 하지 않을 때는 반드시 NULL로 지정해야 함.

	// 성공: return value is nonzero (TRUE)
	// 실패 or 비동기 모드:  return value is zero (FALSE)
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

						// 나중에 고려해보자.

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
		
	// WriteFile if문
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

// 포트로부터 pBuff에 nToRead만큼 읽는다.
// 실제로 읽혀진 Byte수를 리턴한다.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	//DWORD	dwRead, dwError, dwErrorFlags;
	DWORD	dwError, dwErrorFlags;
	DWORD   dwReaded;
	COMSTAT	comstat;
	DWORD	dwRes;

	//----------------- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);
	
	// comStat.cbInQue bytes have been received, but not read
	m_dwRemainedRX = comstat.cbInQue;		// 중요! 이것만큼만 읽으면 되지 않나?

	TEST_var2 = m_dwRemainedRX;


	fTEST81++;

	
	if (m_dwRemainedRX > 0)
	{

		fTEST82++;

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

		if (!m_bWaitingOnRead)
		{
			fTEST83++;

			
			if (! ReadFile( m_hComm, pBuff, m_dwRemainedRX, &dwReaded, &m_osRead))

				// 성공: return value is nonzero (TRUE)
				// 실패 or 비동기 모드:  return value is zero (FALSE)
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
				// 즉시 ReadFile 완료됨.

				m_bWaitingOnRead = false;

				fTEST88++;

			}
		}
	}
	else
	{
		// 읽은 data 수
		dwReaded = 0;

		fTEST89++;
	}

	
	fTEST90++;
	
	return dwReaded;
}

// 포트를 감시하고, 읽힌 내용이 있으면 
// m_ReadData에 저장한 뒤에 MainWnd에 메시지를 보내어 Buffer의 내용을
// 읽어가라고 신고한다.
DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD		dwEvent;
	OVERLAPPED	osStatus;
	BOOL		bOk = TRUE;
	//BYTE		buff[2048];	 // 읽기 버퍼
	BYTE		buff[BUFF_SIZE];	 // 읽기 버퍼
	DWORD		dwReaded;	 // 읽은 바이트수.
		
	DWORD	   dwRes;			// result from WaitForSingleObject
	DWORD      dwOvRes;         // result from GetOverlappedResult

	//BOOL     bReceivedAll = false;	// 수신된 byte를 한번에 모두 읽었는가?

	
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
		
		dwEvent = 0;

		// 포트에 읽을 거리가 올때까지 기다린다. (동기화 모드: blocking!)
		//WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
		// 성공: return value is nonzero.
		// 실패: return value is zero.
		// 2번째 인수: A pointer to a variable that receives a mask indicating the type of event that occurred.
		//             If an error occurs, the value is zero; otherwise, it is one of the following values.
		//if (!WaitCommEvent(pComm->m_hComm, &dwEvent, NULL))


		//// Issue a status event check if one hasn't been issued already.
//		if (!fWaitingOnStat)
//		{
		// 포트에 읽을 거리가 올때까지 기다린다. (동기화 모드: blocking!)
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
				// 포트에서 읽을 수 있는 만큼 읽는다.
				do	
				{

					fTEST2++;

					// 포트에서 수신된 data 읽기
					dwReaded = pComm->ReadComm( buff, 2048);

					TEST_var6 = dwReaded;

					// TEST
					//TEST_var1 = pComm->m_QueueRead.GetSize();
					

					fTEST3++;

					if (BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwReaded)
					{

						fTEST4++;

						// 수신된 data가 있는지 판단!
						if (dwReaded != 0)
						{
							// 수신된 data를 buffer에 저장
							for ( WORD i = 0; i < dwReaded; i++)
								pComm->m_QueueRead.PutByte(buff[i]);
						}

						// TEST
						TEST_var1 = pComm->m_QueueRead.GetSize();

												
						fTEST5++;

						// 수신부 data를 한번에 모두 읽었는가?
						if (dwReaded == pComm->m_dwRemainedRX)
							pComm->m_bReceivedAll = false;				// 수신 종료
						else
						{
							pComm->m_bReceivedAll = true;				// 계속 수신된 data 퍼오기
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
				// 여기서 수신한 data 를 처리하자.

				BYTE aByte;
						

	//	현재 버퍼에 저장된 수신 data byte 수 획득
	int size = (pComm->m_QueueRead).GetSize();

	// 수신 QUEUE 가 empty 이면, 리턴함.
	//if (size == 0) return 0;
	if (size == 0)
		continue;



	for (int i=0;i<size;i++)
	{
		// 1 Byte 씩 data 를 읽기 & 처리
		(pComm->m_QueueRead).GetByte(&aByte);
		
		
		//////////////////////////////////////////////////////////////////
		//aByte 이용 여기서 데이터 처리
	
		

		//////////////////////////////////////////////////////////////////
		// 널문자를 고려하지 않기. 왜냐하면, 숫자 정보만 DSP로 받기 때문에!
		//		if(aByte != '\0')	// NULL 문자가 아니면
		//		{
		//		}
	
		// 1 Byte data를  수신 버퍼에 저장
		rBuf[rCounter] = aByte;

		// 수신된 data 수 카운팅
		rCounter++;

		// TEST
		TEST_var4 = rCounter;


		// DSP로부터 32개의data를 수신했는지 체크
		if (rCounter == TX_BUF)
		{
			// DSP로부터 4개의 data를 수신함.
			/////////////////////////////////////////////
			// data 처리 코드

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
				// 이미 위의 if문에서 조건을 만족하므로, data processing 함.

				// e (101)  Vel 1
				// rBuf 17, 18, 19번 비트 조작
				if (rBuf[1] & 0x80)			
				{
					RxBuf_Temp[0] = 0xFF000000;		// 31 ~ 24 bit 값 저장
//																										31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//																									   |-----------------------|-----------------------|-------------------|-----------------|
					RxBuf_Temp[0] |= (rBuf[1] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit 값 저장
				}
				else
				{
					RxBuf_Temp[0] = (rBuf[1] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit 값 저장
				}

				// f (102)  Accel 0
				// rBuf 21, 22, 23번 비트 조작
				if (rBuf[5] & 0x80)
				{
					RxBuf_Temp[1] = 0xFF000000;		// 31 ~ 24 bit 값 저장

					RxBuf_Temp[1] |= (rBuf[5] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit 값 저장
				}
				else
				{
					RxBuf_Temp[1] = (rBuf[5] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit 값 저장
				}

				// g (103)  Accel 1
				// rBuf 25, 26, 27번 비트 조작
				if (rBuf[9] & 0x80)
				{
					RxBuf_Temp[2] = 0xFF000000;		// 31 ~ 24 bit 값 저장

					RxBuf_Temp[2] |= (rBuf[9] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit 값 저장
				}
				else
				{
					RxBuf_Temp[2] = (rBuf[9] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit 값 저장
				}

				// h (104)  ??? temp
				// rBuf 29, 30, 31번 비트 조작
				if (rBuf[13] & 0x80)
				{
					RxBuf_Temp[3] = 0xFF000000;		// 31 ~ 24 bit 값 저장

					RxBuf_Temp[3] |= (rBuf[13] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit 값 저장
				}
				else
				{
					RxBuf_Temp[3] = (rBuf[13] << 16);		// 23 ~ 16 bit 값 저장
					RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit 값 저장
					RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit 값 저장
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
				// SCI Data 수신 test
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
				
				
///////////////////////////////// 다음 저장을 위해서Reset!!!
				rCounter = 0;
				

			}
			
			// 16개 data가 제대로 순서대로 저장되었는지 체크
			//if (rBuf[0] == 97 && rBuf[4] == 98 && rBuf[8] == 99 && rBuf[12] == 100 && rBuf[16] == 101 && rBuf[20] == 102 && rBuf[24] == 103 && rBuf[28] == 104)
			else if (rBuf[0] == 97 && rBuf[4] == 98 && rBuf[8] == 99 && rBuf[12] == 100)
			{
				// data processing
				if (rBuf[0] == 97)
				{
					// a (97)  Pos 0
					// rBuf 1, 2, 3 번 비트 조작
					// Shift 연산 및 OR 비트 연산

					// 부호 비트가 1 인지 유무 체크
					if (rBuf[1] & 0x80)
					{
						// 음수!
						// 31 ~ 24 bit를 1로 채우기
						RxBuf_Temp[0] = 0xFF000000;		// 31 ~ 24 bit 값 저장

						RxBuf_Temp[0] |= (rBuf[1] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit 값 저장
					}
					else
					{
						// 양수
						RxBuf_Temp[0] = (rBuf[1] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[0] |= (rBuf[2] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[0] |= rBuf[3];					// 7 ~ 0 bit 값 저장
					}
				}
			
				if (rBuf[4] == 98)
				{
					// b (98)  Pos 1
					// rBuf 5, 6, 7번 비트 조작

					if (rBuf[5] & 0x80)
					{
						// 음수!
						// 31 ~ 24 bit를 1로 채우기
						RxBuf_Temp[1] = 0xFF000000;		// 31 ~ 24 bit 값 저장

						RxBuf_Temp[1] |= (rBuf[5] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit 값 저장

					}
					else
					{
						RxBuf_Temp[1] = (rBuf[5] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[1] |= (rBuf[6] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[1] |= rBuf[7];					// 7 ~ 0 bit 값 저장

					}
				}

				if (rBuf[8] == 99)
				{
					// c (99)  Vel 0
					// rBuf 9, 10, 11번 비트 조작

					if (rBuf[9] & 0x80)
					{
						// 음수!
						// 31 ~ 24 bit를 1로 채우기
						RxBuf_Temp[2] = 0xFF000000;		// 31 ~ 24 bit 값 저장

						RxBuf_Temp[2] |= (rBuf[9] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit 값 저장

					}
					else
					{
						RxBuf_Temp[2] = (rBuf[9] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[2] |= (rBuf[10] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[2] |= rBuf[11];					// 7 ~ 0 bit 값 저장

					}
				}

				if (rBuf[12] == 100)
				{
					// d (100)  Vel 1
					// rBuf 13, 14, 15번 비트 조작

					if (rBuf[13] & 0x80)
					{
						RxBuf_Temp[3] = 0xFF000000;		// 31 ~ 24 bit 값 저장

						RxBuf_Temp[3] |= (rBuf[13] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit 값 저장

					}
					else
					{
						RxBuf_Temp[3] = (rBuf[13] << 16);		// 23 ~ 16 bit 값 저장
						RxBuf_Temp[3] |= (rBuf[14] << 8);		// 15 ~ 8 bit 값 저장
						RxBuf_Temp[3] |= rBuf[15];					// 7 ~ 0 bit 값 저장

					}
				}

						
								


				///////////////////////////////////////////////////////////////////////
				// 현재는 4개의 data (Pos[2], Vel[2])만 보내므로, rCounter 는 16에서 리셋되어야 함.
				///////////////////////////////////////////////////////////////////////
				//// DWORD 형 data를 float 형 data로 변환
				

				// DSP Tx Index (송신은 1 kHz로 하고 있음.)
				dDSPTxIndex = (((float)(long)RxBuf_Temp[0]) * 0.001);
				
				fHD_Pos[0] = (((float)(long)RxBuf_Temp[1]) * 0.001)*10;
				//fHD_Pos[1] = ((float)(long)RxBuf_Temp[2]) * 0.001;

				fHD_Vel[0] = (((float)(long)RxBuf_Temp[2]) * 0.001)*100;

				fHD_Accel[0] = (((float)(long)RxBuf_Temp[3]) * 0.001)*10000;
				
			
				////////////////////////////////////
/*
				////////// Data Save ()????????????? /////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// SCI Data 수신 test
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
				///////////////// Reset: 다음 data 변환을 위해서 ///////////////////////////////////////////////////
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











			else		// data가 순서대로 저장되지 않았음.
			{
				// 저장된 buff 정보를 왼쪽으로 한칸 shift & rCounter 1 감소
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

				// 이하 무시 => 다시 반복문 (for 문)으로 되돌아가서 계속 실행
				continue;
			}

			// End of TX_BUF (32) data processing part
			////////////////////////////////////////////////////////////////////
		}


		// End of for 문 (수신한 data 처리 끝)
		/////////////////////////////////////////////////////////////
	}
	




////////////////////////////////////////////////////////////////


			}
	
		//	}
		}


	fTEST9++;
	// End of while 반복문
/////////////////////////////////////////////////
	}	

	fTEST10++;
	
	// 포트가 ClosePort에 의해 닫히면 m_bConnected 가 FALSE가 되어 종료.
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
