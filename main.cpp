#include <iostream>
#include <BuhLog.h>
#include <QCoreApplication>
#include <QDebug>

#include <thread>
#include <QLoggingCategory>

void func(QString msg)
{
    qInfo(client) << msg;
    qWarning(network) << msg;
    qCritical(server) << msg;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BuhLog::attach();

    qInfo(network()) << "SampleLogMsg";

    BuhLog::setLogging(false,"client",QtInfoMsg);

    qInfo(client) << "no more sample";

    BuhLog::setLogging(true,"client",QtInfoMsg);

    qInfo(client) << "yes yes more sample";

    //threading stuff
while(true)
{
    std::thread t1(func,"from t1");
    std::thread t2(func,"from t2");
    std::thread t3(func,"from t3");
    std::thread t4(func,"from t4");
    std::thread t5(func,"from t5");
    std::thread t6(func,"from t6");
    std::thread t7(func,"from t7");
    std::thread t8(func,"from t8");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    t5.join();
    t6.join();
    t7.join();
    t8.join();
}
    return a.exec();
}
