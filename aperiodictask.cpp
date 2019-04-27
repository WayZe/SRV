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

void AperiodicTask::IncreaseStartTime(int value)
{
    _startTime += value;
}

double AperiodicTask::GetStartTime()
{
    return _startTime;
}
