#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QList>
#include <QString>

#include <periodictask.h>
#include <aperiodictask.h>

class Handler : public QObject
{
    Q_OBJECT
private:
    QList<PeriodicTask *> *_periodicTasks = new QList<PeriodicTask *>();
    QList<AperiodicTask *> *_aperiodicTasks = new QList<AperiodicTask *>();
    QStringList _lines;
    QStringList _lineParts;
    int _hyperperiod = -1;
    int _frameLength = -1;
    QList<double> _frames;
    double _currentTime = 0;
    const QString inputName = "/tasks.txt";
    const QString outputName = "/output.txt";

private:
    void ReadFile();
    void Sort();
    void Print();
    void FillFrames();
    void Refresh(double time);
    void Print(QString out);
    int CalcFactorial(int n);
    void GeneratePoisson();
    void DistributePeriodicTasks();
    void FillAperiodicTasksList();
    void FillPeriodicTasksList();
    int GetNextParam();

public:
    explicit Handler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HANDLER_H
