#ifndef APERIODICTASK_H
#define APERIODICTASK_H

#include <QObject>

class AperiodicTask
{
private:
    int _number = 0;
    double _averageTime = 0;
    double _length = 0;
    double _responseTime = 0;
    double _startTime = 0;
    double _beforeFinish;

public:
    AperiodicTask(int number, double averageTime, double length);
    double GetAverageTime();
    void IncreaseStartTime(double value);
    double GetStartTime();
    double GetLength();
    int GetNumber();
    double GetBeforeFinish();
    void SetBeforeFinish(double beforeFinish);
};

#endif // APERIODICTASK_H
