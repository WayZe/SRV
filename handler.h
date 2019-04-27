#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QList>
#include <QString>

#include <periodictask.h>
#include <aperiodictask.h>
#include <element.h>

class Handler : public QObject
{
    Q_OBJECT
private:
    QList<PeriodicTask *> *_periodicTasks = new QList<PeriodicTask *>();
    QList<AperiodicTask *> *_aperiodicTasks = new QList<AperiodicTask *>();
    QList<QList<Element *> *> *_elements = new QList<QList<Element *> *>();
    QStringList _lines;
    QStringList _lineParts;
    int _hyperperiod = -1;
    int _frameLength = -1;
    double _currentTime = 0;
    const QString inputName = "/tasks.txt";
    const QString outputName = "/output.txt";
    const double _E = 0.0000001;

private:
    void ReadFile();
    void Sort();
    void Print();
    void Refresh(double time);
    void Print(QString out);
    void DistributePeriodicTasks();
    void FillAperiodicTasksList();
    void FillPeriodicTasksList();
    int GetNextParam();
    double my_rand(int accuracy);
    uint GeneratePoisson(double a);
    void PrintElements();

public:
    explicit Handler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HANDLER_H
