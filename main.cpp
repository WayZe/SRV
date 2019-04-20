#include <QCoreApplication>
#include <QList>
#include <task.h>
#include <QFile>
#include <QDebug>
#include "handler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Handler *handler = new Handler();
    delete  handler;
    handler = nullptr;

    return a.exec();
}
