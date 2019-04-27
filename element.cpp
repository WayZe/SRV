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
