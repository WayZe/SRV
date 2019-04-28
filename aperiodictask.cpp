#include "aperiodictask.h"

AperiodicTask::AperiodicTask(double startTime, double beginTime, int number, double length)
{
    _startTime = startTime;
    _beginTime = beginTime;
    _number = number;
    _length = length;
}

AperiodicTask::AperiodicTask(int number, double averageTime, double beginTime)
{
    _number = number;
    _averageTime = averageTime;
    _length = beginTime;
    _beforeFinish = beginTime;
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

double AperiodicTask::GetBeginTime()
{
    return _beginTime;
}

void AperiodicTask::SetBeginTime(double time)
{
    _beginTime = time;
}

double AperiodicTask::GetEndTime()
{
    return _endTime;
}

void AperiodicTask::SetEndTime(double time)
{
    _endTime = time;
}
