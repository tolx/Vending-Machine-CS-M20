/***************************************************
*	Final Project 2016
*	Timer Class file
*
*   Members who worked on this:
*	Riley Wallace
*
***************************************************/

#ifndef TIMER
#define TIMER

#include <time.h>


class Timer
{
    double startTime;
    double secsToTimeout;

public:
    //Default constructor sets time threshold to 2 minutes (120 seconds)
    Timer() : startTime(clock()), secsToTimeout(120) {}
    Timer(double minTimeout) : startTime(clock()), secsToTimeout(minTimeout) {}


    //Returns time elapsed in seconds. (ex. 2.783)
    double timeElapsed() { return ((clock() - startTime) / CLOCKS_PER_SEC); }
    //returns true if timeElapsed exceeds the set time for timing out
    bool timedOut() { return (secsToTimeout <= timeElapsed()); }
    //resets timer
    void updateTime() { startTime = clock(); }
};

#endif // !SYSTEM_CLOCK
#pragma once
