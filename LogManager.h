#ifndef LOGMANAGER_H
#define LOGMANAGER_H
//STL
#include <vector>
#include <memory>
//Qt
#include <QFile>
#include <QDateTime>

class LogManager
{
public:
    struct LogFile
    {
        std::unique_ptr<QFile> file_;
        QString threadId_;
        QDateTime timeCreated_;
    };

public:
    LogManager();

    ~LogManager();

    /**
     * @brief getFile
     * Return logFile for requested thread
     * @return
     */
    QFile* getFile();

    /**
     * @brief setMaxFileSize
     * @param maxFileSize
     */
    void setMaxFileSize(int maxFileSize);

    /**
     * @brief setMaxLogSeconds
     * @param maxLogSeconds
     */
    void setMaxLogSeconds(int maxLogSeconds);

    /**
     * @brief setOneLogFile
     * @param oneLogFile
     */
    void setSingleLog(bool singleLog);

    /**
     * @brief setRequestedThread
     * @param requestedThread
     */
    void setRequestedThread(const QString &requestedThread);

    /**
     * @brief closeAll
     * close all open log files
     */
    void closeAll();

private:
    /**
     * @brief addNewFile
     * add a new file to files_ vector
     */
    void addNewFile();

    /**
     * @brief removeFile
     * remove and close a certain log file
     * @param pickedLog
     */
    void removeFile(LogFile *pickedLog);

    /**
     * @brief checkMaxFileSize
     * check if fileSize is smaller than
     * max allowed log File size
     * @param logFile
     * @return
     */
    bool checkMaxFileSize(LogFile *logFile) const;

    /**
     * @brief checkMaxLogTime
     * check if file is longer alive than
     * max allowed up time
     * @param logFile
     * @return
     */
    bool checkMaxLogTime(LogFile *logFile) const;

    /**
     * @brief checkLogValid
     * @param logFile
     * check maxFileSize and maxLogTime
     * @return
     */
    bool checkLogValid(LogFile *logFile) const;

    /**
     * @brief singleLog
     * all threads log into one data file
     * @return
     */
    LogFile* singleLog();

    /**
     * @brief multiLog
     * each thread has its own log file
     * @return
     */
    LogFile* multiLog();

private:
    std::vector<LogFile> files_;
    QString requestedThread_;

    int maxFileSize_;
    int maxLogSeconds_;
    bool singleLog_;
};
#endif //LOGMANAGER_H
