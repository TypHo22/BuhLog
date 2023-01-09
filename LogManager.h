#ifndef LOGMANAGER_H
#define LOGMANAGER_H
//STL
#include <vector>
//Qt
#include <QFile>
#include <QDateTime>


class LogManager
{
public:
    struct LogFile
    {
        QFile* file_;
        QString threadId_;
        QDateTime timeCreated_;
    };

public:
    LogManager();
    ~LogManager();

    QFile* getFile();

    void setMaxFileSize(int maxFileSize);

    void setMaxLogSeconds(int maxLogSeconds);

    void setOneLogFile(bool oneLogFile);

    void setRequestedThread(const QString &requestedThread);

private:
    void addNewFile();
    void removeFile(LogFile *pickedLog);

    bool checkMaxFileSize(LogFile *logFile) const;
    bool checkMaxLogTime(LogFile *logFile) const;
    bool checkLogValid(LogFile *logFile) const;

    LogFile* singleLog();
    LogFile* multiLog();
private:
    std::vector<LogFile> files_;
    QString requestedThread_;

    int maxFileSize_;
    int maxLogSeconds_;
    bool oneLogFile_;
};
#endif //LOGMANAGER_H
