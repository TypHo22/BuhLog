#ifndef BUHLOG_H
#define BUHLOG_H

#include <QLoggingCategory>
#include <LogManager.h>

Q_DECLARE_LOGGING_CATEGORY(network);
Q_DECLARE_LOGGING_CATEGORY(server);
Q_DECLARE_LOGGING_CATEGORY(client);

class BuhLog
{
public:
    explicit BuhLog();
    ~BuhLog();

    static void attach();
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString & msg);

    static void setLogging(bool logging,const QString& category, QtMsgType type);

    static void configureLogger(bool singleLogFile, int maxFileSize, int maxLogSeconds);

private:
    static bool logging_;
    static QString filename_;
    static LogManager* manager_;


};

#endif //BUHLOG_H
