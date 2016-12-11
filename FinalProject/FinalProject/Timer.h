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
    Timer() : startTime(clock()), secsToTimeout(120) {}
    Timer(double minTimeout) : startTime(clock()), secsToTimeout(minTimeout) {}

    //Returns time elapsed in seconds. (ex. 2.783)
    double timeElapsed() { return ((clock() - startTime) / CLOCKS_PER_SEC); }
    bool timeout() { return (secsToTimeout <= timeElapsed()); }
    void updateTime() { startTime = clock(); }
};

#endif // !SYSTEM_CLOCK
#pragma once
