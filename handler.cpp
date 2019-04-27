#include "handler.h"
#include <QList>
#include <periodictask.h>
#include <QFile>
#include <QDebug>
#include <cmath> // для round
#include <QCoreApplication>

Handler::Handler(QObject *parent) : QObject(parent)
{
    ReadFile();

    GeneratePoisson();

    _lines = _allText.split('\n');

    _hyperperiod = _lines.at(0).toInt();
    _lines.removeAt(0);
    _frame = _lines.at(0).toInt();
    _lines.removeAt(0);

    Print(QString::number(_hyperperiod) + "\n");
    Print(QString::number(_frame) + "\n");

    QString out = "(";

    foreach (QString line, _lines)
    {
        _periodicTasks->append(new PeriodicTask(line.split('\t').at(0).toInt(), line.split('\t').at(1).toInt(), line.split('\t').at(2).toDouble(), line.split('\t').at(3).toInt()));
        out += line.split('\t').at(2) + " ";
    }
    out.replace(out.lastIndexOf(' '), 1, ")");

    Print(QString::number(_periodicTasks->length()) + "\n");

    FillFrames();

    Print(out + "\n");

    for (int i = 0; i < _hyperperiod/_frame; i++)
    {
        out = "(";

        Sort();

        for (int j = 0; j < _periodicTasks->length(); j++)
        {
            if (_periodicTasks->at(j)->GetAwake())
            {
                if (_frames.at(i) >= _periodicTasks->at(j)->GetLength())
                {
                    _frames[i] = _frames.at(i) - _periodicTasks->at(j)->GetLength();
                    _frames[i] = round(_frames[i]*10)/10;  // Округление костыль
                    _periodicTasks->at(j)->SetAwake(false);
                    _currentTime += _periodicTasks->at(j)->GetLength();
                    _periodicTasks->at(j)->SetFinishTime(_currentTime);

//                    for (int k = 0; k < std::round(_tasks->at(j)->GetLength() * 10); k++)
//                    {
                        out += QString::number(_periodicTasks->at(j)->GetNumber()) + " ";
//                    }
                    Refresh(_periodicTasks->at(j)->GetLength());
                }
            }

            if (_periodicTasks->at(j)->GetBeforeLimit() < 0)
            {
                Print("WARNING " + QString::number(_periodicTasks->at(j)->GetNumber()));
            }
        }

        out.replace(out.lastIndexOf(' '), 1, ")");

        if (i != _hyperperiod/_frame - 1)
        {
            out += "\n";
        }

        Print(out);

        _currentTime = _frame * (i+1);

        Refresh(_frames.at(i));
    }

    qDebug() << "Data is added to 'output.txt' file.";
}

void Handler::Refresh(double time)
{
    for (int k = 0; k < _periodicTasks->length(); k++)
    {
        if (!_periodicTasks->at(k)->GetAwake())
        {
            if(_periodicTasks->at(k)->GetStartTime() + _periodicTasks->at(k)->GetPeriod() <= _currentTime)
            {
                _periodicTasks->at(k)->SetAwake(true);
                _periodicTasks->at(k)->SetStartTime(_periodicTasks->at(k)->GetStartTime() + _periodicTasks->at(k)->GetPeriod());
                _periodicTasks->at(k)->SetBeforeLimit(_periodicTasks->at(k)->GetLimit() + _periodicTasks->at(k)->GetStartTime() - _currentTime);
            }
        }
        else
        {
            _periodicTasks->at(k)->SetBeforeLimit(_periodicTasks->at(k)->GetBeforeLimit() - time);
        }
    }
}

void Handler::ReadFile()
{
    QFile file(QCoreApplication::applicationDirPath() + inputName);
    QFile output(QCoreApplication::applicationDirPath() + outputName);

    if (file.open(QIODevice::ReadOnly))
    {
        _allText += file.readAll();
        //qDebug() << _allText;

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
}

void Handler::Sort()
{
    // Сортировка массива пузырьком
    for (int i = 0; i < _periodicTasks->length() - 1; i++) {
        for (int j = 0; j < _periodicTasks->length() - i - 1; j++) {
            if (_periodicTasks->at(j)->GetBeforeLimit() > _periodicTasks->at(j + 1)->GetBeforeLimit())
            {
                // меняем элементы местами
#ifdef _WIN32
                _periodicTasks->swapItemsAt(j, j+1);
#else
                _tasks->swap(j, j+1);
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
            QString out = QString::number(task->GetNumber()) + " " + QString::number(task->GetPeriod()) + " " +
                    QString::number(task->GetLength()) + " " + QString::number(task->GetLimit()) + " " +
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

void Handler::FillFrames()
{
    for (int i = 0; i < _hyperperiod/_frame; i++)
    {
        _frames.append(_frame);
    }
}

int Handler::CalcFactorial(int n)
{
    int k = 1;
    for (int i = 0; i < n; i++)
    {
        k *= ( n - i);
    }
    return k;
}


void Handler::GeneratePoisson()
{
    int lyambda = 5;
    for (int k = 0; k < 5; k++)
    {
        qDebug() << (pow(lyambda, k) * pow(M_E, -lyambda)) / CalcFactorial(k);
    }
}
