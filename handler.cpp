#include "handler.h"

#include <QList>
#include <QFile>
#include <QDebug>
#include <cmath> // для round
#include <QCoreApplication>
#include <ctime>

#include <periodictask.h>
#include <aperiodictask.h>

Handler::Handler(QObject *parent) : QObject(parent)
{
    ReadFile();

    _hyperperiod = GetNextParam();

    _frameLength = GetNextParam();

    FillAperiodicTasksList();

    for (int i = 0; i < _aperiodicTasks->length(); i++)
    {
        GeneratePoisson(_aperiodicTasks->at(i)->GetAverageTime());
    }

    int exit = true;
//    while (exit)
//    {
//        exit = false;
//        for (int i = 0; i < _aperiodicTasks->length(); i++)
//        {
//            if (_aperiodicTasks->at(i)->GetStartTime() < _hyperperiod)
//            {
//                _aperiodicTasks->at(i)->IncreaseStartTime(
//                            GeneratePoisson(
//                                _aperiodicTasks->at(i)->GetAverageTime()));
//            }
//            //qDebug() << _aperiodicTasks->at(i)->GetStartTime();
//            if (_aperiodicTasks->at(i)->GetStartTime() < _hyperperiod)
//            {
//                exit = true;
//            }
//        }
//    }

    //qDebug() << "";
    for (int i = 0; i < _aperiodicTasks->length(); i++)
    {
        qDebug() << _aperiodicTasks->at(i)->GetStartTime();
    }

    FillPeriodicTasksList();


    DistributePeriodicTasks();

    PrintElements();

    qDebug() << "Data is added to 'output.txt' file.";
}

int Handler::GetNextParam()
{
    int param = _lines.at(0).toInt();
    _lines.removeAt(0);
    Print(QString::number(param) + "\n");

    return param;
}

void Handler::DistributePeriodicTasks()
{
    double step = 0.1;
    double emptyTime = 0;
    QString out = "";

    for (int i = 0; i < _hyperperiod/_frameLength; i++)
    {
        out = "(";
        _elements->append(new QList<Element *>());

        for (int k = 0; k < _frameLength/step; k++)
        {
            Refresh(step);
            _currentTime += step;
            Sort();

            bool flag = true;
            bool isWork = false;

            for (int l = 0; l < _periodicTasks->length(); l++)
            {
                if (_periodicTasks->at(l)->GetAwake())
                {
                    flag = false;
                }
            }

            if (flag)
            {
                emptyTime += step;
                continue;
            }

            for (int l = 0; l < _periodicTasks->length(); l++)
            {
                if (_periodicTasks->at(l)->GetInWork())
                {
                    isWork = true;
                    break;
                }
            }

            if (!isWork)
            {
                for (int l = 0; l < _periodicTasks->length(); l++)
                {
                    if (_periodicTasks->at(l)->GetAwake())
                    {
                        if (_frameLength - (k + 1) * step > _periodicTasks->at(l)->GetLength())
                        {
                            if (emptyTime > 0)
                            {
                                _elements->at(i)->append(new Element(emptyTime));
                                emptyTime = 0;
                            }

                            _periodicTasks->at(l)->SetInWork(true);
                            isWork = true;
                            break;
                        }
                    }
                }
            }

            if (!isWork)
            {
                emptyTime += step;
                continue;
            }

            for (int j = 0; j < _periodicTasks->length(); j++)
            {
                if (_periodicTasks->at(j)->GetInWork())
                {
                    _periodicTasks->at(j)->SetBeforeFinal(_periodicTasks->at(j)->GetBeforeFinal() - step);


                    if (_periodicTasks->at(j)->GetBeforeFinal() < _E)
                    {
                        _periodicTasks->at(j)->SetAwake(false);
                        _periodicTasks->at(j)->SetInWork(false);
                        _periodicTasks->at(j)->SetFinishTime(_currentTime);
                        _elements->at(i)->append(new Element(_periodicTasks->at(j)->GetNumber()));
                    }
                }

                if (_periodicTasks->at(j)->GetBeforeLimit() < 0)
                {
                    Print("WARNING " +
                          QString::number(_periodicTasks->at(j)->GetNumber()));
                }
            }

            if (emptyTime > 0)
            {
                if (k - _frameLength/step + 1 < _E)
                {
                    _elements->at(i)->append(new Element(emptyTime));
                    emptyTime = 0;
                }
            }
        }

        out.replace(out.lastIndexOf(' '), 1, ")");

        if (i != _hyperperiod/_frameLength - 1)
        {
            out += "\n";
        }

        Print(out);
    }
}

void Handler::PrintElements()
{
    QString out = "";

    for (int i = 0; i < _elements->length(); i++)
    {
        out = "";
        for (int j = 0; j < _elements->at(i)->length(); j++)
        {
            if (_elements->at(i)->at(j)->GetProcessNumber() > 0)
            {
                out += QString::number(_elements->at(i)->at(j)->GetProcessNumber()) + " ";
            }
            else if (_elements->at(i)->at(j)->GetEmptyTime() > 0)
            {
                out += QString::number(_elements->at(i)->at(j)->GetEmptyTime()) + " ";
            }
            else
            {
                out += "ERROR";
            }
        }
        qDebug() << out;
    }
}

void Handler::FillAperiodicTasksList()
{
    for (int i = 0; i < _lines.at(0).split('\t').length(); i++)
    {
        _aperiodicTasks->append(
            new AperiodicTask(_lines.at(0).split('\t').at(i).toDouble(),
            _lines.at(1).split('\t').at(i).toDouble()));
    }

    _lines.removeAt(0);
    _lines.removeAt(0);
}

void Handler::FillPeriodicTasksList()
{
    QString out = "(";

    foreach (QString line, _lines)
    {
        _periodicTasks->append(
                    new PeriodicTask(line.split('\t').at(0).toInt(),
                                     line.split('\t').at(1).toInt(),
                                     line.split('\t').at(2).toDouble(),
                                     line.split('\t').at(3).toInt()));
        out += line.split('\t').at(2) + " ";
    }
    out.replace(out.lastIndexOf(' '), 1, ")");

    Print(QString::number(_periodicTasks->length()) + "\n");

    Print(out + "\n");
}

void Handler::Refresh(double time)
{
    for (int k = 0; k < _periodicTasks->length(); k++)
    {
        if (!_periodicTasks->at(k)->GetAwake())
        {
            if(_periodicTasks->at(k)->GetStartTime() +
                    _periodicTasks->at(k)->GetPeriod() <=
                    _currentTime)
            {
                _periodicTasks->at(k)->SetAwake(true);
                _periodicTasks->at(k)->SetStartTime(
                            _periodicTasks->at(k)->GetStartTime() +
                            _periodicTasks->at(k)->GetPeriod());
                _periodicTasks->at(k)->SetBeforeLimit(
                            _periodicTasks->at(k)->GetLimit() +
                            _periodicTasks->at(k)->GetStartTime() -
                            _currentTime);
                _periodicTasks->at(k)->SetBeforeFinal(_periodicTasks->at(k)->GetLength());
            }
        }
        else
        {
            _periodicTasks->at(k)->SetBeforeLimit(
                        _periodicTasks->at(k)->GetBeforeLimit() -
                        time);
        }
    }
}

void Handler::ReadFile()
{
    QFile file(QCoreApplication::applicationDirPath() + inputName);
    QFile output(QCoreApplication::applicationDirPath() + outputName);
    QString allText = "";

    if (file.open(QIODevice::ReadOnly))
    {
        allText += file.readAll();
        file.close();
    }
    else
    {
        Print("Please, create input file 'tasks.txt'!");
        exit(0);
    }

    if (output.open(QIODevice::WriteOnly))
    {
        output.write("");
        output.close();
    }

    _lines = allText.split('\n');
}

void Handler::Sort()
{
    // Сортировка массива пузырьком
    for (int i = 0; i < _periodicTasks->length() - 1; i++) {
        for (int j = 0; j < _periodicTasks->length() - i - 1; j++) {
            if (_periodicTasks->at(j)->GetBeforeLimit() >
                    _periodicTasks->at(j + 1)->GetBeforeLimit())
            {
                // меняем элементы местами
#ifdef _WIN32
                _periodicTasks->swapItemsAt(j, j+1);
#else
                _periodicTasks->swap(j, j+1);
#endif
            }
        }
    }
}

void Handler::Print()
{
    QFile file(QCoreApplication::applicationDirPath() + outputName);

    if (file.open(QIODevice::Append))
    {;
        foreach (PeriodicTask *task, *_periodicTasks)
        {
            QString out = QString::number(task->GetNumber()) + " " +
                    QString::number(task->GetPeriod()) + " " +
                    QString::number(task->GetLength()) + " " +
                    QString::number(task->GetLimit()) + " " +
                    QString::number(task->GetBeforeLimit()) + "\n";

            file.write(out.toLocal8Bit());
        }
        file.close();
    }
}

void Handler::Print(QString out)
{
    QFile file(QCoreApplication::applicationDirPath() + outputName);

    if (file.open(QIODevice::Append))
    {
        file.write(out.toLocal8Bit());
        file.close();
    }
}

uint Handler::GeneratePoisson(double a)
{
    uint X = 0;
    double Prod = pow(M_E, -a);
    double Sum = Prod;
    double U = my_rand(3);
    while (U > Sum)
    {
        X++;
        Prod *= a/X;
        Sum += Prod;
    }
    //qDebug() << X;
    return X;
}

double Handler::my_rand(int accuracy)
{
    double a = 0;
    qsrand(time(NULL));
    a = (qrand() % (int)(pow(10, accuracy) + 1)) / pow(10, accuracy);
    return a;
}
