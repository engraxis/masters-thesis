#include <vector>
#include <windows.h>

#include "kshFPS.h"



/*>>>>>>>>>>>>>>>>>>>>>>>    Usage   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
FpsTracker* fps = new FpsTracker(5);
fps->timestamp();
glColor3f(YELLOW);
renderText(10, 760, BITMAP_FONT_TYPE_8_BY_13, "FPS: ");
//glColor3f(WHITE);
sprintf(buffer, "%3.3f", fps->fpsAverage());
renderText(45, 760, BITMAP_FONT_TYPE_8_BY_13, buffer);
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



FpsTracker::FpsTracker (int smoothSteps)
    : steps_(smoothSteps), nSnaps_(0)
{
    clock_ = new Clock;
    snaps_ = new double[steps_];
    memset(snaps_, 0, sizeof(double) * steps_);    
}

FpsTracker::~FpsTracker ()
{
    delete snaps_;
    delete clock_;
}

void
FpsTracker::timestamp ()
{
    clock_->inc();
    snaps_[nSnaps_ % steps_] = clock_->queryInc();
    nSnaps_++;
}

float
FpsTracker::fpsAverage () const
{
    int count = (nSnaps_ < steps_) ? nSnaps_ : steps_;
    long sum = 0L;
    for (int i = 0; i < count; ++i)
    {
        sum += snaps_[i];
    }
    if (!sum) sum = 1L; // prevent div-by-zero
    return 1000.0f * count / (float) sum;
}

float 
FpsTracker::fpsInstant () const
{
    double inc = clock_->queryInc();
    if (!inc) inc = 1L; // prevent div-by-zero
    return 1000.0f / (float) inc;
}

void
FpsTracker::setNumSteps (int smoothSteps)
{
    steps_ = smoothSteps;
    delete snaps_;
    snaps_ = new double[steps_];
    memset(snaps_, 0, sizeof(long) * steps_);
    nSnaps_ = 0;
}