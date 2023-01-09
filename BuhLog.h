#ifndef BUHLOG_H
#define BUHLOG_H
//Qt
#include <QLoggingCategory>
//BuhLog
#include <LogManager.h>
//STL
#include <memory>
#include <thread>

Q_DECLARE_LOGGING_CATEGORY(network);
Q_DECLARE_LOGGING_CATEGORY(server);
Q_DECLARE_LOGGING_CATEGORY(client);

class BuhLog
{
public:
    explicit BuhLog();

    ~BuhLog();

    /**
     * @brief startUp
     * Initialize BuhLog logger, must be called
     * before first log call
     */
    static void startUp();

    /**
     * @brief setLogging
     * silence certain logging categories and types
     * @param logging, enable or disable
     * @param category, logging category which should be enabled/disabled
     * @param type, logging type which should be enabled/disabled
     */
    static void setLogging(bool logging,
                           const QString& category,
                           QtMsgType type);

    /**
     * @brief configureLogger
     * configure logrotation
     * @param singleLogFile, if false logger, logs threadwise
     * @param maxFileSize, max file size for log files in bytes
     * @param maxLogSeconds, max lifespan of log file in seconds
     */
    static void configureLogger(bool singleLogFile,
                                int maxFileSize,
                                int maxLogSeconds);

    /**
     * @brief shutDown
     * close all files and exit clean
     */
    static void shutDown();

private:
    /**
     * @brief logMessage
     * write a message to file
     * @param type, type of message (qInfo,qDebug...)
     * @param context, additional information about log message
     * @param msg, actual log message
     */
    static void logMessage(QtMsgType type,
                           const QMessageLogContext &context,
                           const QString & msg);
private:
    static bool logging_;
    static QString filename_;
    static std::unique_ptr<LogManager> manager_;
};

#endif //BUHLOG_H
