#include "aperiodictask.h"

AperiodicTask::AperiodicTask(int number, double averageTime, double length)
{
    _number = number;
    _averageTime = averageTime;
    _length = length;
    _beforeFinish = length;
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

double AperiodicTask::GetBeforeFinish()
{
    return _beforeFinish;
}

void AperiodicTask::SetBeforeFinish(double beforeFinish)
{
    _beforeFinish = beforeFinish;
}
