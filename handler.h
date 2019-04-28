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
    QList<AperiodicTask *> *_distinctAperiodicTasks = new QList<AperiodicTask *>();
    // Для вычисления времени отклика
    QList<AperiodicTask *> *_anotherAperiodicTasks = new QList<AperiodicTask *>();
    QList<QList<Element *> *> *_elements = new QList<QList<Element *> *>();
    QStringList _lines;
    QStringList _lineParts;
    int _hyperperiod = -1;
    int _frameLength = -1;
    double _currentTime = 0;
    const QString inputName = "/tasks.txt";  
    QString aperiodicName = "/output/a.txt";
    QString outputName = "";
    const double _E = 0.0000001;
    const double _step = 0.1;
    int _seed = -1;

private:
    void ReadFile();
    void Sort();
    void Print();
    void Refresh(double time);
    void Print(QString out);
    void Print(QString out, QString path);
    void DistributePeriodicTasks();
    void DistributeAperiodicTasks();
    void FillAperiodicTasksList();
    void FillPeriodicTasksList();
    int GetNextParam();
    double my_rand(int accuracy);
    uint GeneratePoisson(double a);
    void PrintElements();
    void AperiodicSort();
    double CalculateAverageTime();
    double GetCurrentTime(int i, int j);
    double GetResponse(int number);
    void PrintAperiodicCorrection();

public:
    explicit Handler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HANDLER_H
