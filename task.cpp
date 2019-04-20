#include "task.h"

Task::Task(int number, int period, double length, int limit)
{
    _number = number;
    _period = period;
    _length = length;
    _limit = limit;
    _beforeLimit = limit;
}

int Task::GetNumber()
{
    return _number;
}

int Task::GetPeriod()
{
    return _period;
}

double Task::GetLength()
{
    return _length;
}

int Task::GetLimit()
{
    return _limit;
}

void Task::SetBeforeLimit(double limit)
{
    _beforeLimit = limit;
}

double Task::GetBeforeLimit()
{
    return _beforeLimit;
}

bool Task::GetAwake()
{
    return _isAwake;
}

void Task::SetAwake(bool isAwake)
{
    _isAwake = isAwake;
}

double Task::GetFinishTime()
{
    return _finishTime;
}

void Task::SetFinishTime(double finishTime)
{
    _finishTime = finishTime;
}

double Task::GetStartTime()
{
    return  _startTime;
}

void Task::SetStartTime(double startTime)
{
    _startTime = startTime;
}
