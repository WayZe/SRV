#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>

class Element : public QObject
{
    Q_OBJECT
private:
    int _proccessNumber = -1;
    double _emptyTime = -1;
    double _aperProccessTime = -1;
    int _aperProccessNumber =  -1;

public:
    Element(int proccessNumber);
    Element(double emptyTime);
    Element(double aperProccessTime, int aperProccessNumber);
    int GetProcessNumber();
    double GetEmptyTime();
    void SetEmptyTime(double time);
    void SetAperProccessTime(double time);
    void SetAperProccessNumber(int number);
    double GetAperProccessTime();
    int GetAperProccessNumber();

signals:

public slots:
};

#endif // ELEMENT_H
