#include <BuhLog.h>
//QT
#include <QCoreApplication>
#include <QDebug>
#include <QLoggingCategory>
//STL
#include <thread>

void func(QString msg)
{
    qInfo(client) << msg;
    qWarning(network) << msg;
    qCritical(server) << msg;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BuhLog::startUp();
    BuhLog::configureLogger(true,-1,60); //ThreadWise Logging, no file limitation, new file every 10 seconds

    qInfo(network()) << "A sample message";

    BuhLog::setLogging(false,"client",QtInfoMsg); //Disable Info logging for client class

    qInfo(client) << "No more sample message";

    BuhLog::setLogging(true,"client",QtInfoMsg); //Enable Info logging for client class

    qInfo(client) << "Again sample message";

    //write from multiple threads
    while(true)
    {
        std::thread t1(func,"SampleMsg");
        std::thread t2(func,"SampleMsg");
        std::thread t3(func,"SampleMsg");
        std::thread t4(func,"SampleMsg");
        std::thread t5(func,"SampleMsg");
        std::thread t6(func,"SampleMsg");
        std::thread t7(func,"SampleMsg");
        std::thread t8(func,"SampleMsg");
        std::thread t9(func,"SampleMsg");

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();
    }
    return a.exec();
}
