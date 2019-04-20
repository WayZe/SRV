#include "handler.h"
#include <QList>
#include <task.h>
#include <QFile>
#include <QDebug>
#include <cmath> // для round
#include <QCoreApplication>

Handler::Handler(QObject *parent) : QObject(parent)
{
    ReadFile();

    _lines = _allText.split('\n');

    _hyperperiod = _lines.at(0).toInt();
    _lines.removeAt(0);
    _frame = _lines.at(0).toInt();
    _lines.removeAt(0);

    Print("Hyperperiod: " + QString::number(_hyperperiod));
    Print("Frame: " + QString::number(_frame));

    Print("\nTasks");
    foreach (QString line, _lines)
    {
        _tasks->append(new Task(line.split('\t').at(0).toInt(), line.split('\t').at(1).toInt(), line.split('\t').at(2).toDouble(), line.split('\t').at(3).toInt()));
    }

    FillFrames();

    Print();

    Print("\nFrames");

    for (int i = 0; i < _hyperperiod/_frame; i++)
    {
        QString out = "";

        Sort();

        for (int j = 0; j < _tasks->length(); j++)
        {
            if (_tasks->at(j)->GetAwake())
            {
                if (_frames.at(i) >= _tasks->at(j)->GetLength())
                {
                    _frames[i] = _frames.at(i) - _tasks->at(j)->GetLength();
                    _frames[i] = round(_frames[i]*10)/10;  // Округление костыль
                    _tasks->at(j)->SetAwake(false);
                    _currentTime += _tasks->at(j)->GetLength();
                    _tasks->at(j)->SetFinishTime(_currentTime);

                    for (int k = 0; k < std::round(_tasks->at(j)->GetLength() * 10); k++)
                    {
                        out += QString::number(_tasks->at(j)->GetNumber()) + "\t";
                    }
                    Refresh(_tasks->at(j)->GetLength());
                }
            }

            if (_tasks->at(j)->GetBeforeLimit() < 0)
            {
                Print("WARNING " + QString::number(_tasks->at(j)->GetNumber()));
            }
        }

        //out += "F " + QString::number(_frames.at(i));

        Print(out);

        _currentTime = _frame * (i+1);

        Refresh(_frames.at(i));
    }

    qDebug() << "Data is added to 'output.txt' file.";
}

void Handler::Refresh(double time)
{
    for (int k = 0; k < _tasks->length(); k++)
    {
        if (!_tasks->at(k)->GetAwake())
        {
            if(_tasks->at(k)->GetStartTime() + _tasks->at(k)->GetPeriod() <= _currentTime)
            {
                _tasks->at(k)->SetAwake(true);
                _tasks->at(k)->SetStartTime(_tasks->at(k)->GetStartTime() + _tasks->at(k)->GetPeriod());
                _tasks->at(k)->SetBeforeLimit(_tasks->at(k)->GetLimit() + _tasks->at(k)->GetStartTime() - _currentTime);
            }
        }
        else
        {
            _tasks->at(k)->SetBeforeLimit(_tasks->at(k)->GetBeforeLimit() - time);
        }
    }
}

void Handler::ReadFile()
{
    QFile file(QCoreApplication::applicationDirPath() + "\\tasks.txt");
    QFile output(QCoreApplication::applicationDirPath() + "\\output.txt");

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
    for (int i = 0; i < _tasks->length() - 1; i++) {
        for (int j = 0; j < _tasks->length() - i - 1; j++) {
            if (_tasks->at(j)->GetBeforeLimit() > _tasks->at(j + 1)->GetBeforeLimit())
            {
                // меняем элементы местами
                _tasks->swapItemsAt(j, j+1);
            }
        }
    }
}

void Handler::Print()
{
    QFile file(QCoreApplication::applicationDirPath() + "\\output.txt");

    if (file.open(QIODevice::Append))
    {;
        foreach (Task *task, *_tasks)
        {
            //qDebug() << task->GetNumber() << task->GetPeriod() << task->GetLength() << task->GetLimit() << task->GetBeforeLimit() << task->GetAwake();

            QString out = QString::number(task->GetNumber()) + " " + QString::number(task->GetPeriod()) + " " +
                    QString::number(task->GetLength()) + " " + QString::number(task->GetLimit()) + " " +
                    QString::number(task->GetBeforeLimit()) + " " + QString::number(task->GetAwake()) + "\n";

            file.write(out.toLocal8Bit());
        }
        file.close();
    }
    //qDebug() << "";
}

void Handler::Print(QString out)
{
    QFile file(QCoreApplication::applicationDirPath() + "\\output.txt");

    //qDebug() << out;

    if (file.open(QIODevice::Append))
    {
        file.write(out.toLocal8Bit() + "\n");
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
