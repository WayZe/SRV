#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QList>
#include <task.h>
#include <QString>

class Handler : public QObject
{
    Q_OBJECT
private:
    QList<Task *> *_tasks = new QList<Task *>();
    QString _allText = "";
    QStringList _lines;
    QStringList _lineParts;
    int _hyperperiod = -1;
    int _frame = -1;
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

public:
    explicit Handler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HANDLER_H
