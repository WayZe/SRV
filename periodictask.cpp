#include "periodictask.h"

PeriodicTask::PeriodicTask(int number, int period, double length, int limit)
{
    _number = number;
    _period = period;
    _length = length;
    _limit = limit;
    _beforeLimit = limit;
    _beforeFinal = length;
}

int PeriodicTask::GetNumber()
{
    return _number;
}

int PeriodicTask::GetPeriod()
{
    return _period;
}

double PeriodicTask::GetLength()
{
    return _length;
}

int PeriodicTask::GetLimit()
{
    return _limit;
}

void PeriodicTask::SetBeforeLimit(double limit)
{
    _beforeLimit = limit;
}

double PeriodicTask::GetBeforeLimit()
{
    return _beforeLimit;
}

bool PeriodicTask::GetAwake()
{
    return _isAwake;
}

void PeriodicTask::SetAwake(bool isAwake)
{
    _isAwake = isAwake;
}

double PeriodicTask::GetFinishTime()
{
    return _finishTime;
}

void PeriodicTask::SetFinishTime(double finishTime)
{
    _finishTime = finishTime;
}

double PeriodicTask::GetStartTime()
{
    return  _startTime;
}

void PeriodicTask::SetStartTime(double startTime)
{
    _startTime = startTime;
}

void PeriodicTask::SetInWork(bool inWork)
{
    _inWork = inWork;
}

bool PeriodicTask::GetInWork()
{
    return _inWork;
}

double PeriodicTask::GetBeforeFinal()
{
    return _beforeFinal;
}

void PeriodicTask::SetBeforeFinal(double beforeFinal)
{
    _beforeFinal = beforeFinal;
}
