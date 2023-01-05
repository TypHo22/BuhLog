#ifndef BUHLOG_H
#define BUHLOG_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <iostream>
#include <QTextStream>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(network);
Q_DECLARE_LOGGING_CATEGORY(server);
Q_DECLARE_LOGGING_CATEGORY(client);

class BuhLog
{
public:
    explicit BuhLog();
    ~BuhLog() = default;

    static void attach();
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString & msg);

    static void setLogging(bool logging,const QString& category, QtMsgType type);


private:
    static bool logging_;
    static QString filename_;


signals:

};

#endif //BUHLOG_H
