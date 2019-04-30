#include "handler.h"

#include <QList>
#include <QFile>
#include <QDebug>
#include <cmath> // для round
#include <QCoreApplication>
#include <ctime>
#include <iostream>
#include <QDir>

#include <periodictask.h>
#include <aperiodictask.h>

Handler::Handler(QObject *parent) : QObject(parent)
{   
    while (_seed < 0)
    {
        std::cout << "Enter seed: ";
        std::cin >> _seed;
    }

    char tmp[3000];

    while (outputName == "")
    {
        std::cout << "Enter output file name: ";
        std::cin >> tmp;
        QDir(QCoreApplication::applicationDirPath()).mkdir("output");
        outputName = "/output/" + QString(tmp) + ".txt";
        aperiodicName = "/" + aperiodicName.split('/')[1] + "/" +
                        QString(tmp) + aperiodicName.split('/')[2];
        qDebug() << aperiodicName;
    }

    ReadFile();

    _hyperperiod = GetNextParam();

    _frameLength = GetNextParam();

    Print("2\n");

    FillAperiodicTasksList();

    FillPeriodicTasksList();

    DistributePeriodicTasks();

    DistributeAperiodicTasks();

    //CalculateAverageTime();

    PrintAperiodicCorrection();

    PrintElements();

    std::cout << ("Data is added to '" +
                  outputName + "' file.").toLocal8Bit().constData();
}

int Handler::GetNextParam()
{
    int param = _lines.at(0).toInt();
    _lines.removeAt(0);
    Print(QString::number(param) + "\n");

    return param;
}

void Handler::PrintAperiodicCorrection()
{
    QString out = "";

    for (int i = 0; i < _anotherAperiodicTasks->length(); i++)
    {
        out = "";
        out += QString::number(_anotherAperiodicTasks->at(i)->GetNumber()) +
               " " +
               QString::number(_anotherAperiodicTasks->at(i)->GetStartTime()) +
               " " +
               QString::number(_anotherAperiodicTasks->at(i)->GetLength()) +
               " " +
               QString::number(_anotherAperiodicTasks->at(i)->GetBeginTime()) +
               " " +
               QString::number(_anotherAperiodicTasks->at(i)->GetBeginTime() -
                                _anotherAperiodicTasks->at(i)->GetStartTime());
        Print(out + "\n", aperiodicName);
    }

    Print("\n", aperiodicName);

    for (int i = 0; i < _distinctAperiodicTasks->length(); i++)
    {
        Print(QString::number(i+1) + " " +
              QString::number(GetResponse(i+1)) + "\n",
              aperiodicName);
    }
}

void Handler::DistributePeriodicTasks()
{
    double emptyTime = 0;
    QString out = "";

    for (int i = 0; i < _hyperperiod/_frameLength; i++)
    {
        out = "(";
        _elements->append(new QList<Element *>());

        for (int k = 0; k < _frameLength/_step; k++)
        {
            Refresh(_step);
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
                emptyTime += _step;
                if (_frameLength/_step - (k + 1) < _E)
                {
                    _elements->at(i)->append(new Element(emptyTime));
                    emptyTime = 0;
                }

                _currentTime += _step;
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
                        if (_E + _frameLength - (k) * _step >
                                _periodicTasks->at(l)->GetLength())
                        {
                            if (emptyTime > 0)
                            {
                                _elements->at(i)->append(
                                            new Element(emptyTime));
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
                emptyTime += _step;
                _currentTime += _step;
                if (_frameLength/_step - (k+1) < _E)
                {
                    _elements->at(i)->append(new Element(emptyTime));
                    emptyTime = 0;
                }
                continue;
            }

            for (int j = 0; j < _periodicTasks->length(); j++)
            {
                if (_periodicTasks->at(j)->GetInWork())
                {
                    _periodicTasks->at(j)->SetBeforeFinal(
                                _periodicTasks->at(j)->GetBeforeFinal() -
                                _step);

                    _currentTime += _step;
                    if (_periodicTasks->at(j)->GetBeforeFinal() < _E)
                    {
                        _periodicTasks->at(j)->SetAwake(false);
                        _periodicTasks->at(j)->SetInWork(false);
                        _periodicTasks->at(j)->SetFinishTime(_currentTime);
                        _elements->at(i)->append(
                                    new Element(
                                        _periodicTasks->at(j)->GetNumber()));
                    }
                }

                if (_periodicTasks->at(j)->GetBeforeLimit() < 0)
                {
                    Print("WARNING " +
                          QString::number(_periodicTasks->at(j)->GetNumber()));
                }
            }
        }

        out.replace(out.lastIndexOf(' '), 1, ")");

        if (i != _hyperperiod/_frameLength - 1)
        {
            out += "\n";
        }

        //Print(out);
    }
}

double Handler::GetResponse(int number)
{
    double ret = 0;
    int count = 0;

    for (int i = 0; i < _anotherAperiodicTasks->length(); i++)
    {
        if (number == _anotherAperiodicTasks->at(i)->GetNumber())
        {
            ret += _anotherAperiodicTasks->at(i)->GetBeginTime() -
                    _anotherAperiodicTasks->at(i)->GetStartTime();
            count++;
        }
    }

    if (count  == 0)
    {
        return -1;
    }

    return ret/count;
}

void Handler::DistributeAperiodicTasks()
{
    for (int i = 0; i < _elements->length(); i++)
    {
        for (int j = 0; j < _elements->at(i)->length(); j++)
        {
            if (_aperiodicTasks->at(0)->GetStartTime() <=
                    (i + 1) *_frameLength + GetCurrentTime(i, j))
            {
                if (_elements->at(i)->at(j)->GetEmptyTime() != -1)
                {
                    //Фикс 30 апреля добавление условия на появление
                    //ап процесса посередине пустого времени
                    if(_aperiodicTasks->at(0)->GetStartTime() > (i + 1) *
                            _frameLength + GetCurrentTime(i, j - 1) + _E )
                    {
                        _elements->at(i)->insert(j,
                                new Element(
                                   _aperiodicTasks->at(0)->GetStartTime() -
                                   ((i + 1) * _frameLength
                                    + GetCurrentTime(i, j -1 ))
                                    ));
                                _elements->at(i)->at(j + 1)->SetEmptyTime(
                                   _elements->at(i)->at(j + 1)->GetEmptyTime() -
                                   _elements->at(i)->at(j)->GetEmptyTime()
                                            );
                                continue;

                    }
                    _elements->at(i)->at(j)->SetAperProccessNumber(
                                _aperiodicTasks->at(0)->GetNumber());

                    if (_aperiodicTasks->at(0)->GetBeginTime() == 0)
                    {
                        _aperiodicTasks->at(0)->SetBeginTime(
                                    (i + 1) * _frameLength +
                                    GetCurrentTime(i, j));
                    }

                    if (abs(_elements->at(i)->at(j)->GetEmptyTime() -
                            _aperiodicTasks->at(0)->GetBeforeFinish()) < _E)
                    {
                        _elements->at(i)->at(j)->SetAperProccessTime(
                                    _elements->at(i)->at(j)->GetEmptyTime());
                        _elements->at(i)->at(j)->SetEmptyTime(-1);
                        _anotherAperiodicTasks->append(
                                    new AperiodicTask(
                                        _aperiodicTasks->at(0)->GetStartTime(),
                                        _aperiodicTasks->at(0)->GetBeginTime(),
                                        _aperiodicTasks->at(0)->GetNumber(),
                                        _aperiodicTasks->at(0)->GetLength()));
                        _aperiodicTasks->removeAt(0);
                        continue;
                    }

                    if (_elements->at(i)->at(j)->GetEmptyTime() <
                            _aperiodicTasks->at(0)->GetBeforeFinish())
                    {
                         _aperiodicTasks->at(0)->SetBeforeFinish(
                                     _aperiodicTasks->at(0)->GetBeforeFinish() -
                                     _elements->at(i)->at(j)->GetEmptyTime());
                        _elements->at(i)->at(j)->SetAperProccessTime(
                                    _elements->at(i)->at(j)->GetEmptyTime());
                        _elements->at(i)->at(j)->SetEmptyTime(-1);
                        continue;
                    }

                    if (_elements->at(i)->at(j)->GetEmptyTime() >
                            _aperiodicTasks->at(0)->GetBeforeFinish())
                    {
                        _elements->at(i)->at(j)->SetAperProccessTime(
                                    _aperiodicTasks->at(0)->GetBeforeFinish());
                        _elements->at(i)->insert(j + 1,
                                new Element(
                                    _elements->at(i)->at(j)->GetEmptyTime() -
                                    _aperiodicTasks->at(0)->GetBeforeFinish()));
                        _elements->at(i)->at(j)->SetEmptyTime(-1);
                        _anotherAperiodicTasks->append(
                                    new AperiodicTask(
                                        _aperiodicTasks->at(0)->GetStartTime(),
                                        _aperiodicTasks->at(0)->GetBeginTime(),
                                        _aperiodicTasks->at(0)->GetNumber(),
                                        _aperiodicTasks->at(0)->GetLength()));
                        _aperiodicTasks->removeAt(0);
                        continue;
                    }
                }
            }
        }
    }
}

double Handler::GetCurrentTime(int i, int j)
{
    double currentTime = 0;
    for (int k = 0; k < j; k++)
    {
        if (_elements->at(i)->at(k)->GetEmptyTime() != -1)
        {
            currentTime += _elements->at(i)->at(k)->GetEmptyTime();
        }

        if (_elements->at(i)->at(k)->GetAperProccessTime() != -1)
        {
            currentTime += _elements->at(i)->at(k)->GetAperProccessTime();
        }

        if (_elements->at(i)->at(k)->GetProcessNumber() != -1)
        {
            for (int l = 0; l < _periodicTasks->length(); l++)
            {
                if (_elements->at(i)->at(k)->GetProcessNumber() ==
                        _periodicTasks->at(l)->GetNumber())
                {
                    currentTime += _periodicTasks->at(l)->GetLength();
                    break;
                }
            }
        }
    }
    return currentTime;
}

void Handler::PrintElements()
{
    QString out = "";

    for (int i = 0; i < _elements->length(); i++)
    {
        out = "(";
        for (int j = 0; j < _elements->at(i)->length(); j++)
        {
            if (_elements->at(i)->at(j)->GetProcessNumber() > 0)
            {
                out += "(-1 " + QString::number(
                            _elements->at(i)->at(j)->GetProcessNumber()) + ") ";
            }
            else if (_elements->at(i)->at(j)->GetEmptyTime() > 0)
            {
                out += "(-2 " + QString::number(
                            _elements->at(i)->at(j)->GetEmptyTime()) + ") ";
            }
            else
            {
                out += "(-3 " +
                        QString::number(
                            _elements->at(i)->at(j)->GetAperProccessNumber()) +
                        " " +
                        QString::number(
                            _elements->at(i)->at(j)->GetAperProccessTime()) +
                        ") ";
            }
        }
        out.replace(out.length() - 1, 1, ")");
        Print(out + "\n");
    }
}

void Handler::FillAperiodicTasksList()
{
    for (int i = 0; i < _lines.at(0).split('\t').length(); i++)
    {
        _distinctAperiodicTasks->append(
            new AperiodicTask(i + 1,
                              _lines.at(0).split('\t').at(i).toDouble(),
                              _lines.at(1).split('\t').at(i).toDouble()));
    }

    _lines.removeAt(0);
    _lines.removeAt(0);


    for (int i = 0; i < _distinctAperiodicTasks->length(); i++)
    {
        GeneratePoisson(_distinctAperiodicTasks->at(i)->GetAverageTime());
    }

    int exit = true;
    while (exit)
    {
        exit = false;
        for (int i = 0; i < _distinctAperiodicTasks->length(); i++)
        {
            if (_distinctAperiodicTasks->at(i)->GetStartTime() < _hyperperiod)
            {
                _distinctAperiodicTasks->at(i)->IncreaseStartTime(
                        GeneratePoisson(
                            _distinctAperiodicTasks->at(i)->GetAverageTime()) +
                            _distinctAperiodicTasks->at(i)->GetLength());
                _aperiodicTasks->append(
                        new AperiodicTask(
                            _distinctAperiodicTasks->at(i)->GetNumber(),
                            _distinctAperiodicTasks->at(i)->GetAverageTime(),
                            _distinctAperiodicTasks->at(i)->GetLength()));
                _aperiodicTasks->last()->IncreaseStartTime(
                            _distinctAperiodicTasks->at(i)->GetStartTime());
            }

            if (_distinctAperiodicTasks->at(i)->GetStartTime() < _hyperperiod)
            {
                exit = true;
            }
        }
    }

    AperiodicSort();

    QString out = "";
    for (int i = 0; i < _aperiodicTasks->length(); i++)
    {
        out += "(" + QString::number(_aperiodicTasks->at(i)->GetNumber()) +
               " " + QString::number(_aperiodicTasks->at(i)->GetStartTime()) +
               ")";
    }
    Print(out + "\n");

    qDebug() << _aperiodicTasks->length();
}

double Handler::CalculateAverageTime()
{
    double count = 0;
    double sum = 0;
    for (int i = 0; i < _elements->length(); i++)
    {
        for (int j = 0; j < _elements->at(i)->length(); j++)
        {
            if (_elements->at(i)->at(j)->GetStartTime() > 0)
            {
                count++;
                sum += _elements->at(i)->at(j)->GetStartTime() -
                        _elements->at(i)->at(j)->GetAperProccessTime();
            }
        }
    }
    qDebug() << sum/count << sum << count;
    return sum/count;
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
                    _currentTime + _E)
            {
                _periodicTasks->at(k)->SetAwake(true);
                _periodicTasks->at(k)->SetStartTime(
                            _periodicTasks->at(k)->GetStartTime() +
                            _periodicTasks->at(k)->GetPeriod());
                _periodicTasks->at(k)->SetBeforeLimit(
                            _periodicTasks->at(k)->GetLimit() +
                            _periodicTasks->at(k)->GetStartTime() -
                            _currentTime);
                _periodicTasks->at(k)->SetBeforeFinal(
                            _periodicTasks->at(k)->GetLength());
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
    QFile aperiodic(QCoreApplication::applicationDirPath() + aperiodicName);
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

    if (aperiodic.open(QIODevice::WriteOnly))
    {
        aperiodic.write("");
        aperiodic.close();
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

void Handler::AperiodicSort()
{
    // Сортировка массива пузырьком
    for (int i = 0; i < _aperiodicTasks->length() - 1; i++) {
        for (int j = 0; j < _aperiodicTasks->length() - i - 1; j++) {
            if (_aperiodicTasks->at(j)->GetStartTime() >
                    _aperiodicTasks->at(j + 1)->GetStartTime())
            {
                // меняем элементы местами
#ifdef _WIN32
                _aperiodicTasks->swapItemsAt(j, j+1);
#else
                _aperiodicTasks->swap(j, j+1);
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

void Handler::Print(QString out, QString path)
{
    QFile file(QCoreApplication::applicationDirPath() + path);

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
    return X;
}

double Handler::my_rand(int accuracy)
{
    double a = 0;
    qsrand(_seed);
    a = (qrand() % (int)(pow(10, accuracy) + 1)) / pow(10, accuracy);
    return a;
}
