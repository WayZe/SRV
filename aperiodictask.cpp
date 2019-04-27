#include "aperiodictask.h"

AperiodicTask::AperiodicTask(double averageTime, double length)
{
    _averageTime = averageTime;
    _length = length;
}

double AperiodicTask::GetAverageTime()
{
    return  _averageTime;
}
