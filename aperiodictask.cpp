#include "aperiodictask.h"

AperiodicTask::AperiodicTask(int number, double averageTime, double length)
{
    _number = number;
    _averageTime = averageTime;
    _length = length;
}

double AperiodicTask::GetAverageTime()
{
    return  _averageTime;
}

void AperiodicTask::IncreaseStartTime(double value)
{
    _startTime += value;
}

double AperiodicTask::GetStartTime()
{
    return _startTime;
}

double AperiodicTask::GetLength()
{
    return _length;
}

int AperiodicTask::GetNumber()
{
    return _number;
}
