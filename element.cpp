#include "element.h"

Element::Element(int proccessNumber)
{
    _proccessNumber = proccessNumber;
}

Element::Element(double emptyTime)
{
    _emptyTime = emptyTime;
}

Element::Element(double aperProccessTime, int aperProccessNumber)
{
    _aperProccessTime = aperProccessTime;
    _aperProccessNumber = aperProccessNumber;
}

int Element::GetProcessNumber()
{
    return  _proccessNumber;
}

double Element::GetEmptyTime()
{
    return _emptyTime;
}

void Element::SetEmptyTime(double time)
{
    _emptyTime = time;
}

void Element::SetAperProccessTime(double time)
{
    _aperProccessTime = time;
}

void Element::SetAperProccessNumber(int number)
{
    _aperProccessNumber = number;
}

double Element::GetAperProccessTime()
{
    return  _aperProccessTime;
}

int Element::GetAperProccessNumber()
{
    return _aperProccessNumber;
}
