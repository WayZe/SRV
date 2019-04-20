#ifndef TASK_H
#define TASK_H

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
private:
    int _number;
    int _period;
    double _length;
    int _limit;
    double _startTime = 0;
    double _finishTime;
    double _beforeLimit;
    bool _isAwake = true;

public:
    explicit Task(int number, int period, double length, int limit);
    int GetNumber();
    int GetPeriod();
    double GetLength();
    int GetLimit();
    void SetBeforeLimit(double limit);
    double GetBeforeLimit();
    bool GetAwake();
    void SetAwake(bool isAwake);
    double GetFinishTime();
    void SetFinishTime(double finishTime);
    double GetStartTime();
    void SetStartTime(double startTime);

signals:

public slots:
};

#endif // TASK_H
