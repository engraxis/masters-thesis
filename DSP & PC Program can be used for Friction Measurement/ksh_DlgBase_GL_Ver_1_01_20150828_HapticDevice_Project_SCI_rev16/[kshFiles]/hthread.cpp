// File:        hThread.cpp
// Description: Implementation of the CHapticThread class.

// multi threaded app
#ifndef _MT
#error This must be compiled as a multithreaded application. Change project settings.
// On the Project menu, click Settings. In the Project Settings dialog box, click the C/C++ tab. 
// Select Code Generation from the Category drop-down list box. 
// From the Use Run-Time Library drop-down box, select Multithreaded. Click OK. 
#endif // _MT

#include <windows.h>
#include <process.h>
#include <assert.h>
#include <conio.h>
#include "hthread.h"


// Function:    CHapticThread
// Description: Constructor: Initializes the member variables.
CHapticThread::CHapticThread()
{
    m_fRunning          = FALSE;
    m_hEvent            = NULL;
    m_hThread           = NULL; 
    m_dwThreadAddress   = 0;
}


// Function:    ~CHapticThread
// Description: Destructor: Initializes the member variables.
CHapticThread::~CHapticThread()
{
    // Stop MUST be called before destruction...
    assert (m_fRunning == FALSE);
}


// Function:    Start
// Description: Starts the Thread and event that it waits on.  The event is pulsed
//              every "period" milliseconds by the EventThread() which is run
//              called by the multimedia timer system through the timeSetEvent call.
BOOL CHapticThread::Start(UINT period, LPOUTPUTCALLBACK fnOutputFunction, void *pvData)
{    
    BOOL        bResult;
    ULONG       uResult;
    MMRESULT    mmRes;

    m_fnOutputFunction  = fnOutputFunction;
    m_pvUserData        = pvData;

    // if we're already going, this is easy:
    if (m_fRunning)
        return TRUE;

    // set this before our thread is created, so that it doesn't exit on first loop.
    m_fRunning = TRUE;
    
   	// Create Event
    m_hEvent = ::CreateEvent( NULL, TRUE, FALSE, "CHapticThread Event");
	assert( m_hEvent != NULL );

    // Create our thread
    uResult = _beginthreadex(NULL, 0, Thread, (void *)(this), 0, (unsigned *)(&m_dwThreadAddress) );
    assert(uResult != 0);
        // uResult is our ThreadHandle
    m_hThread  = (HANDLE) uResult;
    
    // Set the priority of the thread to the highest possible
    bResult = ::SetThreadPriority( m_hThread, THREAD_PRIORITY_HIGHEST );
    assert (bResult);

    // Create Timer
    m_dwPeriod = GetClosestPossiblePeriod(period);
    assert(m_dwPeriod);

    mmRes = ::timeBeginPeriod( m_dwPeriod );
    assert(mmRes == TIMERR_NOERROR);

    m_TimerID = ::timeSetEvent( m_dwPeriod, m_dwPeriod, EventHandler, (DWORD)this, TIME_PERIODIC );
    assert( m_TimerID != NULL );
    
    return TRUE;
}

// Function:    Stop
// Description: Undoes everything in Start
BOOL CHapticThread::Stop()
{
    BOOL        bResult;
    DWORD       dwResult;
    MMRESULT    mmRes;

    // if it's not started, our job is easy:
    if (!m_fRunning)
        return TRUE;

    // Kill Timer
	mmRes = ::timeKillEvent( m_TimerID );
	assert(mmRes == TIMERR_NOERROR);

    mmRes = ::timeEndPeriod( m_dwPeriod );
    assert(mmRes == TIMERR_NOERROR);

    // this lets Thread() exit
    m_fRunning = FALSE;
    bResult = ::PulseEvent( m_hEvent );
    assert (bResult);
    
    // give it a chance to exit...
    dwResult = WaitForSingleObject(m_hThread, 10000);
    assert (dwResult == WAIT_OBJECT_0);

    // close the thread handle
    bResult = CloseHandle(m_hThread);
    m_hThread = NULL;
    assert(bResult);

    // Close the Event Handle
    bResult = CloseHandle(m_hEvent);
    m_hEvent = NULL;
    assert(bResult);

    return TRUE;
}

// GetClosestPossiblePeriod
// Helper function to clip the desired period to closest one the timer can handle
UINT CHapticThread::GetClosestPossiblePeriod(UINT period)
{
    TIMECAPS    timecaps;
    MMRESULT    mmRes;

    mmRes = ::timeGetDevCaps( &timecaps, sizeof(TIMECAPS) );
    assert(mmRes == TIMERR_NOERROR);

    if ( period < timecaps.wPeriodMin )
        return timecaps.wPeriodMin;

    if ( period > timecaps.wPeriodMax )
        return timecaps.wPeriodMax;

    return period;
}

// EventHandler
// This is called every period milliseconds after a successful call to 
//  timeSetEvent in Start().  All it needs to do is pulse the event that 
//  our Thread() is waiting for...
void PASCAL 
CHapticThread::EventHandler( UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    BOOL            bResult;
    CHapticThread*  me;
    
    me = (CHapticThread*)(dwUser);

    	// Pulse event
	bResult = ::PulseEvent( me->m_hEvent );

    assert (bResult);
}

// This is the actual Thread, it waits for the event to fire and then
// calls our client's output function.
unsigned int __stdcall
CHapticThread::Thread( void *pv )
{
    DWORD           dwResult;
    CHapticThread*  me;
    
    me = (CHapticThread*)(pv);

    while (me->IsRunning())
    {
        dwResult = WaitForSingleObject(me->m_hEvent, 3000);
        assert(dwResult == WAIT_OBJECT_0);

        me->m_fnOutputFunction( me->m_pvUserData );
    }

    return TRUE;
}
