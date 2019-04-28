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
    // Для вычисления времени отклика
    double _beginTime = 0;
    double _endTime = 0;

public:
    AperiodicTask(double startTime, double beginTime, int number, double length);
    AperiodicTask(int number, double averageTime, double length);
    double GetAverageTime();
    void IncreaseStartTime(double value);
    double GetStartTime();
    double GetLength();
    int GetNumber();
    double GetBeforeFinish();
    void SetBeforeFinish(double beforeFinish);
    double GetBeginTime();
    void SetBeginTime(double time);
    double GetEndTime();
    void SetEndTime(double time);
};

#endif // APERIODICTASK_H
