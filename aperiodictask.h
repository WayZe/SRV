#ifndef APERIODICTASK_H
#define APERIODICTASK_H

#include <QObject>

class AperiodicTask
{
private:
    double _averageTime = 0;
    double _length = 0;
    double _responseTime = 0;
    double _startTime = 0;

public:
    AperiodicTask(double averageTime, double length);
};

#endif // APERIODICTASK_H
