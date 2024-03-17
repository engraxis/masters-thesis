#ifndef __KSHFPS_H__
#define __KSHFPS_H__


typedef _int64 MMC_TIME_TYPE;
#define WIN32_LEAN_AND_MEAN


namespace mmc {
    class Clock;

    /*! \brief Utility class for measuring framerate. */
    class FpsTracker {
    public:
        /*! Create a FpsTracker.  \p smoothSteps is the window size
          over which to average the time measurements. */
        FpsTracker (int smoothSteps = 4);

        /*! Deallocate a FpsTracker. */
        ~FpsTracker ();

        /*! Specify the window size \p smoothSteps over which the time
          measurements will be averaged. */
        void setNumSteps (int smoothSteps);

        /*! Makes a timestamp (i.e. takes a snapshot); measures
          time intervals between successful calls.  Usually, you want
          to call this function once per rendering loop. */
        void timestamp ();

        /*! Get the average FPS (averaged over \p smoothSteps
          interval). */
        float fpsAverage () const;

        /*! Get the instantaneous FPS (estimated from the last frame
          only). */
        float fpsInstant () const;

    private:
        Clock *clock_;
        int steps_;
        int nSnaps_;
        double *snaps_;
    };

} // namespace mmc



namespace mmc {

    /*! \brief Class for timing application events.

     Only one of these is needed per application. */
    class Timer {
    public:

        /*! Get a new timer, initially stopped. */
        inline Timer ();

        /*! Start the timer. */
		// �Լ� ����
		// ���� ī���� ���� start_ �� ȹ��.
		// last_ = now_ = start_;
        inline void start ();

        /*! Call this once per frame to advance internal timer state. */
		// �Լ� ����
		// last_ = now_
		// ���� ī���� ���� now_ �� ����
        inline void inc ();

        /*! Returns number of ms elapsed since start() was called.  Use
          this to determine how long the timer has been running. */
		// �Լ� ����
		// now_ - start_ ī���� ���� ��ȯ: ó�� ���ۿ������� ��������� �ð��� ���� ī���� �� ��ȯ
        inline MMC_TIME_TYPE queryElapsed () const;

        /*! Returns number of ms elapsed between the two previous calls to
          \p inc().  Use this to determine the time between frames. */
		// �Լ� ����
		// now_ - last_ ī���� ���� ��ȯ: �ð� ��ȭ���� ���� ī���� �� ��ȯ (�߿� �Լ�)
        inline MMC_TIME_TYPE queryInc () const;

		// ksh - new function
		//inline MMC_TIME_TYPE queryCalTime();
		inline double queryCalTime();

        /*! Returns 1 / frequency of the internal timing clock actually
          being used. */
		// �Լ� ����
		// invFreq_ �� ��ȯ
        inline double getInvFreq () const;

		// Frequency ��ȯ (1�ʿ� �ش��ϴ� ī���� ��)
		inline double getFreq () const;

		// ���� ī���� �� ��ȯ
		inline double getCnt() const;

    private:
        MMC_TIME_TYPE freq_;
        MMC_TIME_TYPE start_;
        MMC_TIME_TYPE now_;
        MMC_TIME_TYPE last_;
        MMC_TIME_TYPE elapsed_;		// unused  �����?
        double invFreq_;
		double dFreq_;

		// calculation computation times for codes
		MMC_TIME_TYPE now1_;
		MMC_TIME_TYPE elapsed1_;

    };

    //////////////////////////////////////////////////////////////////////
    //  Implementation.
    //

    inline
    Timer::Timer ()
        : start_(0), last_(0), now_(0), elapsed_(0)
    {
#ifdef WIN32
		// 1�ʿ� �����ϴ� ī��Ʈ �� => ���ļ��� ����
		// ���ļ��� ���� �Ŀ��� ���� ������ ����.
        QueryPerformanceFrequency((LARGE_INTEGER *) &freq_);
#else
#ifdef LINUX
        freq_ = 1.0;
#else
        freq_ = CLOCKS_PER_SEC;
#endif        
#endif
		// �̰� �Ƹ� �ֱ⸦ �ǹ��� ��??
        invFreq_ = 1.0 / (double) freq_;

		// 1�ʿ� �ش��ϴ� ī���� �� (���ļ�)
		dFreq_ = (double) freq_;
    }

    inline void
    Timer::start ()
    {
#ifdef WIN32
		// ���� ī���� �� ȹ��!
		// receives the current performance-counter value (in counts).
        QueryPerformanceCounter((LARGE_INTEGER *) &start_);
#else
#ifdef LINUX
        struct timeval tv;
        struct timezone tz;
        gettimeofday(&tv, &tz);
        start_ = (double) (tv.tv_sec + 1e-6 * tv.tv_usec);
#else
        start_ = clock();
#endif
#endif
        last_ = now_ = start_;
    }

    inline MMC_TIME_TYPE
    Timer::queryElapsed () const
    {
		// �۾� �ð��� ���� ��ȭ�� ī���� �� ȹ��
        return now_ - start_;
    }

    inline void
    Timer::inc ()		// ���� ī���� ���� now_ ������ ȹ��!
    {
        last_ = now_;
#ifdef WIN32
        QueryPerformanceCounter((LARGE_INTEGER *) &now_);
#else
#ifdef LINUX
        struct timeval tv;
        struct timezone tz;
        gettimeofday(&tv, &tz);
        now_ = (double) (tv.tv_sec + 1e-6 * tv.tv_usec);        
#else
        now_ = clock();
#endif
#endif		
    }

    inline double
    Timer::queryCalTime()
    {
		// ���� �ð� ��� (cnt)
		QueryPerformanceCounter((LARGE_INTEGER *) &now1_);

		elapsed1_ = now1_ - now_;


        return elapsed1_ / dFreq_;
    }

		


	inline MMC_TIME_TYPE
    Timer::queryInc () const
    {
        return now_ - last_;
    }

	

    inline double
    Timer::getInvFreq () const
    {
        return invFreq_;
    }

	inline double
	Timer::getFreq () const
	{
		return dFreq_;
	}

	// ���� ī���� �� ��ȯ
	inline double
	Timer::getCnt() const
	{
		return (double) now_;
	}
		

} // namespace mmc



namespace mmc
{

    /*! \brief High precision clock for timing events. 
      
      This class is implemented on top of the Timer class.  Usually it is
      not necessary to use the Timer class directly; instead, use this
      class. */
    class Clock
    {
    public:
        /*! Construct a clock. */
        inline Clock ();

        /*! Reset the clock (0 time elapsed). */
        inline void reset ();

        /*! Call once per frame to update the internal clock state. */
        inline void inc ();

        /*! Returns the amount of time (in ms) elapsed between last
          two calls to \p inc(). */
        inline double queryInc () const;

        /*! Returns the amount of time (in ms) elapsed since clock creation or
          reset() was called. */
        inline double queryTime () const;

        /*! Pause the clock. */
        inline void pauseToggle ();

    private:
        Timer *timer_;
        MMC_TIME_TYPE inc_;
        MMC_TIME_TYPE curTime_;
        MMC_TIME_TYPE start_;
        bool paused_;
        double invFreq_;
    };

    //////////////////////////////////////////////////////////////////////
    //  Implementation.
    //

    inline
    Clock::Clock ()
        : paused_(false)
    {
        timer_ = new Timer;
        invFreq_ = timer_->getInvFreq();
        timer_->start();
        reset();
    }

    void
    Clock::reset ()
    {
        start_ = timer_->queryElapsed();
        inc_ = curTime_ = 0;
    }

    void
    Clock::inc ()
    {
        timer_->inc();
        if (!paused_)
        {
            inc_ = timer_->queryInc();
            curTime_ += inc_;
        }
        else
        {
            inc_ = 0;
        }
    }

    // Converts internal clock increment to long in milliseconds.
    double
    Clock::queryInc () const
    {
        return (double) (1000.0 * inc_ * invFreq_);
    }

    // Converts internal clock time to long in milliseconds.
    double
    Clock::queryTime () const
    {
        return (double) (1000.0 * curTime_ * invFreq_);
    }

    void
    Clock::pauseToggle ()
    {
        if (paused_)
        {
            paused_ = false;
        }
        else
        {
            paused_ = true;
            inc_ = 0;
        }
    }

} // namespace mmc



using namespace mmc;
#endif