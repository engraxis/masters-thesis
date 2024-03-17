// File:        hthread.h
// Description: Definition of the CHapticThread class.  This class can be used 
//              create a thread that will call the provided callback at 
//              intervals of "period" ms
#ifndef _HTHREAD_H_
#define _HTHREAD_H_

typedef VOID (__stdcall * LPOUTPUTCALLBACK)(void *pv);


class CHapticThread {
public:
	CHapticThread();
	~CHapticThread();

	int     Start(UINT period, LPOUTPUTCALLBACK fnOutputFunction, void *pvData);
	int     Stop();
    BOOL    IsRunning()     { return m_fRunning; }

private:
    static VOID __stdcall EventHandler  ( UINT, UINT, DWORD, DWORD, DWORD );
    static UINT __stdcall Thread        ( void *pv );

    UINT    GetClosestPossiblePeriod(UINT period);

private:
	BOOL		        m_fRunning;
	DWORD	            m_dwPeriod;		// in milliseconds
    DWORD               m_dwThreadAddress;
    HANDLE	            m_hThread;
    HANDLE	            m_hEvent;
    MMRESULT	        m_TimerID;

   	LPOUTPUTCALLBACK	m_fnOutputFunction;
	void *				m_pvUserData;	
};

#endif
